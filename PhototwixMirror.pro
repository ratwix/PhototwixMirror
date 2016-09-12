TEMPLATE = app

QT += qml quick widgets concurrent core network
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
    effect.cpp \
    filedownloader.cpp \
    photoqueue.cpp \
    photoqueuetwitter.cpp \
    photoqueuemanager.cpp \
    wifimanager.cpp \
    wifiitem.cpp \
    raspigpio.cpp \
    printermanager.cpp \
    cameraworker.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =
INCLUDEPATH += $[QT_SYSROOT]/usr/local/include
LIBS += -L$[QT_SYSROOT]/usr/local/lib -lwiringPi -L$[QT_SYSROOT]/opt/vc/lib -lbcm_host

QMAKE_CXXFLAGS += -std=c++0x -mno-unaligned-access
# Default rules for deployment.
include(deployment.pri)

# TODO: trouver la bonne variable pour pas mettre le path entier $$DESTDIR_TARGET : non
scripts.path += /opt/PhototwixMirror/bin/scripts
scripts.files = scripts/*
INSTALLS += scripts


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
    effect.h \
    filedownloader.h \
    photoqueue.h \
    photoqueuetwitter.h \
    photoqueuemanager.h \
    wifimanager.h \
    wifiitem.h \
    raspigpio.h \
    printermanager.h \
    cameraworker.h

DISTFILES += \
    scripts/connectWifi.sh \
    scripts/get_paper.sh \
    scripts/print.sh \
    scripts/dnpds40
