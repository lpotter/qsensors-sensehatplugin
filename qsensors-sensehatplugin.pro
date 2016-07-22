TEMPLATE = lib
TARGET = qtsensors_sensehat
QT = core sensors
CONFIG += plugin debug

OTHER_FILES = plugin.json \
    Sensors.conf
CONFIG += c++11

LIBS += -lRTIMULib

SOURCES += main.cpp \
    sensehatsensorbase.cpp \
    sensehatcompass.cpp \
    sensehataccelerometer.cpp \
    sensehatgyroscope.cpp \
    sensehatpressuresensor.cpp \
    sensehattemperaturesensor.cpp \
    sensehatmagnetometer.cpp

HEADERS += \
    sensehatsensorbase.h \
    qsensors-sensehatpluginprivate_p.h \
    sensehatcompass.h \
    sensehataccelerometer.h \
    sensehatgyroscope.h \
    sensehatpressuresensor.h \
    sensehattemperaturesensor.h \
    sensehatmagnetometer.h

PLUGIN_TYPE = sensors

