#include "leftbagdetectorplugin.h"
#include <QtCore>

// opencv includes
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

LeftbagdetectorPlugin::LeftbagdetectorPlugin()
    :blobPositionReader(),
      startingDummy()
{

    //connect(&blobPositionReader, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));
    connect(&blobPositionReader, SIGNAL(generateEvent(QList<DetectedEvent>)), &blobDistanceNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&blobPositionReader, SIGNAL(generateEvent(QList<DetectedEvent>)), &blobSpeedNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&blobDistanceNode, SIGNAL(generateEvent(QList<DetectedEvent>)), &distanceChangeNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&blobDistanceNode, SIGNAL(generateEvent(QList<DetectedEvent>)), &leftBagNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&distanceChangeNode, SIGNAL(generateEvent(QList<DetectedEvent>)), &leftBagNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&blobSpeedNode, SIGNAL(generateEvent(QList<DetectedEvent>)), &leftBagNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&leftBagNode, SIGNAL(generateEvent(QList<DetectedEvent>)), &leftBagWriterNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&leftBagNode, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));

}

LeftbagdetectorPlugin::~LeftbagdetectorPlugin()
{
    debugMsg("LeftBag Plugin initialized");

}

bool LeftbagdetectorPlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{
    cv::cvtColor(in, out, CV_BGR2GRAY);

    blobPositionReader.processEvents(startingDummy);
    return true;
}

bool LeftbagdetectorPlugin::init()
{
    input_file = "/home/chathuranga/Programming/FYP/data/text/2013-10-24-sample-blobs.txt";
    output_file = "/home/chathuranga/Programming/FYP/data/text/2013-10-24-blobs-sample-abobjects.txt";

    createStringParam("input_file",input_file,false);
    createStringParam("output_file",output_file,false);
    blobPositionReader.openFile(input_file);
    leftBagWriterNode.openFile(output_file);
    return true;
}

bool LeftbagdetectorPlugin::release()
{
    blobPositionReader.closeFile();
    leftBagWriterNode.closeFile();
    return true;
}

PluginInfo LeftbagdetectorPlugin::getPluginInfo() const
{
    PluginInfo pluginInfo(
        "Left Bag Detector Plugin",
        0,
        1,
        "This is an anomaly detectior for abandoned bags",
        "Chathuranga Hettiarachchi");
    return pluginInfo;
}

void LeftbagdetectorPlugin::onStringParamChanged(const QString& varName, const QString& val){
    if(varName == "input_file"){
        blobPositionReader.openFile(input_file);
        debugMsg("input_file set to "  + val);
    }
    else if(varName == "output_file"){
        blobPositionReader.openFile(output_file);
        debugMsg("output_file set to "  + val);
    }
}


void LeftbagdetectorPlugin::onCaptureEvent(QList<DetectedEvent> captured_event){

    foreach(DetectedEvent e, captured_event){
        debugMsg(e.getIdentifier() + " " + e.getMessage() + " " + e.getConfidence());
    }
}

// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(LeftbagdetectorPlugin, LeftbagdetectorPlugin);
#endif
