#ifndef RASPIGPIO_H
#define RASPIGPIO_H

#include <QObject>
#include <QTimer>
#include "clog.h"


#define BUTTON_ACTION_PIN           2  //PIN 3 / GPIO_GEN2
#define BUTTON_ACTION_PIN_EXPORT    3  //PIN 3 / GPIO_GEN2

#define FLASH_COMMAND_PIN           4  //PIN 7 / GPIO_GEN4
#define FLASH_COMMAND_PIN_EXPORT    4 //PIN 7 / GPIO_GEN4


class RaspiGPIO : public QObject
{
    Q_OBJECT
public:
    explicit RaspiGPIO(QObject *parent = 0);

    Q_INVOKABLE void activeFlash(bool on);
    bool canPush() const;
    void setCanPush(bool canPush);

private:
    bool    m_canPush;
    QTimer  *m_canPushTimer;
signals:
    void    actionButtonPushed();
    void    canPushFalse();
public slots:
    void    canPushTimerTrue();
    void    canPushTimerFalse();
};

#endif // RASPIGPIO_H
