#ifndef RASPIGPIO_H
#define RASPIGPIO_H

#include <QObject>
#include <QTimer>

#define LED_PIN             18 //PIN 12 / GPIO_GEN1 / GPIO18
#define BUTTON_PHOTO_PIN    27 //PIN 13 / GPIO_GEN2 / GPIO27

class RaspiGPIO : public QObject
{
    Q_OBJECT
public:
    explicit RaspiGPIO(QObject *parent = 0);

    Q_INVOKABLE void blink(int msec, int mspeed);

private:
    QTimer  m_blinkTimer;
    bool    m_blinkState;
signals:
    void    blinkSignal();
public slots:
    void    blinkChange();
    void    blinkStop();

};

#endif // RASPIGPIO_H
