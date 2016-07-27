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
