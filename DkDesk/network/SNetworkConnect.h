///////////////////////////////////////////
//*  @file   SNetworkConnect.h
//*  @date    2023-2-22 21:07:18
//*  @brief   网络连接，连接服务器的
//*
//*  @author  顿开教育-顽石老师
///////////////////////////////////////////
#ifndef SNETWORKCONNECT_H
#define SNETWORKCONNECT_H

#include<QTcpSocket>

class SNetworkConnect : public QObject
{
	Q_OBJECT
public:
	explicit SNetworkConnect(QObject* parent = nullptr);
	~SNetworkConnect();

	void connectToHost(const QString& adress, quint16 port);
	qint64 write(const char* data);
	qint64 write(const char* data, qint64 maxSize);
	qint64 read(char* data, qint64 maxSize);
	QByteArray readAll();


	static SNetworkConnect* mainCon();
public slots:
	void onError(QAbstractSocket::SocketError socketError);
signals:
	void connectSucceed();  //连接成功
	void connectFailed();  //连接成功
	void disConnected();    //断开连接
	void readReady();
private:
	QTcpSocket* _tcpSocket = nullptr;
};

#endif // SNETWORKCONNECT_H
