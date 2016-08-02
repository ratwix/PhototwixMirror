#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include "parameters.h"
#include "keyemitter.h"
#include "videoitem.h"
#include "clog.h"
#include "mail.h"

int main(int argc, char *argv[])
{
    CLog::SetLevel(CLog::Debug);

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    Parameters parameters(QGuiApplication::applicationDirPath());
    KeyEmitter keyEmitter;

    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));


    qmlRegisterType<VideoItem>("com.phototwix.components", 1, 0, "VideoItem");
    qmlRegisterType<Template>("com.phototwix.components", 1, 0, "Template");
    qmlRegisterType<TemplatePhotoPosition>("com.phototwix.components", 1, 0, "TemplatePhotoPosition");
    qmlRegisterType<PhotoGallery>("com.phototwix.components", 1, 0, "PhotoGallery");
    qmlRegisterType<Photo>("com.phototwix.components", 1, 0, "Photo");
    qmlRegisterType<PhotoPart>("com.phototwix.components", 1, 0, "PhotoPart");
    qmlRegisterType<Mail>("com.phototwix.components", 1, 0, "Mail");
    qmlRegisterType<KeyEmitter>("com.phototwix.components", 1, 0, "KeyEmitter");

    engine.rootContext()->setContextProperty("parameters", &parameters);
    engine.rootContext()->setContextProperty("applicationDirPath", QGuiApplication::applicationDirPath());
    engine.rootContext()->setContextProperty("parameters", &parameters);
    engine.rootContext()->setContextProperty("photoGallery", parameters.getPhotogallery());
    engine.rootContext()->setContextProperty("keyEmitter", &keyEmitter);

    CLog::Write(CLog::Debug, ("Application dir path "  + QGuiApplication::applicationDirPath()).toStdString());

    engine.load(QUrl(QStringLiteral("qrc:/CommandScreenMain.qml")));

    return app.exec();
}
