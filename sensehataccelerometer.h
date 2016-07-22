#ifndef SENSEHATACCELEROMETER_H
#define SENSEHATACCELEROMETER_H

#include <QObject>
#include "sensehatsensorbase.h"
#include <QAccelerometerReading>

class SenseHatAccelerometer : public SenseHatSensorBase
{
    Q_OBJECT
public:
    SenseHatAccelerometer(QSensor *sensor);
    static char const * const id;
private slots:
    void accelChanged(const QAccelerometerReading &value);
private:
    QAccelerometerReading m_reading;

};

#endif // SENSEHATACCELEROMETER_H
