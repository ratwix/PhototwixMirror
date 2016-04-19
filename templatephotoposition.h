#ifndef TEMPLATEPHOTOPOSITION
#define TEMPLATEPHOTOPOSITION

#include <QObject>
#include <QUrl>
#include <string>
#include "clog.h"

#include "common.h"

#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "parameters.h"

using namespace std;
using namespace rapidjson;

class Parameters;
class TemplatePhotoPosition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int number READ number WRITE setNumber NOTIFY numberChanged)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal rotate READ rotate WRITE setRotate NOTIFY rotateChanged)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(qreal xphoto READ xphoto WRITE setXphoto NOTIFY xphotoChanged)

public:
    TemplatePhotoPosition();
    TemplatePhotoPosition(const Value &value);
    ~TemplatePhotoPosition();
    //Accessors
    qreal x() const;
    void setX(const qreal &x);

    qreal y() const;
    void setY(const qreal &y);

    qreal rotate() const;
    void setRotate(const qreal &rotate);

    qreal width() const;
    void setWidth(const qreal &width);

    qreal height() const;
    void setHeight(const qreal &height);

    qreal xphoto() const;
    void setXphoto(const qreal &xphoto);
    //End of accessors

    void Serialize(PrettyWriter<StringBuffer>& writer) const;
    void Unserialize(Value const &value);

    int number() const;
    void setNumber(const int &number);

signals:
    void xChanged(qreal);
    void yChanged(qreal);
    void rotateChanged(qreal);
    void widthChanged(qreal);
    void heightChanged(qreal);
    void xphotoChanged();
    void numberChanged(int);

private:
    int         m_number;
    qreal       m_x;
    qreal       m_y;
    qreal       m_rotate;
    qreal       m_width;
    qreal       m_height;
    qreal       m_xphoto;
};

#endif // TEMPLATEPHOTOPOSITION


