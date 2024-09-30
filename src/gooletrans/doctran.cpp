#include "doctran.h"
#include <QDebug>
#include "translate.h"

DocTran::DocTran(QObject *parent)
    : QObject{parent}
{
    tran = std::make_shared<Translate>(this);
    QObject::connect(tran.get(), &Translate::translateComplete, this, &DocTran::onCompleteTran);
}

DocTran::~DocTran()
{
    doc = nullptr;
    cur_mess = nullptr;
}

void DocTran::translateDoc(std::shared_ptr<TsDoc> doc, const QString &path)
{
    this->doc = doc;
    cur_mess = doc->firstContext()->firstMessage();
    if (cur_mess == nullptr)
        return;
    this->save_path = path;
    QString data;
    if (cur_mess->getType() == TsMessage::String) {
        auto m2 = std::static_pointer_cast<TsString>(cur_mess->getContent());
        data = m2->getContent();
    } else {
        auto m2 = std::static_pointer_cast<TsNumerus>(cur_mess->getContent());
        data = m2->getBase();
        base = true;
    }
    this->tran->translateToVn(data);
}

static std::shared_ptr<TsMessage> nextMess(std::shared_ptr<TsMessage> mess)
{
    std::shared_ptr<TsMessage> ret = mess->next();
    if (ret == nullptr) {
        auto par = mess->getContext()->next();
        if (par == nullptr)
            return nullptr;
        ret = par->firstMessage();
    }
    return ret;
}
void DocTran::onCompleteTran(const QString &tran)
{
    QString note = "From En : %1 ; to Vn : %2";
    if (tran == "") {
        goto NEXT;
    }

    if (cur_mess->getType() == TsMessage::String) {
        auto m2 = std::static_pointer_cast<TsString>(cur_mess->getContent());
        qDebug() << note.arg(m2->getContent(), tran);
        m2->setContent(tran);
    } else {
        auto m2 = std::static_pointer_cast<TsNumerus>(cur_mess->getContent());

        if (base) {
            qDebug() << note.arg(m2->getBase(), tran);
            m2->setBase(tran);
            // reset ;
            base = false;
            this->tran->translateToVn(m2->getNumerus());
            return;
        } else {
            qDebug() << note.arg(m2->getNumerus(), tran);
            m2->setNumerus(tran);
        }
    }
NEXT:

    auto n_mess = nextMess(this->cur_mess);
    if (n_mess == nullptr)
        goto END;
    cur_mess = n_mess;
    if (n_mess->getType() == TsMessage::String) {
        auto m2 = std::static_pointer_cast<TsString>(cur_mess->getContent());
        this->tran->translateToVn(m2->getContent());
    } else {
        base = true;
        auto m2 = std::static_pointer_cast<TsNumerus>(cur_mess->getContent());
        this->tran->translateToVn(m2->getBase());
    }
    return;
END:
    qDebug() << "Complete trans: save to " << this->save_path;
    this->doc->save(this->save_path);
    std::exit(0);
    return;
}
