TEMPLATE = lib
TARGET = qtsensors_sensehat
QT = core sensors

OTHER_FILES = plugin.json
CONFIG += c++11

LIBS += -lRTIMULib

SOURCES += main.cpp \
    sensehatsensorbase.cpp \
    sensehatcompass.cpp

HEADERS += \
    sensehatsensorbase.h \
    qsensors-sensehatpluginprivate_p.h \
    sensehatcompass.h

PLUGIN_TYPE = sensors
PLUGIN_CLASS_NAME = QSenseHatSensorPlugin

