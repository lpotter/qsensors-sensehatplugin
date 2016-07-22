#ifndef SENSEHATCOMPASS_H
#define SENSEHATCOMPASS_H

#include <QObject>
#include "sensehatsensorbase.h"
#include <QCompass>

class SenseHatCompass : public SenseHatSensorBase
{
    Q_OBJECT
public:
    SenseHatCompass(QSensor *sensor);
    static char const * const id;

private slots:
    void compassChanged(const QCompassReading &value);
private:
    QCompassReading m_reading;

};

#endif // SENSEHATCOMPASS_H
