#include <QApplication>
#include <sstream>
#include <string>
#include "templatephotoposition.h"
#include "clog.h"

string d2s(qreal const &q) {
    std::ostringstream strs;
    strs << q;
    return strs.str();
}


TemplatePhotoPosition::TemplatePhotoPosition()
{
    //initialise
    setX(0);
    setY(0);
    setHeight(0.2);
    setWidth(0.35);
    setRotate(0);
    setXphoto(0);
}

TemplatePhotoPosition::~TemplatePhotoPosition()
{
   CLog::Write(CLog::Debug, "Delete TemplatePhotoPosition");
}

TemplatePhotoPosition::TemplatePhotoPosition(const Value &value)
{
    Unserialize(value);
}

void TemplatePhotoPosition::Serialize(PrettyWriter<StringBuffer> &writer) const {
    writer.StartObject();

    writer.Key("number");
    writer.Int(number());

    writer.Key("x");
    writer.Double(x());

    writer.Key("y");
    writer.Double(y());

    writer.Key("height");
    writer.Double(height());

    writer.Key("width");
    writer.Double(width());

    writer.Key("rotate");
    writer.Double(rotate());

    writer.Key("xphoto");
    writer.Double(xphoto());

    writer.EndObject();
}

void TemplatePhotoPosition::Unserialize(const Value &value) {
    if (value.HasMember("number")) {
        setNumber(value["number"].GetInt());
    }

    if (value.HasMember("x")) {
        setX(value["x"].GetDouble());
    }

    if (value.HasMember("y")) {
        setY(value["y"].GetDouble());
    }

    if (value.HasMember("height")) {
        setHeight(value["height"].GetDouble());
    }

    if (value.HasMember("width")) {
        setWidth(value["width"].GetDouble());
    }

    if (value.HasMember("rotate")) {
        setRotate(value["rotate"].GetDouble());
    }

    if (value.HasMember("xphoto")) {
        setXphoto(value["xphoto"].GetDouble());
    }
}

int TemplatePhotoPosition::number() const
{
    return m_number;
}

void TemplatePhotoPosition::setNumber(const int &number)
{
    m_number = number;
}


qreal TemplatePhotoPosition::x() const
{
    return m_x;
}

void TemplatePhotoPosition::setX(const qreal &x)
{
    m_x = x;
}
qreal TemplatePhotoPosition::y() const
{
    return m_y;
}

void TemplatePhotoPosition::setY(const qreal &y)
{
    m_y = y;
}
qreal TemplatePhotoPosition::rotate() const
{
    return m_rotate;
}

void TemplatePhotoPosition::setRotate(const qreal &rotate)
{
    m_rotate = rotate;
}
qreal TemplatePhotoPosition::width() const
{
    return m_width;
}

void TemplatePhotoPosition::setWidth(const qreal &width)
{
    m_width = width;
}
qreal TemplatePhotoPosition::height() const
{
    return m_height;
}

void TemplatePhotoPosition::setHeight(const qreal &height)
{
    m_height = height;
}
qreal TemplatePhotoPosition::xphoto() const
{
    return m_xphoto;
}

void TemplatePhotoPosition::setXphoto(const qreal &xphoto)
{
    m_xphoto = xphoto;
    emit xphotoChanged();
}

