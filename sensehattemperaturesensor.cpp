#include "sensehattemperaturesensor.h"
#include "sensehatsensorbase.h"
#include <QDebug>

char const * const SenseHatTemperatureSensor::id("sensehat.temperature");

SenseHatTemperatureSensor::SenseHatTemperatureSensor(QSensor *sensor)
    : SenseHatSensorBase(sensor)
{
    sensorFlag = SenseHatSensorBase::Temperature;
    setReading<QAmbientTemperatureReading>(&m_reading);
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(temperatureChanged(QAmbientTemperatureReading)),this,SLOT(temperatureChanged(QAmbientTemperatureReading)));
}


void SenseHatTemperatureSensor::temperatureChanged(const QAmbientTemperatureReading &value)
{
    m_reading.setTemperature(value.temperature());

    m_reading.setTimestamp(value.timestamp());
    newReadingAvailable();
}
