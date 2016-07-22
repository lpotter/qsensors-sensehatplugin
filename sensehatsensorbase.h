#ifndef SENSEHATSENSORBASE_H
#define SENSEHATSENSORBASE_H

#include <QObject>
#include <QSensorBackend>
#include <QCompassReading>
#include <QGyroscopeReading>
#include <QAccelerometerReading>
#include <QOrientationReading>
#include <QMagnetometerReading>
#include <QRotationReading>
#include <QAmbientTemperatureReading>
#include <QPressureReading>

#include <QLoggingCategory>

#include <RTIMULib.h>
#include <QTimer>
//Q_DECLARE_LOGGING_CATEGORY(qSenseHat)

class QSenseHatSensorsPrivate;
class SenseHatSensorBase : public QSensorBackend
{
    Q_OBJECT
public:
    enum UpdateFlag {
        Humidity = 0x01,
        Pressure = 0x02,
        Temperature = 0x04,
        Gyro = 0x08,
        Acceleration = 0x10,
        Compass = 0x20,
        Orientation = 0x40,
        Magnetometer = 0x80,
        Rotation = 0x160,
        All = 0xFF
    };
    Q_DECLARE_FLAGS(UpdateFlags, UpdateFlag)

    SenseHatSensorBase(QSensor *sensor);
    ~SenseHatSensorBase();

    QSenseHatSensorsPrivate *d_ptr;
    SenseHatSensorBase::UpdateFlags sensorFlag;
    void start() Q_DECL_OVERRIDE;
    void stop() Q_DECL_OVERRIDE;
    void poll(SenseHatSensorBase::UpdateFlags sensorFlag = All);
    bool isFeatureSupported(QSensor::Feature feature) const Q_DECL_OVERRIDE;

signals:
    void humidityChanged(qreal value);
    void pressureChanged(const QPressureReading &value);
    void temperatureChanged(const QAmbientTemperatureReading &value);
    void gyroChanged(const QGyroscopeReading &value);
    void accelerationChanged(const QAccelerometerReading &value);
    void compassChanged(const QCompassReading &value);
    void orientationChanged(const QOrientationReading &value);//
    void magnetometerChanged(const QMagnetometerReading &value);
    void rotationChanged(const QRotationReading &value);//
private:
};

#endif // SENSEHATSENSORBASE_H
