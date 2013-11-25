#include "leftbagdetectorplugin.h"
#include <QtCore>

// opencv includes
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

LeftbagdetectorPlugin::LeftbagdetectorPlugin()
{


}

LeftbagdetectorPlugin::~LeftbagdetectorPlugin()
{
    debugMsg("Left Bag Plugin Distroyied");

}

bool LeftbagdetectorPlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{
    cv::cvtColor(in, out, CV_BGR2GRAY);

    blobPositionReader.processEvents(startingDummy);
    return true;
}

bool LeftbagdetectorPlugin::init()
{

    QDateTime timestamp = QDateTime::currentDateTime();

    //connect(&blobPositionReader, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));
    //connect(&blobPositionReader, SIGNAL(generateEvent(QList<DetectedEvent>)), &blobDistanceNode, SLOT(captureEvent(QList<DetectedEvent>)));
    //connect(&blobPositionReader, SIGNAL(generateEvent(QList<DetectedEvent>)), &blobSpeedNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(this, SIGNAL(generateEvent(QList<DetectedEvent>)), &blobDistanceNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(this, SIGNAL(generateEvent(QList<DetectedEvent>)), &blobSpeedNode, SLOT(captureEvent(QList<DetectedEvent>)));

    connect(&blobDistanceNode, SIGNAL(generateEvent(QList<DetectedEvent>)), &distanceChangeNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&blobDistanceNode, SIGNAL(generateEvent(QList<DetectedEvent>)), &leftBagNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&distanceChangeNode, SIGNAL(generateEvent(QList<DetectedEvent>)), &leftBagNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&blobSpeedNode, SIGNAL(generateEvent(QList<DetectedEvent>)), &leftBagNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&leftBagNode, SIGNAL(generateEvent(QList<DetectedEvent>)), &leftBagWriterNode, SLOT(captureEvent(QList<DetectedEvent>)));

    //connect(&blobDistanceNode, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));
    //connect(&distanceChangeNode, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));
    //connect(&blobSpeedNode, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));
    connect(&leftBagNode, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));
    //connect(this, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));

    //QDir dir(QCoreApplication::instance()->applicationDirPath());
    QDir dir(QDir::home());
    if(!dir.exists("NoobaVSS")){
        dir.mkdir("NoobaVSS");
    }
    dir.cd("NoobaVSS");
    if(!dir.exists("data")){
        dir.mkdir("data");
    }
    dir.cd("data");
    if(!dir.exists("text")){
        dir.mkdir("text");
    }
    dir.cd("text");

    output_file = dir.absoluteFilePath(timestamp.currentDateTime().toString("yyyy-MM-dd-hhmm") + "-abobjects.txt");

    createStringParam("input_file",input_file,false);
    createStringParam("output_file",output_file,false);

    createDoubleParam("still_object_speed_threshold",0.01,200.0,0.0);
    createDoubleParam("leaving_object_speed_threshold",3.0,200.0,0.0);
    createDoubleParam("distance_change_rate_threshold",1.0,200.0,0.0);
    createDoubleParam("split_min_limit",250.0,500,0.0);
    createDoubleParam("split_max_limit",300.0,500.0,0.0);


    //blobPositionReader.openFile(input_file);
    leftBagWriterNode.openFile(output_file);

    leftBagNode.setStillObjectSpeedThreshold(0.01);
    leftBagNode.setLeavingObjectSpeedThreshold(3.0);
    leftBagNode.setDistanceChangeRateThreshold(1.0);
    leftBagNode.setSplitMinLimit(250.0);
    leftBagNode.setSplitMaxLimit(300.0);

    debugMsg("Left Bag Detector Plugin Initialized");

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
        input_file = val;
        blobPositionReader.openFile(input_file);
        debugMsg("input_file set to "  + val);
    }
    else if(varName == "output_file"){
        output_file = val;
        blobPositionReader.openFile(output_file);
        debugMsg("output_file set to "  + val);
    }

}

void LeftbagdetectorPlugin::onDoubleParamChanged(const QString& varName, double val){

    if(varName == "still_object_speed_threshold"){
        leftBagNode.setStillObjectSpeedThreshold(val);
        debugMsg(QString("still_object_speed_threshold set to %1").arg(val));
    }
    else if(varName == "leaving_object_speed_threshold"){
        leftBagNode.setLeavingObjectSpeedThreshold(val);
        debugMsg(QString("leaving_object_speed_threshold set to %1").arg(val));
    }
    else if(varName == "distance_change_rate_threshold"){
        leftBagNode.setDistanceChangeRateThreshold(val);
        debugMsg(QString("distance_change_rate_threshold set to %1").arg(val));
    }
    else if(varName == "split_min_limit"){
        leftBagNode.setSplitMinLimit(val);
        debugMsg(QString("split_min_limit set to %1").arg(val));
    }
    else if(varName == "split_max_limit"){
        leftBagNode.setSplitMaxLimit(val);
        debugMsg(QString("split_max_limit set to %1").arg(val));
    }

}
void LeftbagdetectorPlugin::onCaptureEvent(QList<DetectedEvent> captured_event){

    foreach(DetectedEvent e, captured_event){
        debugMsg(QString(e.getIdentifier() + " " + e.getMessage() + " %1").arg(e.getConfidence()));
    }
}

void LeftbagdetectorPlugin::inputData(const QStringList &strList, QList<QImage> imageList){
    Q_UNUSED(imageList)
    QStringList stringList = strList;
    QList<DetectedEvent> receivedEvents;
    foreach(QString str,stringList){
        //debugMsg("recv" + str);
        QList<QString> parameters = str.split(" ");
        receivedEvents.append(DetectedEvent(parameters.at(0),parameters.at(1),parameters.at(2).toFloat()));
    }
    emit generateEvent(receivedEvents);
}

// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(LeftbagdetectorPlugin, LeftbagdetectorPlugin);
#endif
