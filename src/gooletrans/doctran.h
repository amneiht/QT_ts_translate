#ifndef DOCTRAN_H
#define DOCTRAN_H

#include <QObject>
#include <memory.h>
#include <tran/ts.h>

class Translate;

class DocTran : public QObject
{
    Q_OBJECT
public:
    explicit DocTran(QObject *parent = nullptr);
    ~DocTran();

    void translateDoc(std::shared_ptr<TsDoc> doc, const QString &path);

private:
    std::shared_ptr<TsDoc> doc;
    std::shared_ptr<TsMessage> cur_mess = nullptr;
    bool base = false;

    QString save_path;
    std::shared_ptr<Translate> tran;
private slots:
    void onCompleteTran(const QString &tran);
};

#endif // DOCTRAN_H
