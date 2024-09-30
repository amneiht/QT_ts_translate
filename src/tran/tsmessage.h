#ifndef TSMESSAGE_H
#define TSMESSAGE_H

#include <QObject>
#include <libxml/tree.h>

class TsDoc;
class TsContext;
class TsContent;
class TsString;

class TsMessage : public QObject
{
    Q_OBJECT
public:
    enum Type {
        String,
        Numerus,
    };
    explicit TsMessage(xmlNodePtr node, const TsDoc *ctx = nullptr);
    QString getSource() const;
    std::shared_ptr<TsContent> getContent() const;

    std::shared_ptr<TsMessage> next() const;
    Type getType() const;

    void setFinished(bool finish);
    std::shared_ptr<TsContext> getContext();

private:
    const TsDoc *con;
    xmlNodePtr node;
    Type m_type = Type::String;
private slots:
    void onSourceChanged();
};

class TsContent
{
public:
    TsContent(xmlNodePtr node);
    TsMessage::Type getType() const;

protected:
    TsMessage::Type m_type;
    xmlNodePtr node;
};
class TsNumerus : public TsContent
{
public:
    TsNumerus(xmlNodePtr snode);
    QString getBase() const;
    void setBase(const QString &data);
    QString getNumerus() const;
    void setNumerus(const QString &data);

private:
    xmlNodePtr m_base;
    xmlNodePtr m_numerus;
};

class TsString : public TsContent
{
public:
    TsString(xmlNodePtr snode);
    void setContent(const QString &content);
    QString getContent() const;
};

#endif // TSMESSAGE_H
