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

#ifndef SENSEHATSENSORBASE_H
#define SENSEHATSENSORBASE_H

#include <RTIMULib.h>

#include <QObject>
#include <QSensorBackend>
#include <QCompassReading>
#include <QGyroscopeReading>
#include <QAccelerometerReading>
#include <QOrientationReading>
#include <QMagnetometerReading>
#include <QRotationReading>
#include <QAmbientTemperatureReading>
#include <QPressureReading>
#include <QHumidityReading>
#include <QTimer>

#include <QLoggingCategory>

class QSenseHatSensorsPrivate;
class SenseHatSensorBase : public QSensorBackend
{
    Q_OBJECT
public:
    enum UpdateFlag {
        Humidity = 0x01,
        Pressure = 0x02,
        Temperature = 0x04,
        Gyro = 0x08,
        Acceleration = 0x10,
        Compass = 0x20,
        Orientation = 0x40,
        Magnetometer = 0x80,
        Rotation = 0x160,
        All = 0xFF
    };
    Q_DECLARE_FLAGS(UpdateFlags, UpdateFlag)

    SenseHatSensorBase(QSensor *sensor);
    ~SenseHatSensorBase();

    QSenseHatSensorsPrivate *d_ptr;
    SenseHatSensorBase::UpdateFlags sensorFlag;
    void start() Q_DECL_OVERRIDE;
    void stop() Q_DECL_OVERRIDE;
    void poll(SenseHatSensorBase::UpdateFlags sensorFlag = All);
    bool isFeatureSupported(QSensor::Feature feature) const Q_DECL_OVERRIDE;

signals:
    void humidityChanged(const QHumidityReading &value);
    void pressureChanged(const QPressureReading &value);
    void temperatureChanged(const QAmbientTemperatureReading &value);
    void gyroChanged(const QGyroscopeReading &value);
    void accelerationChanged(const QAccelerometerReading &value);
    void compassChanged(const QCompassReading &value);
    void orientationChanged(const QOrientationReading &value);
    void magnetometerChanged(const QMagnetometerReading &value);
    void rotationChanged(const QRotationReading &value);
};

#endif // SENSEHATSENSORBASE_H
