#include <QDebug>
#include <QFile>
#include <string.h>

#include "clog.h"
#include "cameraworker.h"

CameraWorker::CameraWorker()
{
    m_previewProcess = new QProcess(this);
    connect(m_previewProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus){ capturePhotoEnd();});
}

CameraWorker::~CameraWorker()
{

}

void CameraWorker::startPreview(const QString &fileName, int previewX, int previewY, int previewWidth, int previewHeight)
{
    if (!m_previewProcess->isOpen()) {
        currentFilename = fileName;
        QString program = "raspistill";
        QStringList arguments;

        if (previewX != CAMERA_PREVIEW_X || previewY != CAMERA_PREVIEW_Y || previewWidth != CAMERA_PREVIEW_WIDTH || previewHeight != CAMERA_PREVIEW_HEIGHT) {
            arguments << "--preview" << QString::number(previewX) + ',' + QString::number(previewY) + ',' + QString::number(previewWidth) + ',' + QString::number(previewHeight);
        }

        arguments << "-h" << QString::number(CAMERA_HEIGHT) << "-w" << QString::number(CAMERA_WIDTH)
                  << "-k" << "-o" << fileName;


        m_previewProcess->start(program, arguments);
    }
}

void CameraWorker::capturePhoto()
{
    //Capture photo and end process by sending x to input chanel

    if (m_previewProcess->isOpen()) {
        m_previewProcess->write("x\n");
        //m_previewProcess->closeWriteChannel();
    }
}

void CameraWorker::capturePhotoEnd()
{
    m_previewProcess->close();
    emit imageCaptured(currentFilename);
}
