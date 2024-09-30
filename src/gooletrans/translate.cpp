#include "translate.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkReply>
#include <QtDebug>

const int lg = 45;
int ischar(ushort d)
{
    if (d < ' ')
        return 0;

    return 1;
}
static QString cleartext(QString in)
{
    QChar *cs = in.data();
    int d = in.length();
    QChar *str = new QChar[d + 1];
    int dem, p;
    int start;
    dem = 0;
    p = 0;
    int ispace = 1;
    int size = sizeof(QChar);
    QChar space = QChar(' ');
    for (int i = 0; i < d; i++) {
        if (ischar(cs[i].unicode())) {
            if (cs[i].unicode() == ' ') {
                if (!ispace) {
                    memcpy(&(str[p]), &(cs[i]), size);
                    p++;
                }
                ispace = 1;
            } else {
                if (!start) {
                    start = 1;
                }
                memcpy(&(str[p]), &(cs[i]), size);
                p++;
                ispace = 0;
            }
        } else {
            if (i == dem) {
                if (p > 0) {
                    //                    if(cs[i].unicode()!=str[p-1].unicode())
                    {
                        memcpy(&(str[p]), &(cs[i]), size);
                        p++;
                    }
                } else {
                    memcpy(&(str[p]), &(cs[i]), size);
                    p++;
                }

            } else {
                if ((i - dem < lg) || str[p - 1].unicode() == '.')
                //                                    if((i-dem<lg))
                {
                    memcpy(&(str[p]), &(cs[i]), size);
                    p++;
                } else {
                    if (!ispace) {
                        if (str[p - 1].unicode() != '-') {
                            memcpy(&(str[p]), &(space), size);
                            p++;
                        }
                    }
                }
            }
            dem = i + 1;
            start = 0;
            ispace = 1;
        }
    }
    str[p] = QChar('\0');
    QString res(str);
    delete[] str;
    return res;
}

Translate::Translate(QObject *parent)
    : QObject{parent}
{
    connect(&net, &QNetworkAccessManager::finished, this, &Translate::onComplete);
}

void Translate::translateToVn(const QString &text)
{
    QString qrs
        = "https://translate.googleapis.com/translate_a/single?client=gtx&sl=auto&tl=vi&dt=t&q=";
    qrs = qrs + cleartext(text).trimmed();
    QNetworkRequest request(qrs);
    this->net.get(request);
}

void Translate::onComplete(QNetworkReply *reply)
{
    QString strReply = (QString) reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray qarr = jsonResponse.array().takeAt(0).toArray();
    QString res = "";
    foreach (const QJsonValue &qa, qarr) {
        const QJsonValue qstr = qa.toArray().at(0);
        res.append(qstr.toString());
    }
    // emit signale
    emit this->translateComplete(res);
}
