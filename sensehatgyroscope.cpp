#include "sensehatgyroscope.h"
#include "sensehatsensorbase.h"


char const * const SenseHatGyroscope::id("sensehat.gyroscope");

SenseHatGyroscope::SenseHatGyroscope(QSensor *sensor)
    : SenseHatSensorBase(sensor)

{
    sensorFlag = SenseHatSensorBase::Gyro;
    setReading<QGyroscopeReading>(&m_reading);
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(compassChanged(QGyroscopeReading)),this,SLOT(compassChanged(QGyroscopeReading)));
}


void SenseHatGyroscope::gyroChanged(const QGyroscopeReading &value)
{
    m_reading.setX(value.x());
    m_reading.setY(value.y());
    m_reading.setZ(value.z());
    m_reading.setTimestamp(value.timestamp());
    newReadingAvailable();
}
