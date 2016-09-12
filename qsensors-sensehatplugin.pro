TEMPLATE = lib
TARGET = $$qtLibraryTarget(qtsensors_sensehat)
QT = core sensors
CONFIG += plugin release 

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
    sensehatmagnetometer.cpp \
    sensehatrotationsensor.cpp

HEADERS += \
    sensehatsensorbase.h \
    qsensors-sensehatpluginprivate_p.h \
    sensehatcompass.h \
    sensehataccelerometer.h \
    sensehatgyroscope.h \
    sensehatpressuresensor.h \
    sensehattemperaturesensor.h \
    sensehatmagnetometer.h \
    sensehatrotationsensor.h

PLUGIN_TYPE = sensors

DESTDIR = $$PLUGIN_TYPE


target.path = $$[QT_INSTALL_PLUGINS]/$$PLUGIN_TYPE
INSTALLS += target

config.path=/etc/xdg/QtProject
config.files= Sensors.conf
INSTALLS += config
