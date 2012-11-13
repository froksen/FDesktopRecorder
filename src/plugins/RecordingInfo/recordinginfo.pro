TEMPLATE = lib
TARGET = RecordingInfo
CONFIG += plugin release
VERSION = 1.0.0

INSTALLS += target

HEADERS += \
    PluginInterface.h \
    recordinginfo.h

SOURCES += \
    recordinginfo.cpp



