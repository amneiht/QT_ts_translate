#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <QObject>
#include <qnetworkaccessmanager.h>

class Translate : public QObject
{
    Q_OBJECT
public:
    explicit Translate(QObject *parent = nullptr);

    void translateToVn(const QString &text);
signals:
    void translateComplete(const QString &tran);

private:
    QNetworkAccessManager net;
private slots:
    void onComplete(QNetworkReply *reply);
};

#endif // TRANSLATE_H
