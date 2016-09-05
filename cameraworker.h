#ifndef CAMERAWORKER_H
#define CAMERAWORKER_H

#include <QObject>
#include <QImage>
#include <QQuickImageProvider>

#include "interface/vcos/vcos.h"

#include "interface/mmal/mmal.h"
#include "interface/mmal/mmal_logging.h"
#include "interface/mmal/mmal_buffer.h"
#include "interface/mmal/util/mmal_util.h"
#include "interface/mmal/util/mmal_util_params.h"
#include "interface/mmal/util/mmal_default_components.h"
#include "interface/mmal/util/mmal_connection.h"
#include "interface/mmal/mmal_parameters_camera.h"

// Standard port setting for the camera component
#define MMAL_CAMERA_PREVIEW_PORT 0
#define MMAL_CAMERA_VIDEO_PORT 1
#define MMAL_CAMERA_CAPTURE_PORT 2


// Stills format information
// 0 implies variable
#define STILLS_FRAME_RATE_NUM 0
#define STILLS_FRAME_RATE_DEN 1

/// Video render needs at least 2 buffers.
#define VIDEO_OUTPUT_BUFFERS_NUM 3

#define PREVIEW_FRAME_RATE_NUM 0
#define PREVIEW_FRAME_RATE_DEN 1

#define PICAM_JPEG_QUALITY  85
#define PICAM_RESTART_INTERVAL  0

class CameraWorker : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    CameraWorker(QQuickImageProvider::ImageType type=QQuickImageProvider::Image,
                 QQuickImageProvider::Flags flags=QQmlImageProviderBase::ForceAsynchronousImageLoading);

    ~CameraWorker();

    Q_INVOKABLE void capturePhoto(const QString &fileName);
    Q_INVOKABLE void openCamera();
    Q_INVOKABLE void closeCamera();

    virtual QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize);
    QImage preview_result() const;
    void setPreview_result(const QImage &preview_result);

private:
    bool                m_camera_enabled;

    MMAL_PORT_T         *m_preview_port;
    MMAL_PORT_T         *m_still_port;
    MMAL_PORT_T         *m_video_port;
    MMAL_COMPONENT_T    *m_camera;
    MMAL_POOL_T         *m_encoder_pool;
    MMAL_COMPONENT_T    *m_encoder_component;
    MMAL_CONNECTION_T   *m_encoder_connection;

    QImage              m_preview_result;

    MMAL_STATUS_T connect_ports(MMAL_PORT_T *output_port, MMAL_PORT_T *input_port, MMAL_CONNECTION_T **connection);
    void          initCamera();
    void          freeCamera();
    void          init_still_encoder();

signals:
    void imageCaptured(const QString &filename);
public slots:

};

#endif // CAMERA_H
