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
    utils/rubberband.cpp \
    utils/areaselectordialog.cpp


HEADERS  += mainwindow.h \
    dialogs/about/aboutprog.h \
    utils/windowgrapper.h \
    module/recordingdevices.h \
    dialogs/settingsdialog/settingsdialog.h \
    module/process.h \
    module/settingsmanager.h \
    utils/rubberband.h \
    utils/areaselectordialog.h

FORMS    += mainwindow.ui\
            dialogs/about/aboutprog.ui \
    dialogs/settingsdialog/settingsdialog.ui \
            utils/areaselectordialog.ui \


TRANSLATIONS    = translations/FDesktopRecorder.main/da_DK.ts \
                  translations/FDesktopRecorder.main/de_DE.ts \
                  translations/FDesktopRecorder.main/es_ES.ts \
                  translations/FDesktopRecorder.main/it_IT.ts \
                  translations/FDesktopRecorder.main/ro_RO.ts \
                  translations/FDesktopRecorder.main/fr_FR.ts \
                  translations/FDesktopRecorder.main/el_GR.ts \
		  translations/FDesktopRecorder.main/ru_RU.ts \
                  translations/FDesktopRecorder.main/pt_BR.ts \
                  translations/FDesktopRecorder.main/zh_CN.ts \
		  translations/FDesktopRecorder.main/pl_PL.ts \
		  translations/FDesktopRecorder.main/uk_UA.ts \
                  translations/FDesktopRecorder_untranslated.ts


RESOURCES += \
    resources.qrc





































