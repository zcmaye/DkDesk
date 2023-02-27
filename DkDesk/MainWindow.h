#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include<QWidget>
#include"ui_MainWindow.h"

class MainWindow : public QWidget
{
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void init();
public:
	void onConnectSucceed();
	void onConnectFailed();
	void onReadReady();
private:
	Ui::mainWindow *ui;
};

#endif // MAINWINDOW_H_
