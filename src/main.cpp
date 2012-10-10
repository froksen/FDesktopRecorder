#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTranslator>
#include <QSettings>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Loads the settingsfile
    QCoreApplication::setOrganizationName("fdesktoprecorder");
    QCoreApplication::setApplicationName("fdesktoprecorder");

    QString locale = QLocale::system().name();
    qDebug() << "-----  Localization setup  -----";
    qDebug() << "System default locale:" << locale;

    //Reads startup behavoir

    QSettings configurationfile;
    configurationfile.beginGroup("startupbehavior");
    if(configurationfile.value("language").toString() != "default" && configurationfile.contains("language"))
    {
        qDebug() << "Using locale:" << configurationfile.value("language").toString();
       locale = configurationfile.value("language").toString();
    }


    configurationfile.endGroup();

    //Loads the translations
    QTranslator translator;
    translator.load(":/translations/FDesktopRecorder.main/"+locale);
    a.installTranslator(&translator);






    MainWindow w;
    w.show();




    return a.exec();
}
