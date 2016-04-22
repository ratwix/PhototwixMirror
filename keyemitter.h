#ifndef KEYEMITTER_H
#define KEYEMITTER_H

#include <QObject>
#include <string>

using namespace std;

class KeyEmitter  : public QObject
{
    Q_OBJECT
public:
    KeyEmitter();
    ~KeyEmitter();

private:
    struct keyMesage {
        Qt::Key key;
        Qt::KeyboardModifiers modifiers;
        QString text;
    };

    keyMesage stringToKey(QString s);


public slots:
    void emitKey(keyMesage k);
    void emitKey(QString key);
};

#endif // KEYEMITTER_H
