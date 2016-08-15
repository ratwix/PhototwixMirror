#include <wiringPi.h>
#include <QQmlEngine>
#include "raspigpio.h"
#include "clog.h"

RaspiGPIO::RaspiGPIO(QObject *parent) : QObject(parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    m_blinkState = false;

    m_blinkTimer.stop();
    connect(&m_blinkTimer, SIGNAL(timeout()), this, SLOT(blinkChange()));

    //CALL system "gpio export 18 out"
    wiringPiSetupSys();
    pinMode (LED_PIN, OUTPUT);
}

void RaspiGPIO::blink(int msec, int mspeed)
{

    m_blinkTimer.start(mspeed);
    QTimer::singleShot(msec, this, SLOT(blinkStop()));
}

void RaspiGPIO::blinkChange()
{
    if (m_blinkState) {
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
     m_blinkState = !m_blinkState;
}

void RaspiGPIO::blinkStop()
{
    CLog::Write(CLog::Debug, "blink stop");
    m_blinkTimer.stop();
    m_blinkState = false;
    digitalWrite(LED_PIN, LOW);
}
