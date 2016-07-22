#include "sensehatmagnetometer.h"
#include "sensehatsensorbase.h"
#include <QDebug>

char const * const SenseHatMagnetometer::id("sensehat.magnetometer");

SenseHatMagnetometer::SenseHatMagnetometer(QSensor *sensor)
    : SenseHatSensorBase(sensor)
{
    sensorFlag = SenseHatSensorBase::Magnetometer;
    setReading<QMagnetometerReading>(&m_reading);
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(magnetometerChanged(QCompassReading)),this,SLOT(magChanged(QCompassReading)));
}


void SenseHatMagnetometer::magChanged(const QMagnetometerReading &value)
{
    m_reading.setX(value.x());
    m_reading.setY(value.y());
    m_reading.setZ(value.z());
    m_reading.setTimestamp(value.timestamp());
    newReadingAvailable();
}
