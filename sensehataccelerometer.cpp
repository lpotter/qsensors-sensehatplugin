#include "sensehataccelerometer.h"
#include "sensehatsensorbase.h"
#include <QDebug>

char const * const SenseHatAccelerometer::id("sensehat.accelerometer");

SenseHatAccelerometer::SenseHatAccelerometer(QSensor *sensor)
    : SenseHatSensorBase(sensor)
{
    sensorFlag = SenseHatSensorBase::Acceleration;
    setReading<QAccelerometerReading>(&m_reading);
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(accelerationChanged(QAccelerometerReading)),this,SLOT(accelChanged(QAccelerometerReading)));
}


void SenseHatAccelerometer::accelChanged(const QAccelerometerReading &value)
{
    m_reading.setX(value.x());
    m_reading.setY(value.y());
    m_reading.setZ(value.z());
    m_reading.setTimestamp(value.timestamp());
    newReadingAvailable();
}
