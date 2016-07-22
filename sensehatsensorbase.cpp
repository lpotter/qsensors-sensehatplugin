#include "sensehatsensorbase.h"
#include "qsensors-sensehatpluginprivate_p.h"

#include <QStandardPaths>
#include <QSensorBackend>

#include <RTIMULib.h>
#include <unistd.h>
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

void QSenseHatSensorsPrivate::open()
{
    qDebug() << Q_FUNC_INFO;

 //   CLocale c; // to avoid decimal separator trouble in the ini file
    const QString configFileName = QStringLiteral("RTIMULib.ini");
    const QString defaultConfig = QStringLiteral("/etc/") + configFileName;
    const QString writableConfigDir = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation)
            + QStringLiteral("/sense_hat");
    const QString writableConfig = writableConfigDir + QStringLiteral("/") + configFileName;

//    if (!flags.testFlag(QSenseHatSensors::DontCopyIniFile)) {
//        if (!QFile::exists(writableConfig)) {
//            qCDebug(qSenseHat) << "Copying" << defaultConfig << "to" << writableConfig;
//            if (QFile::exists(defaultConfig)) {
//                QDir(QStringLiteral("/")).mkpath(writableConfigDir);
//                QFile::copy(defaultConfig, writableConfig);
//            } else {
//                qWarning("/etc/RTIMULib.ini not found, sensors may not be functional");
//            }
//        }
//        QByteArray dirName = writableConfigDir.toUtf8();
//        settings = new RTIMUSettings(dirName.constData(), "RTIMULib");
//    } else {
        settings = new RTIMUSettings("/etc", "RTIMULib");
   // }

    rtimu = RTIMU::createIMU(settings);
    pollInterval = qMax(1, rtimu->IMUGetPollInterval());
    qDebug() << "IMU name" <<   rtimu->IMUName() << "Recommended poll interval" << pollInterval << "ms";

    rthumidity = RTHumidity::createHumidity(settings);
    qDebug() << "Humidity sensor name" << rthumidity->humidityName();

    rtpressure = RTPressure::createPressure(settings);
   qDebug() <<  "Pressure sensor name" << rtpressure->pressureName();
}

void QSenseHatSensorsPrivate::update(SenseHatSensorBase::UpdateFlags what)
{
//    qDebug() << Q_FUNC_INFO << what;

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
        if (!imuInited) {
            imuInited = true;
            if (!rtimu->IMUInit())
                qWarning("Failed to initialize IMU");
        }
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
    return deg < 0 ? deg + 360 : deg;
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
            gyro.setX((qreal)data.gyro.x());
            gyro.setY((qreal)data.gyro.y());
            gyro.setZ((qreal)data.gyro.z());

            emit q->gyroChanged(gyro);
        }
    }

    if (what.testFlag(SenseHatSensorBase::Acceleration)) {
        if (data.accelValid) {
            acceleration.setTimestamp((quint64)data.timestamp);
            acceleration.setX((qreal)data.accel.x());
            acceleration.setY((qreal)data.accel.y());
            acceleration.setZ((qreal)data.accel.z());
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
            compass.setTimestamp((quint64)data.timestamp);
            compass.setAzimuth(toDeg360(data.fusionPose.z()));
                    emit q->compassChanged(compass);
            qDebug() << Q_FUNC_INFO << compass.azimuth();
        }
    }

    if (what.testFlag(SenseHatSensorBase::Magnetometer)) {
        if (data.compassValid) {
            mag.setTimestamp((qreal)data.timestamp);
            mag.setX((qreal)data.compass.x());
            mag.setY((qreal)data.compass.y());
            mag.setZ((qreal)data.compass.z());
            emit q->magnetometerChanged(mag);
        }

    }
}

SenseHatSensorBase::SenseHatSensorBase(QSensor *sensor)
    : QSensorBackend(sensor),
      d_ptr(new QSenseHatSensorsPrivate(this))
{
    qDebug() << Q_FUNC_INFO;
    d_ptr->open();
    d_ptr->pollTimer.setInterval(d_ptr->pollInterval);
    connect(&d_ptr->pollTimer, &QTimer::timeout, [this] { d_ptr->update(sensorFlag); });
}

SenseHatSensorBase::~SenseHatSensorBase()
{
}

void SenseHatSensorBase::start()
{
    qDebug() << Q_FUNC_INFO;
    d_ptr->pollTimer.start();
}

void SenseHatSensorBase::stop()
{
    qDebug() << Q_FUNC_INFO;
    d_ptr->pollTimer.stop();
}

bool SenseHatSensorBase::isFeatureSupported(QSensor::Feature /*feature*/) const
{
    return false;
}

void SenseHatSensorBase::poll(SenseHatSensorBase::UpdateFlags sensorFlag)
{
    //Q_D(QSenseHatSensors);
    d_ptr->update(sensorFlag);
}
