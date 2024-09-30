#ifndef TSDOC_H
#define TSDOC_H

#include <QList>
#include <QObject>
#include <libxml/SAX.h>
#include <memory.h>

class TsContext;

class TsDoc : public QObject
{
    Q_OBJECT
public:
    explicit TsDoc();
    ~TsDoc();

    bool setSource(const QString &path);
    void save(const QString &path = "");

    std::shared_ptr<TsContext> firstContext() const;
    QList<std::shared_ptr<TsContext>> listContext() const;
    const std::shared_ptr<TsContext> findContext(const QString &name);

    void copyFrom(const TsDoc *doc);
    static std::shared_ptr<TsDoc> create(const QString &path);
signals:
    void sourceChange();

private:
    xmlDocPtr doc = nullptr;
};

#endif // TSDOC_H
