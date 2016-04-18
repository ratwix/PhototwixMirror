#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
#include "parameters.h"
#include "videoitem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Parameters parameters(QGuiApplication::applicationDirPath());

    engine.rootContext()->setContextProperty("parameters", &parameters);

    qmlRegisterType<VideoItem>("com.phototwix.components", 1, 0, "VideoItem");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
