#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTranslator>
#include <QSettings>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name();
    qDebug() << "Locale:" << locale;

    //Loads the translations
    QTranslator translator;
    translator.load(":/translations/FDesktopRecorder_"+locale);
    a.installTranslator(&translator);


    //Loads the settingsfile
    QCoreApplication::setOrganizationName("fdesktoprecorder");
    QCoreApplication::setApplicationName("fdesktoprecorder");

    //Reads startup behavoir
    QSettings configurationfile;

    MainWindow w;

    w.show();




    return a.exec();
}
