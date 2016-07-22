#ifndef SENSEHATPRESSURESENSOR_H
#define SENSEHATPRESSURESENSOR_H

#include <QObject>
#include "sensehatsensorbase.h"
#include <QPressureReading>

class SenseHatPressureSensor : public SenseHatSensorBase
{
        Q_OBJECT
public:
    SenseHatPressureSensor(QSensor *sensor);
    static char const * const id;
private slots:
    void pressureChanged(const QPressureReading &value);
private:
    QPressureReading m_reading;

};

#endif // SENSEHATPRESSURESENSOR_H
