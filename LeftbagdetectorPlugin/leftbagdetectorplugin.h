#ifndef LEFTBAGDETECTORPLUGIN_H
#define LEFTBAGDETECTORPLUGIN_H

#include "leftbagdetectorplugin_global.h"
#include "noobapluginapi.h"

#include <QObject>

class LEFTBAGDETECTORPLUGIN_EXPORT LeftbagdetectorPlugin: public NoobaPluginAPI
{
    Q_OBJECT
    Q_INTERFACES(NoobaPluginAPI)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "nooba.plugins.qt5.leftbagdetector-plugin" FILE "leftbagdetectorPlugin.json")
#endif

public:
    LeftbagdetectorPlugin();
    ~LeftbagdetectorPlugin();

    bool procFrame(const cv::Mat &in, cv::Mat &out, ProcParams &params);
    bool init();
    bool release();
    PluginInfo getPluginInfo() const;

public slots:

    /**
     * These functions will be called when the parameters are changed by the
     * user.
     */
//    void onIntParamChanged(const QString& varName, int val);
//    void onDoubleParamChanged(const QString& varName, double val);
//    void onStringParamChanged(const QString& varName, const QString& val);
//    void onMultiValParamChanged(const QString& varName, const QString& val);


};

#endif // LEFTBAGDETECTORPLUGIN_H
