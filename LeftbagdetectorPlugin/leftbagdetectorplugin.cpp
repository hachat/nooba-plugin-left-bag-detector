#include "leftbagdetectorplugin.h"
#include <QtCore>
#include <opencv2/core/core.hpp>

LeftbagdetectorPlugin::LeftbagdetectorPlugin()
{

}

LeftbagdetectorPlugin::~LeftbagdetectorPlugin()
{

}

bool LeftbagdetectorPlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{

    return true;
}

bool LeftbagdetectorPlugin::init()
{
    return true;
}

bool LeftbagdetectorPlugin::release()
{
    return true;
}

PluginInfo LeftbagdetectorPlugin::getPluginInfo() const
{
    PluginInfo pluginInfo(
        "Leftbagdetector Plugin",
        0,
        1,
        "Plugin Description goes here",
        "Plugin Creator");
    return pluginInfo;
}


// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(LeftbagdetectorPlugin, LeftbagdetectorPlugin);
#endif
