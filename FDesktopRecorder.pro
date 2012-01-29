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
    runterminal.cpp \
    configurationfile.cpp \
    aboutprog.cpp \
    windowgrapper.cpp \
    dialogterminaloutput.cpp \
    recordingdevices.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    runterminal.h \
    configurationfile.h \
    aboutprog.h \
    windowgrapper.h \
    dialogterminaloutput.h \
    recordingdevices.h \
    settingsdialog.h

FORMS    += mainwindow.ui\
            aboutprog.ui \
    dialogterminaloutput.ui \
    settingsdialog.ui


TRANSLATIONS    = translations/FDesktopRecorder_da_DK.ts \
                  translations/FDesktopRecorder_de_DE.ts \

















