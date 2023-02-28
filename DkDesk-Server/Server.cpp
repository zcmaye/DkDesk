#include "Server.h"
#include <QtEndian>
#include "common/SJsonData.h"
#include "SMessageState.h"
Server::Server()
	:_tcpServer(new QTcpServer)
{
	this->init();
}

Server::~Server()
{
	for (auto s : _wraps)
	{
		s->sock->write("Byte~");
		s->sock->close();
	}
	_tcpServer->close();
	delete _tcpServer;
}

void Server::init()
{
	//开启服务器
	if (!_tcpServer->listen(QHostAddress::AnyIPv4, 5678))
	{
		qInfo() <<"listen failed" << _tcpServer->errorString();
	}
	//关联信号和槽
	QObject::connect(_tcpServer, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

bool Server::udidRepeat(qint32 udid) const
{
	for (auto w : _wraps)
	{
		if (w->udid == udid)
		{
			return true;
		}
	}
	return false;
}

void  Server::onNewConnection()
{
	if (_tcpServer->hasPendingConnections())
	{
		auto sock = _tcpServer->nextPendingConnection();
		_wraps.insert(new Wrap(0,sock));
		qInfo() << __FUNCSIG__<<sock;
		//关联信号
		QObject::connect(sock, &QTcpSocket::readyRead, this, &Server::onClientReadReady);
		QObject::connect(sock, &QTcpSocket::disconnected, this, &Server::onClientDisConnected);
	
	}
}

void  Server::onClientReadReady()
{
	int i = 0;
	for (auto w : _wraps)
	{
		qInfo() << "udids[" << i++ << "]" << w->udid;
	}

	auto sock = dynamic_cast<QTcpSocket*>(sender());
	if (sock)
	{
		auto byte = sock->readAll();
		SJsonData jd = SJsonData::fromString(byte.data());
		switch ((MsgType)jd.numberValue("type"))
		{
		case MsgType::UdidCreated:
		{
			qint32 id = jd.numberValue("udid");
			//如果udid和已经存在的重复了 +1之后返回给客户端
			if (udidRepeat(id))
			{
				id += 1;
				jd.addValue("udid", id);
			}

			jd.addValue("state", MsgState::Ok);
			sock->write(jd.toJson().data());
			//找到socket对应的wrap，把id存储进去
			for (auto w : _wraps)
			{
				if (w->sock == sock)
				{
					w->udid = id;
				}
			}
			break;
		}
		case MsgType::UserLogin:
			break;
		case MsgType::UserRegister:
			break;
		case MsgType::Request_Connect:
		{
			qInfo() << "beg Request_Connect" << jd.toJson().data();
			//查找要连接的伙伴是否存在
			auto it = std::find_if(_wraps.begin(), _wraps.end(), [&](Wrap* w)
				{
					return w->udid == (qint32)jd.numberValue("toUdid");
				});
			if (it != _wraps.end())
			{
				jd.addValue("state", MsgState::Ok);
				//拿到伙伴的IP地址,端口号已经由客户端传递过来了
				jd.addValue("address", sock->peerAddress().toString().toStdString());

				//询问伙伴是否接受连接
				(*it)->sock->write(jd.toJson().data());
			}
			else//设备不存在或不在线
			{
				jd.addValue("state", MsgState::NotFound);
				sock->write(jd.toJson().data());
			}
			qInfo() << "end Request_Connect"<<jd.toJson().data();
			break;
		}
		case MsgType::Ready_Connect:
		{
			qInfo() << "beg Ready_Connect" << jd.toJson().data();
			auto it = std::find_if(_wraps.begin(), _wraps.end(), [&](Wrap* w)
				{
					return w->udid == (qint32)jd.numberValue("fromUdid");
				});
			if (it != _wraps.end())
			{
				jd.addValue("address", sock->peerAddress().toString().toStdString());	//获取客户端的公网ip地址
				//jd.addValue("port", jd.numberValue("port"));	//拿到客户端发过来的端口号，转发给请求连接的客户端
				(*it)->sock->write(jd.toJson().data());
			}
			qInfo() << "end Ready_Connect" << jd.toJson().data();
		}
			break;
		default:
			break;
		}
	}
	
}

void Server::onClientDisConnected()
{
	auto sock = dynamic_cast<QTcpSocket*>(sender());
	for (auto w : _wraps)
	{
		if (sock == w->sock)
		{
			_wraps.remove(w);
		}
	}
	qInfo() << __FUNCSIG__;
}
