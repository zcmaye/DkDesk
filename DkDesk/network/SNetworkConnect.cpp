#include "SNetworkConnect.h"
#include"SNetworkConnect.h"

SNetworkConnect::SNetworkConnect(QObject* parent)
	:QObject(parent)
	,_tcpSocket(new QTcpSocket)
{
	QObject::connect(this, &SNetworkConnect::connectSucceed, []()
		{
			qInfo() << "connectSucceed~";
		});
	QObject::connect(this, &SNetworkConnect::disConnected, []()
		{
			qInfo() << "Server quit~";
		});
	QObject::connect(_tcpSocket, &QTcpSocket::errorOccurred, [=]()
		{
			qInfo() << "connect error"<<_tcpSocket->errorString();
		});
}

SNetworkConnect::~SNetworkConnect()
{
	_tcpSocket->close();
	delete _tcpSocket;
}

void SNetworkConnect::connectToHost(const QString& address, quint16 port)
{
	_tcpSocket->connectToHost(address, port);
	//转发连接成功信号
	connect(_tcpSocket, &QTcpSocket::connected, this, &SNetworkConnect::connectSucceed);
	//连接断开
	connect(_tcpSocket, &QTcpSocket::disconnected, this, &SNetworkConnect::disConnected);
	//有错误
	connect(_tcpSocket, &QTcpSocket::errorOccurred, this, &SNetworkConnect::onError);
	//有消息
	connect(_tcpSocket, &QTcpSocket::readyRead, this, &SNetworkConnect::readReady);
	
}

qint64 SNetworkConnect::write(const char* data)
{
	return _tcpSocket->write(data);
}

qint64 SNetworkConnect::write(const char* data, qint64 maxSize)
{
	return _tcpSocket->write(data,maxSize);
}

qint64 SNetworkConnect::read(char* data, qint64 maxSize)
{
	return _tcpSocket->read(data,maxSize);
}

QByteArray SNetworkConnect::readAll()
{
	return _tcpSocket->readAll();
}

SNetworkConnect* SNetworkConnect::mainCon()
{
	static auto con = new SNetworkConnect;
	if (con->_tcpSocket->state() == QTcpSocket::SocketState::UnconnectedState)
	{
		//con->connectToHost("192.168.1.3", 5678);
		con->connectToHost("106.52.68.188", 5678);
		//QObject::connect(con, &SNetworkConnect::connectSucceed, []()
		//	{
		//		qInfo() << "connectSucceed~";
		//	});
		//QObject::connect(con, &SNetworkConnect::disConnected, []()
		//	{
		//		qInfo() << "Server quit~";
		//	});
	}
	return con;
}

void SNetworkConnect::onError(QAbstractSocket::SocketError socketError)
{
	switch (socketError)
	{
	case QAbstractSocket::ConnectionRefusedError:
		break;
	case QAbstractSocket::RemoteHostClosedError:
		break;
	case QAbstractSocket::HostNotFoundError:
		break;
	case QAbstractSocket::SocketAccessError:
		break;
	case QAbstractSocket::SocketResourceError:
		break;
	case QAbstractSocket::SocketTimeoutError:
		break;
	case QAbstractSocket::DatagramTooLargeError:
		break;
	case QAbstractSocket::NetworkError:
		break;
	case QAbstractSocket::AddressInUseError:
		break;
	case QAbstractSocket::SocketAddressNotAvailableError:
		break;
	case QAbstractSocket::UnsupportedSocketOperationError:
		break;
	case QAbstractSocket::UnfinishedSocketOperationError:
		break;
	case QAbstractSocket::ProxyAuthenticationRequiredError:
		break;
	case QAbstractSocket::SslHandshakeFailedError:
		break;
	case QAbstractSocket::ProxyConnectionRefusedError:
		break;
	case QAbstractSocket::ProxyConnectionClosedError:
		break;
	case QAbstractSocket::ProxyConnectionTimeoutError:
		break;
	case QAbstractSocket::ProxyNotFoundError:
		break;
	case QAbstractSocket::ProxyProtocolError:
		break;
	case QAbstractSocket::OperationError:
		break;
	case QAbstractSocket::SslInternalError:
		break;
	case QAbstractSocket::SslInvalidUserDataError:
		break;
	case QAbstractSocket::TemporaryError:
		break;
	case QAbstractSocket::UnknownSocketError:
		break;
	default:
		break;
	}
	emit connectFailed();
	qWarning() << "has error" << _tcpSocket->errorString();
}
