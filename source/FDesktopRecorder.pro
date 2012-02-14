#-------------------------------------------------
#
# Project created by QtCreator 2012-01-29T08:47:10
#
#-------------------------------------------------

QT       += core gui

TARGET = FDesktopRecorder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    configurationfile.cpp \
    aboutprog.cpp \
    windowgrapper.cpp \
    recordingdevices.cpp \
    settingsdialog.cpp \
    process.cpp \
    settingsmanager.cpp

HEADERS  += mainwindow.h \
    configurationfile.h \
    aboutprog.h \
    windowgrapper.h \
    recordingdevices.h \
    settingsdialog.h \
    process.h \
    settingsmanager.h

FORMS    += mainwindow.ui\
            aboutprog.ui \
    settingsdialog.ui


TRANSLATIONS    = translations/FDesktopRecorder_da_DK.ts \
                  translations/FDesktopRecorder_de_DE.ts \
                  translations/FDesktopRecorder_untranslated.ts \


RESOURCES += \
    resources.qrc



































