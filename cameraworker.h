#ifndef CAMERAWORKER_H
#define CAMERAWORKER_H

#include <QObject>
#include <QProcess>

// Standard port setting for the camera component

#define PICAM_JPEG_QUALITY  85
#define CAMERA_WIDTH    3280    //2592
#define CAMERA_HEIGHT   2464    //1944

#define CAMERA_PREVIEW_4x3_WIDTH    1296    //1024
#define CAMERA_PREVIEW_4x3_HEIGHT   972     //768

#define CAMERA_PREVIEW_16x9_WIDTH   1280
#define CAMERA_PREVIEW_16x9_HEIGHT  720

#define CAMERA_PREVIEW_X            0
#define CAMERA_PREVIEW_Y            0
#define CAMERA_PREVIEW_WIDTH        639
#define CAMERA_PREVIEW_HEIGHT       480

class CameraWorker : public QObject
{
    Q_OBJECT
public:
    CameraWorker();

    ~CameraWorker();

    /**
      * Send a signal to prreview to take the photo and quit
      * */
    Q_INVOKABLE void capturePhoto();

    /**
      * Start a preview and wait a keypress to exit (capturePhoto() )
      * */
    Q_INVOKABLE void startPreview(const QString &fileName, int previewX=CAMERA_PREVIEW_X,
                                                            int previewY=CAMERA_PREVIEW_Y,
                                                            int previewWidth=CAMERA_PREVIEW_WIDTH,
                                                            int previewHeight=CAMERA_PREVIEW_HEIGHT);

private:
    QProcess *  m_previewProcess;
    QString     currentFilename;
signals:
    void imageCaptured(const QString &filename);
public slots:
    void capturePhotoEnd();
};

#endif // CAMERA_H
