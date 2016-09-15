#ifndef RASPIGPIO_H
#define RASPIGPIO_H

#include <QObject>
#include <QTimer>
#include "clog.h"


#define BUTTON_ACTION_PIN           2  //PIN 5 / GPIO_GEN3
#define BUTTON_ACTION_PIN_EXPORT    3  //PIN 5 / GPIO_GEN3

class RaspiGPIO : public QObject
{
    Q_OBJECT
public:
    explicit RaspiGPIO(QObject *parent = 0);


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
