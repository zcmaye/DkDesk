#include "AcceptBuddyConnection.h"
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
	_tcpServer->listen(QHostAddress::AnyIPv4,5678);
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
	for (auto w : _wraps)
	{
		qInfo() << w->udid;
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
		case MsgType::ConnectBuddy:
		{
			//查找要连接的伙伴是否存在
			auto it = std::find_if(_wraps.begin(), _wraps.end(), [&](Wrap* w)
				{
					return w->udid == (qint32)jd.numberValue("toUdid");
				});
			if (it != _wraps.end())
			{
				jd.addValue("state", MsgState::Ok);
				//拿到伙伴的IP地址和端口号
				jd.addValue("address", (*it)->sock->peerAddress().toString().toStdString());
				jd.addValue("port", (*it)->sock->peerPort());

			}
			else//设备不存在或不在线
			{
				jd.addValue("state", MsgState::NotFound);

			}
			sock->write(jd.toJson().data());
			break;
		}
		default:
			break;
		}

		qInfo() << __FUNCSIG__<<byte;

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
