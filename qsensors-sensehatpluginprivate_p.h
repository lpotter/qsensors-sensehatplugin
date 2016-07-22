#ifndef QSENSORSSENSEHATPLUGINPRIVATE_P_H
#define QSENSORSSENSEHATPLUGINPRIVATE_P_H

#include "sensehatsensorbase.h"
#include <QCompassReading>
#include <QGyroscopeReading>
#include <QAccelerometerReading>
#include <QOrientationReading>

class QSenseHatSensorsPrivate
{
public:
    QSenseHatSensorsPrivate(SenseHatSensorBase *q_ptr);
    ~QSenseHatSensorsPrivate();
    SenseHatSensorBase *q;

    RTIMUSettings *settings = Q_NULLPTR;
    RTIMU *rtimu = Q_NULLPTR;
    RTHumidity *rthumidity = Q_NULLPTR;
    RTPressure *rtpressure = Q_NULLPTR;

    bool imuInited;
    int pollInterval;
    bool humidityInited;
    bool pressureInited;
    QTimer pollTimer;
    bool temperatureFromHumidity;


    qreal humidity = 0;
    qreal pressure = 0;
    qreal temperature = 0;
    QGyroscopeReading gyro;
    QAccelerometerReading acceleration;
    QCompassReading compass;
    QOrientationReading orientation;

    void open();
    void update(SenseHatSensorBase::UpdateFlags what);
    void report(const RTIMU_DATA &data, SenseHatSensorBase::UpdateFlags what);

};



#endif // QSENSORSSENSEHATPLUGINPRIVATE_P_H

