#ifndef CUSTOMSLIBRARY_H
#define CUSTOMSLIBRARY_H

#include <QObject>
#include <QJsonObject>

class CustomsLibrary : public QObject
{
    Q_OBJECT
public:
    explicit CustomsLibrary(QObject *pParent = nullptr);

    std::optional<std::shared_ptr<QJsonObject>> GetCustomJson(const QString& pFileId) const;

    void AddCustomJson(const QString& pFileId, const QJsonObject& pJson);

protected:
    std::map<QString, QJsonObject> mCustoms;
};

#endif // CUSTOMSLIBRARY_H
