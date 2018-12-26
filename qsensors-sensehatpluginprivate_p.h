/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Canonical Ltd
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

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
#include <QHumidityReading>

class QSenseHatSensorsPrivate
{
public:
    QSenseHatSensorsPrivate(SenseHatSensorBase *q_ptr);
    ~QSenseHatSensorsPrivate();
    SenseHatSensorBase *q;

    RTIMUSettings *settings = nullptr;
    RTIMU *rtimu = nullptr;
    RTHumidity *rthumidity = nullptr;
    RTPressure *rtpressure = nullptr;

    bool imuInited = false;
    int pollInterval = 0;
    bool humidityInited = false;
    bool pressureInited = false;
    QTimer pollTimer;
    bool temperatureFromHumidity =  true;

    qreal m_humidity = 0;
    QPressureReading pressure;
    QAmbientTemperatureReading temperature;
    QGyroscopeReading gyro;
    QAccelerometerReading acceleration;
    QCompassReading compass;
    QOrientationReading orientation;
    QMagnetometerReading mag;
    QRotationReading rotation;
    QAmbientTemperatureReading temp;
    QHumidityReading humidity;

    bool open();
    void update(SenseHatSensorBase::UpdateFlags what);
    void report(const RTIMU_DATA &data, SenseHatSensorBase::UpdateFlags what);
    quint64 getTimestamp();
};

#endif // QSENSORSSENSEHATPLUGINPRIVATE_P_H

