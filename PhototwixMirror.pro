TEMPLATE = app

QT += qml quick widgets concurrent core
CONFIG += c++11

SOURCES += main.cpp \
    parameters.cpp \
    videoitem.cpp \
    clog.cpp \
    photo.cpp \
    photogallery.cpp \
    photopart.cpp \
    template.cpp \
    templatephotoposition.cpp \
    mail.cpp \
    keyemitter.cpp \
    effect.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    parameters.h \
    videoitem.h \
    common.h \
    clog.h \
    photo.h \
    photogallery.h \
    photopart.h \
    template.h \
    templatephotoposition.h \
    mail.h \
    keyemitter.h \
    effect.h
