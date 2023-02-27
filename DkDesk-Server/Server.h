/*****************************************************************//**
 * \file   SMainWindow.h
 * \brief
 *
 * \author Maye - 顽石老师
 * \date   February 2023
 * \other  微信公众号<C语言Plus> 欢迎关注
 *********************************************************************/

#ifndef SERVER_H_
#define SERVER_H_

#include <QTcpServer>
#include <QTcpSocket>

class Wrap
{
public:
	Wrap() :Wrap(0, nullptr) {};
	Wrap(quint32 udid, QTcpSocket* sock) :udid(udid), sock(sock) {}
	bool operator==(qint32 udid) { return this->udid == udid; }
	qint32 udid;
	QTcpSocket* sock;
};

class Server : public QObject
{
	Q_OBJECT
public:
	Server();
	~Server();
	void init();
	bool udidRepeat(qint32 udid)const;
public slots:
	void onNewConnection();
	void onClientReadReady();
	void onClientDisConnected();
private:
	QTcpServer* _tcpServer = nullptr;
	QSet<Wrap*> _wraps;
};

#endif // !SERVER_H_
