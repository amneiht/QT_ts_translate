#ifndef TSCONTEXT_H
#define TSCONTEXT_H

#include <QObject>
#include <libxml/tree.h>
#include <memory.h>
class TsDoc;
class TsMessage;
class TsContext : public QObject
{
    Q_OBJECT
public:
    TsContext(xmlNodePtr node, const TsDoc *doc = nullptr);
    ~TsContext();
    QString getName() const;

    std::shared_ptr<TsMessage> firstMessage() const;
    QList<std::shared_ptr<TsMessage>> getMessageList() const;
    std::shared_ptr<TsMessage> findMess(const QString &name) const;

    std::shared_ptr<TsContext> next() const;

    void copyFrom(const TsContext *context);
signals:
    void sourceChange();

private:
    // keep refernce
    const TsDoc *par = nullptr;
    xmlNodePtr node;
private slots:
    void onSourceChanged();
};

#endif // TSCONTEXT_H
