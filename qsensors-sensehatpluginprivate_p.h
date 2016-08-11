/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Canonical Ltd
** Contact: http://www.qt.io/licensing/
**
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

// parts from qsensehat module

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

    bool open();
    void update(SenseHatSensorBase::UpdateFlags what);
    void report(const RTIMU_DATA &data, SenseHatSensorBase::UpdateFlags what);

};



#endif // QSENSORSSENSEHATPLUGINPRIVATE_P_H

