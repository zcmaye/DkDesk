///////////////////////////////////////////
//*  @file    eventfilterobject.h
//*  @date    2023-2-22 19:07:17
//*  @brief   过滤器对象(窗口拖动)
//*
//*  @author  顿开教育-顽石老师
///////////////////////////////////////////
#ifndef SCLICKWIDGET_H
#define SCLICKWIDGET_H

#include <QWidget>
#include <QAbstractButton>

class SClickWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SClickWidget(QWidget*parent = nullptr);
    inline void setChecked(bool check) { _checkable = check; };
    inline bool isChecked()const { return _checkable; };
    inline bool checkable() const { return _isChecked; };
    inline void setCheckable(bool able) { _isChecked = able; };
signals:
    void clicked();
    void pressed();
    void released();
protected:
    void mousePressEvent(QMouseEvent*ev)override;
    void mouseReleaseEvent(QMouseEvent* ev)override;
private:
    bool _isDown = false;
    bool _checkable = true;
    bool _isChecked = false;
};

#endif // EVENTFILTEROBJECT_H
