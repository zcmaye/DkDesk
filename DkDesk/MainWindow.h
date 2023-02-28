#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include<QWidget>
#include"ui_MainWindow.h"
#include<QUdpSocket>
class MainWindow : public QWidget
{
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void init();
	void initUdpSocket();
public:
	void onConnectSucceed();
	void onConnectFailed();
	void onReadReady();
private:
	Ui::mainWindow *ui;

	QUdpSocket* _udpSocket;
	QString _address;	//当前正在远程的伙伴的地址和端口号
	quint16 _port{0};
};

#endif // MAINWINDOW_H_
