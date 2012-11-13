#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>

class PluginInterface
{
public:
    //Returns the name of the plugin
    virtual QString Name() const = 0;

    //Returns the description of the plugin
    virtual QString Description() const = 0;

    //Is runned when a recording has successfully finished
    virtual void PostRecording(QString recordingFilename,int recordingduration_s=0) const = 0;
};

Q_DECLARE_INTERFACE( PluginInterface, "org.FDesktopRecorder.PluginInterface/0.1" )


#endif // PLUGININTERFACE_H
