#include "sensehatcompass.h"
#include "sensehatsensorbase.h"
#include <QCompassReading>
#include <QDebug>

char const * const SenseHatCompass::id("sensehat.compass");

SenseHatCompass::SenseHatCompass(QSensor *sensor)
    : SenseHatSensorBase(sensor)

{
    sensorFlag = SenseHatSensorBase::Compass;
    setReading<QCompassReading>(&m_reading);
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(compassChanged(QCompassReading)),this,SLOT(compassChanged(QCompassReading)));
}


void SenseHatCompass::compassChanged(const QCompassReading &value)
{
    m_reading.setAzimuth(value.azimuth());
    m_reading.setTimestamp(value.timestamp());
    newReadingAvailable();
}
