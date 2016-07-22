#ifndef SENSEHATTEMPERATURESENSOR_H
#define SENSEHATTEMPERATURESENSOR_H

#include <QObject>
#include <QAmbientTemperatureReading>
#include "sensehatsensorbase.h"

class SenseHatTemperatureSensor : public SenseHatSensorBase
{
    Q_OBJECT
public:
    SenseHatTemperatureSensor(QSensor *sensor);
    static char const * const id;
private slots:
    void temperatureChanged(const QAmbientTemperatureReading &value);
private:
    QAmbientTemperatureReading m_reading;
};

#endif // SENSEHATTEMPERATURESENSOR_H
