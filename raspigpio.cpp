#include <wiringPi.h>
#include <QQmlEngine>
#include <iostream>
#include "raspigpio.h"
#include "clog.h"

std::function<void()> cbfunc;

void actionButtonCallback()
{
    cbfunc();
}

void actionButtonRealCallback(RaspiGPIO *r)
{
    if (r && r->canPush()) {
        emit r->canPushFalse(); //Disable button for 1 second
        CLog::Write(CLog::Debug, "Action button pushed");
        emit r->actionButtonPushed();
    }
}

int myWiringPiISR(int val, int mask, std::function<void()> callback)
{
  cbfunc = callback;
  return wiringPiISR(val, mask, &actionButtonCallback);
}

RaspiGPIO::RaspiGPIO(QObject *parent) : QObject(parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    //Export pin
    QString cmd = QString("sudo gpio export ") + BUTTON_ACTION_PIN_EXPORT + " in";
    system(cmd.toStdString().c_str());

    cmd = QString("sudo gpio export ") + FLASH_COMMAND_PIN_EXPORT + " out";
    system(cmd.toStdString().c_str());

    //Setup button
    wiringPiSetupSys();
    pinMode (BUTTON_ACTION_PIN, INPUT);
    pullUpDnControl(BUTTON_ACTION_PIN, PUD_DOWN);

    //Setup button delay
    m_canPush = true;
    m_canPushTimer = new QTimer(this);
    m_canPushTimer->setSingleShot(true);
    connect(this, SIGNAL(canPushFalse()), this, SLOT(canPushTimerFalse()));
    connect(m_canPushTimer, SIGNAL(timeout()), this, SLOT(canPushTimerTrue()));

    //action button callback
    if (myWiringPiISR(BUTTON_ACTION_PIN, INT_EDGE_FALLING, std::bind(actionButtonRealCallback, this)) < 0) {
        CLog::Write(CLog::Error, QString("Unable to setup ISR: ") + QString::number(errno));
    }

    //Setup flash gpio
    pinMode (FLASH_COMMAND_PIN, OUTPUT);
    activeFlash(false);
}

void RaspiGPIO::activeFlash(bool on)
{
    if (on) {
        digitalWrite(FLASH_COMMAND_PIN, HIGH);
    } else {
        digitalWrite(FLASH_COMMAND_PIN, LOW);
    }
}


bool RaspiGPIO::canPush() const
{
    return m_canPush;
}

void RaspiGPIO::setCanPush(bool canPush)
{
    m_canPush = canPush;
}

void RaspiGPIO::canPushTimerTrue()
{
    m_canPush = true;
}

void RaspiGPIO::canPushTimerFalse()
{
    m_canPush = false;
    m_canPushTimer->start(1000);
}

