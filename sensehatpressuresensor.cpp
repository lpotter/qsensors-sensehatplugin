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


#include "sensehatpressuresensor.h"
#include "sensehatsensorbase.h"
#include <QDebug>

char const * const SenseHatPressureSensor::id("sensehat.pressure");


SenseHatPressureSensor::SenseHatPressureSensor(QSensor *sensor)
    : SenseHatSensorBase(sensor)
{
    sensorFlag = SenseHatSensorBase::Pressure;
    setReading<QPressureReading>(&m_reading);
    qDebug() << Q_FUNC_INFO;
    connect(this,SIGNAL(pressureChanged(QPressureReading)),this,SLOT(pressureChanged(QPressureReading)));
}


void SenseHatPressureSensor::pressureChanged(const QPressureReading &value)
{
    m_reading.setPressure(value.pressure());
  //  m_reading.setTemperature(value.y());
    m_reading.setTimestamp(value.timestamp());
    newReadingAvailable();
}
