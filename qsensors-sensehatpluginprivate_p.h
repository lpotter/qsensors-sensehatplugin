// Copyright (C) 2015, 2016 Canonical Ltd
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License version 3 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

