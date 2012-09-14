#-------------------------------------------------
#
# Project created by QtCreator 2012-01-29T08:47:10
#
#-------------------------------------------------

QT       += core gui
QT += phonon

LIBS += -lkdeui

TARGET = FDesktopRecorder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogs/about/aboutprog.cpp \
    utils/windowgrapper.cpp \
    module/recordingdevices.cpp \
    dialogs/settingsdialog/settingsdialog.cpp \
    module/process.cpp \
    module/settingsmanager.cpp \
    utils/rubberband.cpp

HEADERS  += mainwindow.h \
    dialogs/about/aboutprog.h \
    utils/windowgrapper.h \
    module/recordingdevices.h \
    dialogs/settingsdialog/settingsdialog.h \
    module/process.h \
    module/settingsmanager.h \
    utils/rubberband.h

FORMS    += mainwindow.ui\
            dialogs/about/aboutprog.ui \
    dialogs/settingsdialog/settingsdialog.ui \


TRANSLATIONS    = translations/FDesktopRecorder_da_DK.ts \
                  translations/FDesktopRecorder_de_DE.ts \
                  translations/FDesktopRecorder_es_ES.ts \
                  translations/FDesktopRecorder_it_IT.ts \
		  translations/FDesktopRecorder_ro_RO.ts \
                  translations/FDesktopRecorder_untranslated.ts \


RESOURCES += \
    resources.qrc





































