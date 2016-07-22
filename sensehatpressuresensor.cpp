#include "sensehatpressuresensor.h"
#include "sensehatsensorbase.h"
#include <QDebug>

char const * const SenseHatPressureSensor::id("sensehat.pressure");


SenseHatPressureSensor::SenseHatPressureSensor(QSensor *sensor)
    : SenseHatSensorBase(sensor)
{
    sensorFlag = SenseHatSensorBase::Pressure;
    setReading<QPressureReading>(&m_reading);
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(pressureChanged(QPressureReading)),this,SLOT(pressureChanged(QPressureReading)));
}


void SenseHatPressureSensor::pressureChanged(const QPressureReading &value)
{
    m_reading.setPressure(value.pressure());
  //  m_reading.setTemperature(value.y());
    m_reading.setTimestamp(value.timestamp());
    newReadingAvailable();
}
