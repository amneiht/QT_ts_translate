#include "tsmessage.h"
#include "tscontext.h"
#include "tsdoc.h"
#include "xmlhelper.h"
TsMessage::TsMessage(xmlNodePtr node, const TsDoc *ctx)
{
    con = ctx;
    this->node = node;
    if (ctx)
        QObject::connect(ctx, &TsDoc::sourceChange, this, &TsMessage::onSourceChanged);
    // get qttr

    auto rs = Xmlhelper::getAttributeValue(node, "numerus");
    if (rs == "yes")
        m_type = Type::Numerus;
}

QString TsMessage::getSource() const
{
    QString ret = "";
    if (node) {
        auto source = Xmlhelper::findChildWithName(node, "source");
        if (source)
            ret = (const char *) source->children->content;
    }
    return ret;
}

std::shared_ptr<TsContent> TsMessage::getContent() const
{
    std::shared_ptr<TsContent> ret = nullptr;
    QString sou = getSource();
    (void) sou;
    if (node) {
        auto tran = Xmlhelper::findChildWithName(node, "translation");
        if (tran) {
            if (m_type == Type::String) {
                auto ctx = new TsString(tran);
                ret = std::shared_ptr<TsContent>(ctx);
            } else {
                ret = std::shared_ptr<TsContent>(new TsNumerus(tran));
            }
        }
    }
    return ret;
}

std::shared_ptr<TsMessage> TsMessage::next() const
{
    std::shared_ptr<TsMessage> ret = nullptr;
    auto nptr = node->next;
    while (nptr != nullptr) {
        if (strcmp((const char *) nptr->name, "message") == 0) {
            ret = std::make_shared<TsMessage>(nptr, this->con);
            break;
        }
        nptr = nptr->next;
    }
    return ret;
}

TsMessage::Type TsMessage::getType() const
{
    return m_type;
}

void TsMessage::setFinished(bool finish)
{
    // find atribute
    QString value = "";
    if (!finish) {
        value = "unfinished";
    }
    auto tran = Xmlhelper::findChildWithName(node, "translation");
    if (tran)
        Xmlhelper::setAttribute(tran, "type", value);
}

std::shared_ptr<TsContext> TsMessage::getContext()
{
    std::shared_ptr<TsContext> ret = std::make_shared<TsContext>(node->parent, this->con);
    return ret;
}

void TsMessage::onSourceChanged()
{
    node = nullptr;
    if (con)
        QObject::disconnect(con, &TsDoc::sourceChange, this, &TsMessage::onSourceChanged);
    this->deleteLater();
}
