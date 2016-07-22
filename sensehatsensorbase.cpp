#include "sensehatsensorbase.h"
#include "qsensors-sensehatpluginprivate_p.h"

#include <QStandardPaths>
#include <QSensorBackend>

#include <RTIMULib.h>
#include <unistd.h>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(qSenseHat)
static const int MAX_READ_ATTEMPTS = 5;

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
    qCDebug(qSenseHat, "IMU name %s Recommended poll interval %d ms", rtimu->IMUName(), pollInterval);

    rthumidity = RTHumidity::createHumidity(settings);
    qCDebug(qSenseHat, "Humidity sensor name %s", rthumidity->humidityName());

    rtpressure = RTPressure::createPressure(settings);
    qCDebug(qSenseHat, "Pressure sensor name %s", rtpressure->pressureName());
}

void QSenseHatSensorsPrivate::update(SenseHatSensorBase::UpdateFlags what)
{
    qDebug() << Q_FUNC_INFO << what;

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
            | SenseHatSensorBase::Compass | SenseHatSensorBase::Orientation;
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

void QSenseHatSensorsPrivate::report(const RTIMU_DATA &data, SenseHatSensorBase::UpdateFlags what)
{
    qDebug() << Q_FUNC_INFO << what;

    if (what.testFlag(SenseHatSensorBase::Humidity)) {
        if (data.humidityValid) {
            humidity = data.humidity;
            emit q->humidityChanged(humidity);
            q->newReadingAvailable();
        }
    }

    if (what.testFlag(SenseHatSensorBase::Pressure)) {
        if (data.pressureValid) {
            pressure = data.pressure;
            emit q->pressureChanged(pressure);
            q->newReadingAvailable();        }
    }

    if (what.testFlag(SenseHatSensorBase::Temperature)) {
        if (data.temperatureValid) {
            temperature = data.temperature;
            emit q->temperatureChanged(temperature);
            q->newReadingAvailable();
        }
    }

    if (what.testFlag(SenseHatSensorBase::Gyro)) {
        if (data.gyroValid) {
            gyro.setTimestamp((quint64)data.timestamp);
            gyro.setX((qreal)data.gyro.x());
            gyro.setY((qreal)data.gyro.y());
            gyro.setZ((qreal)data.gyro.z());

            emit q->gyroChanged(gyro);
            q->newReadingAvailable();
        }
    }

    if (what.testFlag(SenseHatSensorBase::Acceleration)) {
        if (data.accelValid) {
            //  acceleration = QAccelerometerReading(data.timestamp, data.accel.x(), data.accel.y(), data.accel.z());
            acceleration.setTimestamp((quint64)data.timestamp);
            acceleration.setX((qreal)data.accel.x());
            acceleration.setY((qreal)data.accel.y());
            acceleration.setZ((qreal)data.accel.z());
            emit q->accelerationChanged(acceleration);
            q->newReadingAvailable();
        }
    }

    if (what.testFlag(SenseHatSensorBase::Compass)) {
        if (data.compassValid) {
            //   compass = QCompassReading(data.timestamp,data.compass.x(), data.compass.y(), data.compass.z());
            compass.setTimestamp((quint64)data.timestamp);
            compass.setAzimuth((qreal)data.compass.x());
            //            compass.setY((qreal)data.compass.y());
            //            compass.setZ((qreal)data.compass.z());
            emit q->compassChanged(compass);
            q->newReadingAvailable();
            qDebug() << Q_FUNC_INFO << compass.azimuth();
        }
    }

    // QSensors orientation is not the same as android
    //    if (what.testFlag(SenseHatSensorBase::Orientation)) {
    //        if (data.fusionPoseValid) {
    //            orientation = QOrientationReading(data.timestamp, toDeg360(data.fusionPose.x()),  // roll
    //                                    toDeg360(data.fusionPose.y()),  // pitch
    //                                    toDeg360(data.fusionPose.z())); // yaw
    //            emit q->orientationChanged(orientation);
    //        }
    //    }
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

void SenseHatSensorBase::poll(SenseHatSensorBase::UpdateFlags sensorFlag)
{
    //Q_D(QSenseHatSensors);
    d_ptr->update(sensorFlag);
}
