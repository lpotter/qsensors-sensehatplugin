#ifndef SENSEHATSENSORBASE_H
#define SENSEHATSENSORBASE_H

#include <QObject>
#include <QSensorBackend>
#include <QCompassReading>
#include <QGyroscopeReading>
#include <QAccelerometerReading>
#include <QOrientationReading>

#include <RTIMULib.h>
#include <QTimer>

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

signals:
    void humidityChanged(qreal value);
    void pressureChanged(qreal value);
    void temperatureChanged(qreal value);
    void gyroChanged(const QGyroscopeReading &value);
    void accelerationChanged(const QAccelerometerReading &value);
    void compassChanged(const QCompassReading &value);
    void orientationChanged(const QOrientationReading &value);
private:
};

#endif // SENSEHATSENSORBASE_H
