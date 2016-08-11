// Copyright (C) 2015, 2016 Canonical Ltd
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License version 3 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// Inspired by qtsensehat module

#include "sensehatsensorbase.h"
#include "qsensors-sensehatpluginprivate_p.h"

#include <QFile>
#include <QStandardPaths>
#include <QSensorBackend>

#include <RTIMULib.h>
#include <unistd.h>
#include <errno.h>

#define RADIANS_TO_DEGREES 57.2957795
#define STANDARD_GRAVITY 9.80665
#define RADIANS_TO_DEGREES 57.2957795

static const int MAX_READ_ATTEMPTS = 5;
//Q_LOGGING_CATEGORY(qSenseHat, "sensor.sensehat")

QSenseHatSensorsPrivate::QSenseHatSensorsPrivate(SenseHatSensorBase *q_ptr)
    : q(q_ptr),
      imuInited(false),
      humidityInited(false),
      pressureInited(false),
      temperatureFromHumidity(true)
{

}

QSenseHatSensorsPrivate::~QSenseHatSensorsPrivate()
{
    delete rtpressure;
    delete rthumidity;
    delete rtimu;
    delete settings;
}

bool QSenseHatSensorsPrivate::open()
{
    qDebug() << Q_FUNC_INFO;

    QString sensehatConfigDir = QFile::decodeName(qgetenv("SENSEHAT_CONFIG_DIR"));
    if (sensehatConfigDir.isEmpty())
        sensehatConfigDir = QString::fromLatin1("/etc/xdg");

    settings = new RTIMUSettings(sensehatConfigDir.toLocal8Bit(), "RTIMULib");

    rtimu = RTIMU::createIMU(settings);
    if ((rtimu == NULL) || (rtimu->IMUType() == RTIMU_TYPE_NULL)) {
        qDebug() << "No IMU found";
        return false;
    }
    rtimu->setGyroEnable(true);
    rtimu->setAccelEnable(true);
    rtimu->setCompassEnable(true);

//    rtimu->setDebugEnable(true);

    pollInterval = qMax(1, rtimu->IMUGetPollInterval());
    qDebug() << "IMU name" <<   rtimu->IMUName() << "Recommended poll interval" << pollInterval << "ms";

    rthumidity = RTHumidity::createHumidity(settings);
    if (rthumidity == NULL) {
        qDebug() << "Failed to create Humidity";
    } else {
        qDebug() << "Humidity sensor name" << rthumidity->humidityName();
    }
    rtpressure = RTPressure::createPressure(settings);
    if (rtpressure == NULL) {
        qDebug() << "Failed to create Pressure";
    } else {
        qDebug() <<  "Pressure sensor name" << rtpressure->pressureName();
    }
    if (!imuInited) {
        if (!rtimu->IMUInit())
            qWarning("Failed to initialize IMU");
        else
            imuInited = true;
    }
    return imuInited;
}

void QSenseHatSensorsPrivate::update(SenseHatSensorBase::UpdateFlags what)
{
    int humFlags = SenseHatSensorBase::Humidity;
    if (temperatureFromHumidity)
        humFlags |= SenseHatSensorBase::Temperature;
    if (what & humFlags) {
        if (!humidityInited) {
            humidityInited = true;
            if (!rthumidity->humidityInit())
                qWarning("Failed to initialize humidity sensor");
        }
        RTIMU_DATA data;
        if (rthumidity->humidityRead(data))
            report(data, what & humFlags);
        else
            qWarning("Failed to read humidity data");
    }

    int presFlags = SenseHatSensorBase::Pressure;
    if (!temperatureFromHumidity)
        presFlags |= SenseHatSensorBase::Temperature;
    if (what & presFlags) {
        if (!pressureInited) {
            pressureInited = true;
            if (!rtpressure->pressureInit())
                qWarning("Failed to initialize pressure sensor");
        }
        RTIMU_DATA data;
        if (rtpressure->pressureRead(data))
            report(data, what & presFlags);
        else
            qWarning("Failed to read pressure data");
    }

    const int imuFlags = SenseHatSensorBase::Gyro | SenseHatSensorBase::Acceleration
            | SenseHatSensorBase::Compass | SenseHatSensorBase::Orientation
            | SenseHatSensorBase::Magnetometer | SenseHatSensorBase::Rotation;
    if (what & imuFlags) {

        int attempts = MAX_READ_ATTEMPTS;
        while (attempts--) {
            if (rtimu->IMURead())
                break;
            usleep(pollInterval * 1000);
        }
        if (attempts >= 0)
            report(rtimu->getIMUData(), what & imuFlags);
        else
            qWarning("Failed to read intertial measurement data");
    }

}

static inline qreal toDeg360(qreal rad)
{
    const qreal deg = rad * RADIANS_TO_DEGREES;
    qreal result = deg < 0 ? deg + 360 : deg;
    qWarning() << Q_FUNC_INFO << deg << result;
    return result;
}

void QSenseHatSensorsPrivate::report(const RTIMU_DATA &data, SenseHatSensorBase::UpdateFlags what)
{
    //qDebug() << Q_FUNC_INFO << what;

//    if (what.testFlag(SenseHatSensorBase::Humidity)) {
//        if (data.humidityValid) {
//            humidity = data.humidity;
//            emit q->humidityChanged(humidity);
//        }
//    }

    if (what.testFlag(SenseHatSensorBase::Pressure)) {
        if (data.pressureValid) {
            if (pressure.pressure() != (qreal)data.pressure) {
                pressure.setTimestamp((quint64)data.timestamp);
                pressure.setPressure((qreal)data.pressure);
                emit q->pressureChanged(pressure);
            }
        }
    }

    if (what.testFlag(SenseHatSensorBase::Temperature)) {
        if (data.temperatureValid) {
            if (temperature.temperature() != (qreal)data.temperature) {
                temperature.setTemperature((qreal)data.temperature);
                temperature.setTimestamp((quint64)data.timestamp);
                emit q->temperatureChanged(temperature);
            }
        }
    }

    if (what.testFlag(SenseHatSensorBase::Gyro)) {
        if (data.gyroValid) {
            gyro.setTimestamp((quint64)data.timestamp);
            gyro.setX((qreal)data.gyro.x() * RADIANS_TO_DEGREES);
            gyro.setY((qreal)data.gyro.y() * RADIANS_TO_DEGREES);
            gyro.setZ((qreal)data.gyro.z() * RADIANS_TO_DEGREES);

            emit q->gyroChanged(gyro);
        }
    }

    if (what.testFlag(SenseHatSensorBase::Acceleration)) {
        if (data.accelValid) {
            acceleration.setTimestamp((quint64)data.timestamp);
            acceleration.setX((qreal)data.accel.x() * STANDARD_GRAVITY);
            acceleration.setY((qreal)data.accel.y() * STANDARD_GRAVITY);
            acceleration.setZ((qreal)data.accel.z() * STANDARD_GRAVITY);
            emit q->accelerationChanged(acceleration);
        }
    }
    if (what.testFlag(SenseHatSensorBase::Rotation)) {
        if (data.fusionPoseValid) {
            rotation.setTimestamp((quint64)data.timestamp);
            rotation.setFromEuler(toDeg360(data.fusionPose.x()),
                                   toDeg360(data.fusionPose.y()),
                                   toDeg360(data.fusionPose.z()));
            emit q->rotationChanged(rotation);
        }
    }
    // in rtimulib compass is magnetometer and azimuth comes from
    // sensorfusion algo
    if (what.testFlag(SenseHatSensorBase::Compass)) {
        if (data.fusionPoseValid) {
            qWarning() << "XXXX";
            qWarning() << Q_FUNC_INFO <<"measured"<< toDeg360(rtimu->getMeasuredPose().x());
            qWarning() << Q_FUNC_INFO <<"measured"<< toDeg360(rtimu->getMeasuredPose().y());
            qWarning() << Q_FUNC_INFO <<"measured"<< toDeg360(rtimu->getMeasuredPose().z());
            qWarning() << Q_FUNC_INFO <<"fusion"<< toDeg360(data.fusionPose.x());
            qWarning() << Q_FUNC_INFO <<"fusion"<<  toDeg360(data.fusionPose.y());
            qWarning() << Q_FUNC_INFO <<"fusion"<<  toDeg360(data.fusionPose.z());

            compass.setTimestamp((quint64)data.timestamp);
            compass.setAzimuth(toDeg360(data.fusionPose.z()));
                    emit q->compassChanged(compass);
        }
    }

    if (what.testFlag(SenseHatSensorBase::Magnetometer)) {
        if (data.compassValid) {
            mag.setTimestamp((qreal)data.timestamp);
            mag.setX((qreal)data.compass.x() * .000001);
            mag.setY((qreal)data.compass.y() * .000001);
            mag.setZ((qreal)data.compass.z() * .000001);
            emit q->magnetometerChanged(mag);
        }

    }
}

SenseHatSensorBase::SenseHatSensorBase(QSensor *sensor)
    : QSensorBackend(sensor),
      d_ptr(new QSenseHatSensorsPrivate(this))
{
    qDebug() << Q_FUNC_INFO;
    if (d_ptr->open()) {
        d_ptr->pollTimer.setInterval(d_ptr->pollInterval);
        connect(&d_ptr->pollTimer, &QTimer::timeout, [this] { d_ptr->update(sensorFlag); });
    }
}

SenseHatSensorBase::~SenseHatSensorBase()
{
}

void SenseHatSensorBase::start()
{
    qDebug() << Q_FUNC_INFO;
    if (d_ptr->imuInited)
        d_ptr->pollTimer.start();
    else {
        sensorError(-ENODEV);
        stop();
    }
}

void SenseHatSensorBase::stop()
{
    qDebug() << Q_FUNC_INFO;
    if (d_ptr->imuInited)
        d_ptr->pollTimer.stop();
    sensorStopped();
}

bool SenseHatSensorBase::isFeatureSupported(QSensor::Feature /*feature*/) const
{
    return false;
}

void SenseHatSensorBase::poll(SenseHatSensorBase::UpdateFlags sensorFlag)
{
    d_ptr->update(sensorFlag);
}
