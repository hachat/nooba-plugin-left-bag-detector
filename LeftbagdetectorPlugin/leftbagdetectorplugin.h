#ifndef LEFTBAGDETECTORPLUGIN_H
#define LEFTBAGDETECTORPLUGIN_H

#include "leftbagdetectorplugin_global.h"
#include "noobapluginapi.h"
#include <QObject>

#define DEFAULT_STILL_OBJECT_SPEED_THRESHOLD 0.01
#define DEFAULT_LEAVING_OBJECT_SPEED_THRESHOLD 3.0
#define DEFAULT_DISTANCE_CHANGE_RATE_THRESHOLD 1.0
#define DEFAULT_SPLIT_MIN_LIMIT 250.0
#define DEFAULT_SPLIT_MAX_LIMIT 300.0


#include <NoobaVSSAD/readernode.h>
#include <NoobaVSSAD/distancenode.h>
#include <NoobaVSSAD/filewriternode.h>
#include <NoobaVSSAD/speednode.h>
#include <NoobaVSSAD/distancechange.h>
#include <NoobaVSSAD/abandonedobjectnode.h>


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
    void onDoubleParamChanged(const QString& varName, double val);
    void onStringParamChanged(const QString& varName, const QString& val);
//    void onMultiValParamChanged(const QString& varName, const QString& val);

    void onCaptureEvent(QList<DetectedEvent> captured_event);

    void inputData(const QStringList &strList, QList<QImage> imageList);
signals:
    void generateEvent(QList<DetectedEvent> generated_event);


private:
    ReaderNode blobPositionReader;
    DistanceNode blobDistanceNode;
    SpeedNode blobSpeedNode;
    AbandonedObjectNode leftBagNode;
    DistanceChange distanceChangeNode;
    FileWriterNode leftBagWriterNode;
    FileWriterNode blobWriterNode;

    QList<DetectedEvent> startingDummy;

    //Parameters
    QString input_file;
    QString output_file;

};

#endif // LEFTBAGDETECTORPLUGIN_H
