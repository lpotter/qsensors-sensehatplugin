#ifndef SENSEHATMAGNETOMETER_H
#define SENSEHATMAGNETOMETER_H

#include <QObject>
#include "sensehatsensorbase.h"
#include <QMagnetometerReading>

class SenseHatMagnetometer : public SenseHatSensorBase
{
    Q_OBJECT
public:
    SenseHatMagnetometer(QSensor *sensor);
    static char const * const id;

private slots:
    void magChanged(const QMagnetometerReading &value);
private:
    QMagnetometerReading m_reading;

};

#endif // SENSEHATMAGNETOMETER_H
