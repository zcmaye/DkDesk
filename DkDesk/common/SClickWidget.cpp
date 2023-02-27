#include "SClickWidget.h"
#include<QMouseEvent>

SClickWidget::SClickWidget(QWidget*parent)
    : QWidget(parent)
{
    
}


void SClickWidget::mousePressEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        emit pressed();
        _isDown = true;
    }
}

void SClickWidget::mouseReleaseEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        emit released();
        if (_isDown)
            emit clicked();
        _isDown = false;
    }
}

