TARGET = qtsensors_sensehat
QT = core sensors-private

CONFIG += c++11

SOURCES += main.cpp \
    sensehatsensorbase.cpp \
    sensehatcompass.cpp \
    sensehataccelerometer.cpp \
    sensehatgyroscope.cpp \
    sensehatpressuresensor.cpp \
    sensehattemperaturesensor.cpp \
    sensehatmagnetometer.cpp \
    sensehatrotationsensor.cpp \
    sensehathumiditysensor.cpp

HEADERS += \
    sensehatsensorbase.h \
    qsensors-sensehatpluginprivate_p.h \
    sensehatcompass.h \
    sensehataccelerometer.h \
    sensehatgyroscope.h \
    sensehatpressuresensor.h \
    sensehattemperaturesensor.h \
    sensehatmagnetometer.h \
    sensehatrotationsensor.h \
    sensehathumiditysensor.h

OTHER_FILES = plugin.json \
    Sensors.conf

LIBS += -lRTIMULib

CONFIGFILES.files = Sensors.conf
CONFIGFILES.path = /etc/xdg/QtProject/
INSTALLS += CONFIGFILES

PLUGIN_TYPE = sensors
PLUGIN_CLASS_NAME = sensehatSensorPlugin
load(qt_plugin)
