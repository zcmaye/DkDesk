#include "SEventFilterObject.h"
#include<QMouseEvent>
#include<QWidget>
SEventFilterObject::SEventFilterObject(QObject *parent) : QObject(parent)
{
    setDropShadowEffect(true);
}

bool SEventFilterObject::eventFilter(QObject *watched, QEvent *ev)
{
    auto* mouseEv = static_cast<QMouseEvent*>(ev);
    static QPoint dis;
    if(ev->type() == QEvent::MouseButtonPress)
    {
        dis = mouseEv->pos();
    }
    else if(ev->type() == QEvent::MouseMove && mouseEv->buttons()&Qt::LeftButton
            && !dis.isNull())
    {
        auto *pthis = static_cast<QWidget*>(watched);
        //if(pthis->parent()==nullptr)
        pthis->move(mouseEv->globalPosition().toPoint() - dis);
        //pthis->move(pthis->mapToParent(mouseEv->pos()) - dis);
    }
    else if(ev->type() == QEvent::MouseButtonRelease)
    {
        dis = QPoint();
    }
    return false;
}

void SEventFilterObject::setDropShadowEffect(bool enable)
{
    auto parentw = dynamic_cast<QWidget*>(parent());
    if (!parentw)
        return;

    if (enable)
    {
        auto effect = new QGraphicsDropShadowEffect(this);
        effect->setBlurRadius(20);
        effect->setColor(QColor(112, 112, 112));
        effect->setOffset(0);
        parentw->setGraphicsEffect(effect);
    }
    else
    {
        parentw->setGraphicsEffect(nullptr);
    }
}
