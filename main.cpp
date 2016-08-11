/****************************************************************************
**
** Copyright (C) 2016 Canonical Ltd
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


#include "sensehatcompass.h"
#include "sensehataccelerometer.h"
#include "sensehatgyroscope.h"
#include "sensehatpressuresensor.h"
#include "sensehattemperaturesensor.h"
#include "sensehatmagnetometer.h"
#include "sensehatrotationsensor.h"

#include <qsensorplugin.h>
#include <qsensorbackend.h>
#include <qsensormanager.h>

#include <QtCore/QFile>
#include <QtCore/QDebug>

class QSenseHatSensorPlugin : public QObject, public QSensorPluginInterface, public QSensorBackendFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.qt-project.Qt.QSensorPluginInterface/1.0" FILE "plugin.json")
    Q_INTERFACES(QSensorPluginInterface)

public:
    void registerSensors()
    {
        if (!QSensorManager::isBackendRegistered(QCompass::type, SenseHatCompass::id))
            QSensorManager::registerBackend(QCompass::type, SenseHatCompass::id, this);

        if (!QSensorManager::isBackendRegistered(QAccelerometer::type, SenseHatAccelerometer::id))
            QSensorManager::registerBackend(QAccelerometer::type, SenseHatAccelerometer::id, this);

        if (!QSensorManager::isBackendRegistered(QGyroscope::type, SenseHatGyroscope::id))
            QSensorManager::registerBackend(QGyroscope::type, SenseHatGyroscope::id, this);

        if (!QSensorManager::isBackendRegistered(QPressureSensor::type, SenseHatPressureSensor::id))
            QSensorManager::registerBackend(QPressureSensor::type, SenseHatPressureSensor::id, this);

        if (!QSensorManager::isBackendRegistered(QAmbientTemperatureSensor::type, SenseHatTemperatureSensor::id))
            QSensorManager::registerBackend(QAmbientTemperatureSensor::type, SenseHatTemperatureSensor::id, this);

        if (!QSensorManager::isBackendRegistered(QMagnetometer::type, SenseHatMagnetometer::id))
            QSensorManager::registerBackend(QMagnetometer::type, SenseHatMagnetometer::id, this);

        if (!QSensorManager::isBackendRegistered(QRotationSensor::type, SenseHatRotationSensor::id))
            QSensorManager::registerBackend(QRotationSensor::type, SenseHatRotationSensor::id, this);

    }

    QSensorBackend *createBackend(QSensor *sensor)
    {

        if (sensor->identifier() == SenseHatCompass::id)
            return new SenseHatCompass(sensor);

        if (sensor->identifier() == SenseHatAccelerometer::id)
            return new SenseHatAccelerometer(sensor);

        if (sensor->identifier() == SenseHatGyroscope::id)
            return new SenseHatGyroscope(sensor);

        if (sensor->identifier() == SenseHatPressureSensor::id)
            return new SenseHatPressureSensor(sensor);

        if (sensor->identifier() == SenseHatTemperatureSensor::id)
            return new SenseHatTemperatureSensor(sensor);

        if (sensor->identifier() == SenseHatMagnetometer::id)
            return new SenseHatMagnetometer(sensor);

        if (sensor->identifier() == SenseHatRotationSensor::id)
            return new SenseHatRotationSensor(sensor);

        return Q_NULLPTR;
    }
};

#include "main.moc"

