/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Canonical Ltd
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "sensehatsensorbase.h"
#include "qsensors-sensehatpluginprivate_p.h"

#include <QFile>
#include <QStandardPaths>
#include <QSensorBackend>
//#include <QDeadlineTimer>

#include <RTIMULib.h>
#include <unistd.h>
#include <errno.h>

#define RADIANS_TO_DEGREES 57.2957795
#define STANDARD_GRAVITY 9.80665
#define RADIANS_TO_DEGREES 57.2957795

QSenseHatSensorsPrivate::QSenseHatSensorsPrivate(SenseHatSensorBase *q_ptr)
    : q(q_ptr)
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
    QString sensehatConfigDir = QFile::decodeName(qgetenv("SENSEHAT_CONFIG_DIR"));
    if (sensehatConfigDir.isEmpty())
        sensehatConfigDir = QString::fromLatin1("/etc/xdg");

    settings = new RTIMUSettings(sensehatConfigDir.toLocal8Bit(), "RTIMULib");

    rtimu = RTIMU::createIMU(settings);
    if ((rtimu == NULL) || (rtimu->IMUType() == RTIMU_TYPE_NULL)) {
        qWarning() << "No IMU found";
        return false;
    }
    rtimu->setGyroEnable(true);
    rtimu->setAccelEnable(true);
    rtimu->setCompassEnable(true);

    pollInterval = qMax(1, rtimu->IMUGetPollInterval());

    rthumidity = RTHumidity::createHumidity(settings);
    if (rthumidity == NULL) {
        qWarning() << "Failed to create Humidity";
    }
    rtpressure = RTPressure::createPressure(settings);
    if (rtpressure == NULL) {
        qWarning() << "Failed to create Pressure";
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
        if (rtimu->IMURead())
            report(rtimu->getIMUData(), what & imuFlags);
    }
}

static inline qreal toDeg360(qreal rad)
{
    const qreal deg = rad * RADIANS_TO_DEGREES;
    qreal result = deg < 0 ? deg + 360 : deg;
    return result;
}

void QSenseHatSensorsPrivate::report(const RTIMU_DATA &data, SenseHatSensorBase::UpdateFlags what)
{
    switch (what) {
    case SenseHatSensorBase::Humidity:
        if (data.humidityValid) {
            if (humidity.relativeHumidity() != data.humidity) {
                humidity.setTimestamp(getTimestamp());
                humidity.setRelativeHumidity(data.humidity);
                emit q->humidityChanged(humidity);
            }
        }
        break;
    case SenseHatSensorBase::Pressure:
        if (data.pressureValid) {
            if (pressure.pressure() != (qreal)data.pressure) {
                pressure.setTimestamp(getTimestamp());
                pressure.setPressure((qreal)data.pressure);
                emit q->pressureChanged(pressure);
            }
        }
        break;
    case SenseHatSensorBase::Temperature:
        if (data.temperatureValid) {
            if (temperature.temperature() != (qreal)data.temperature) {
                temperature.setTemperature((qreal)data.temperature);
                temperature.setTimestamp(getTimestamp());
                emit q->temperatureChanged(temperature);
            }
        }
        break;
    case SenseHatSensorBase::Gyro:
        if (data.gyroValid) {
            gyro.setTimestamp((quint64)data.timestamp);
            gyro.setX((qreal)data.gyro.x() * RADIANS_TO_DEGREES);
            gyro.setY((qreal)data.gyro.y() * RADIANS_TO_DEGREES);
            gyro.setZ((qreal)data.gyro.z() * RADIANS_TO_DEGREES);

            emit q->gyroChanged(gyro);
        }
        break;
    case SenseHatSensorBase::Acceleration:
        if (data.accelValid) {
            acceleration.setTimestamp((quint64)data.timestamp);
            acceleration.setX((qreal)data.accel.x() * STANDARD_GRAVITY);
            acceleration.setY((qreal)data.accel.y() * STANDARD_GRAVITY);
            acceleration.setZ((qreal)data.accel.z() * STANDARD_GRAVITY);
            emit q->accelerationChanged(acceleration);
        }
        break;
    case SenseHatSensorBase::Rotation:
        if (data.fusionPoseValid) {
            rotation.setTimestamp((quint64)data.timestamp);
            rotation.setFromEuler(toDeg360(data.fusionPose.x()),
                                  toDeg360(data.fusionPose.y()),
                                  toDeg360(data.fusionPose.z()));
            emit q->rotationChanged(rotation);
        }
        break;
    // in rtimulib compass is magnetometer and azimuth comes from
    // sensorfusion algo
    case SenseHatSensorBase::Compass:
        if (data.fusionPoseValid) {
            compass.setTimestamp((quint64)data.timestamp);
            compass.setAzimuth(toDeg360(data.fusionPose.z()));
                    emit q->compassChanged(compass);
        }
        break;
    case SenseHatSensorBase::Magnetometer:
        if (data.compassValid) {
            mag.setTimestamp((qreal)data.timestamp);
            mag.setX((qreal)data.compass.x() * .000001);
            mag.setY((qreal)data.compass.y() * .000001);
            mag.setZ((qreal)data.compass.z() * .000001);
            emit q->magnetometerChanged(mag);
        }
        break;
    };
}

quint64 QSenseHatSensorsPrivate::getTimestamp()
{
    struct timespec tv;
    int ok;
    ok = clock_gettime(CLOCK_MONOTONIC, &tv);
    Q_ASSERT(ok == 0);
    Q_UNUSED(ok);

    quint64 result = (tv.tv_sec * 1000000ULL) + (tv.tv_nsec * 0.001); // scale to microseconds
    return result;
}

SenseHatSensorBase::SenseHatSensorBase(QSensor *sensor)
    : QSensorBackend(sensor),
      d_ptr(new QSenseHatSensorsPrivate(this))
{
    if (d_ptr->open()) {
        d_ptr->pollTimer.setInterval(d_ptr->pollInterval);
        connect(&d_ptr->pollTimer, &QTimer::timeout,
                [this] { d_ptr->update(sensorFlag); });
    }
}

SenseHatSensorBase::~SenseHatSensorBase()
{
}

void SenseHatSensorBase::start()
{
    if (d_ptr->imuInited)
        d_ptr->pollTimer.start();
    else {
        sensorError(-ENODEV);
        stop();
    }
}

void SenseHatSensorBase::stop()
{
    if (d_ptr->imuInited)
        d_ptr->pollTimer.stop();
    sensorStopped();
}

bool SenseHatSensorBase::isFeatureSupported(QSensor::Feature) const
{
    return false;
}

void SenseHatSensorBase::poll(SenseHatSensorBase::UpdateFlags sensorFlag)
{
    d_ptr->update(sensorFlag);
}
