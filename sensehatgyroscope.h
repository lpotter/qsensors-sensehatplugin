#ifndef SENSEHATGYROSCOPE_H
#define SENSEHATGYROSCOPE_H
#include <QObject>
#include "sensehatsensorbase.h"

#include <QGyroscope>

class SenseHatGyroscope : public SenseHatSensorBase
{
    Q_OBJECT
public:
    SenseHatGyroscope(QSensor *sensor);
    static char const * const id;

private slots:
    void gyroChanged(const QGyroscopeReading &value);
private:
    QGyroscopeReading m_reading;

};

#endif // SENSEHATGYROSCOPE_H
