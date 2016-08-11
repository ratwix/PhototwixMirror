#include "photopart.h"

PhotoPart::PhotoPart():
    m_path("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

PhotoPart::PhotoPart(TemplatePhotoPosition *tpp):
    m_path("")
{
    CLog::Write(CLog::Debug, "Add new photo part to photo");
    m_photoPosition = tpp;
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

PhotoPart::~PhotoPart() {
    CLog::Write(CLog::Debug, "Delete Photopart");
}

TemplatePhotoPosition *PhotoPart::photoPosition() const
{
    return m_photoPosition;
}

void PhotoPart::setPhotoPosition(TemplatePhotoPosition *photoPosition)
{
    m_photoPosition = photoPosition;
}
QUrl PhotoPart::path() const
{
    return m_path;
}

void PhotoPart::setPath(const QUrl &path)
{
    CLog::Write(CLog::Debug, "Ecriture du path " + path.toString().toStdString());
    m_path = path;
    emit pathChanged();
}

void PhotoPart::Serialize(PrettyWriter<StringBuffer> &writer) const
{
    writer.StartObject();
    writer.Key("photoPartUrl");
    writer.String(m_path.toString().toStdString().c_str());
    writer.EndObject();
}




