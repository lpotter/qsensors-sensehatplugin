#ifndef QSENSORSSENSEHATPLUGINPRIVATE_P_H
#define QSENSORSSENSEHATPLUGINPRIVATE_P_H

#include "sensehatsensorbase.h"
#include <QCompassReading>
#include <QGyroscopeReading>
#include <QAccelerometerReading>
#include <QOrientationReading>
#include <QMagnetometerReading>
#include <QRotationReading>
#include <QAmbientTemperatureReading>

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
    QPressureReading pressure;
    QAmbientTemperatureReading temperature;
    QGyroscopeReading gyro;
    QAccelerometerReading acceleration;
    QCompassReading compass;
    QOrientationReading orientation;
    QMagnetometerReading mag;
    QRotationReading rotation;
    QAmbientTemperatureReading temp;

    void open();
    void update(SenseHatSensorBase::UpdateFlags what);
    void report(const RTIMU_DATA &data, SenseHatSensorBase::UpdateFlags what);

};



#endif // QSENSORSSENSEHATPLUGINPRIVATE_P_H

