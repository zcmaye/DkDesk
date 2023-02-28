#include "MainWindow.h"

#include "common/SEventFilterObject.h"
#include "common/SUDID.h"
#include "common/SJsonData.h"

#include "network/SMessageState.h"
#include "network/SNetworkConnect.h"
//#include "network/AcceptBuddyConnection.h"

#include <QButtonGroup>
#include <QSettings>
#include <QClipBoard>
#include <QApplication>
#include <QTcpServer>
#include <QHostInfo>
#include <QNetworkInformation>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <QNetworkDatagram>

MainWindow::MainWindow(QWidget* parent)
	:QWidget(parent)
	, ui(new Ui::mainWindow)
{
	ui->setupUi(this);
	this->init();
	initUdpSocket();
	//quint32 ip4addr;
	//auto addr_list =  QNetworkInterface::allAddresses();
	//for (auto addr : addr_list)
	//{
	//	bool ok = false;
	//	ip4addr = addr.toIPv4Address(&ok)<<ok;
	//	if (ok)
	//	{
	//		break;
	//	}
	//}
	
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::init()
{
	srand(time(nullptr));
	setWindowFlag(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	installEventFilter(new SEventFilterObject(this));

	ui->remoteBtn->setChecked(true);
	ui->buddyConnectBtn->setEnabled(false);

	//关联信号和槽
	//titleBar
	connect(ui->closeBtn, &QPushButton::released, this, &MainWindow::close);
	connect(ui->minBtn, &QPushButton::released, this, &MainWindow::showMinimized);
	connect(ui->moreBtn, &QPushButton::released, this, [=]()
		{
			auto len =_udpSocket->writeDatagram("Hello!,hahaha!", QHostAddress(_address), _port);
			qInfo() << "send succeed:"<< len << _address<<"    "<<ui->localUDIDEdit->text();
		});
	//leftMenuWidget
	connect(ui->userInfoWidget, &SClickWidget::released, this, [=]
		{
			ui->deviceBtn->setChecked(true);
			ui->stackedWidget->setCurrentIndex(0);
		});
	connect(ui->remoteBtn, &QPushButton::released, this, [=] {ui->stackedWidget->setCurrentIndex(1); });
	connect(ui->deviceBtn, &QPushButton::released, this, [=] {ui->stackedWidget->setCurrentIndex(2); });
	//remotePage
	//--复制本机识别码
	connect(ui->localUDIDCopyBtn, &QPushButton::released, this, [=]()
		{
			auto clipBoard = qApp->clipboard();
			QString str = "我的识别码：" + ui->localUDIDEdit->text() + "\n";
			str += "使用顿开教育远程控制即可对我发起远程协助~\n顽石老师";
			clipBoard->setText(str);
		});
	//--查看验证码
	connect(ui->authCodeSeeBtn, &QPushButton::toggled, this, [=](bool chk)
		{
			if (chk)
			{
				ui->authCodeEdit->setText("------");
			}
			else
			{
				QSettings settings("DkDesk.ini", QSettings::Format::IniFormat);
				ui->authCodeEdit->setText(settings.value("authCode").toString());
			}
		});
	//--编辑验证码
	connect(ui->authCodeModifyBtn, &QPushButton::released, this, [=]()
		{
			if (ui->authCodeEdit->isReadOnly())
			{
				ui->authCodeEdit->setReadOnly(false);
				ui->authCodeEdit->setFocus();
			}
			else
			{
				//ui->authCodeEdit->clearFocus();
				ui->authCodeEdit->setReadOnly(true);
				
			}
			

		});
	//--验证码框丢失焦点
	connect(ui->authCodeEdit, &QLineEdit::editingFinished, this, [=]
		{
			ui->authCodeEdit->setReadOnly(true);
		});

	//连接伙伴
	connect(ui->buddyUDIDEdit, &QLineEdit::textChanged, this, [=](const QString& text)
		{
			ui->buddyConnectBtn->setEnabled(!text.isEmpty());
		});
	connect(ui->buddyConnectBtn, &QPushButton::released, this, [=]
		{
			if (ui->buddyUDIDEdit->text().toInt() < 100'000'000)
			{
				qInfo() << "请输入正确的识别码";
				return;
			}
			SJsonData jd;
			jd.addValue("type", MsgType::Request_Connect);
			jd.addValue("toUdid", ui->buddyUDIDEdit->text().toInt());
			jd.addValue("fromUdid", ui->localUDIDEdit->text().toInt());
			jd.addValue("port", _udpSocket->localPort());
			SNetworkConnect::mainCon()->write(jd.toJson().data());
			qInfo() << _udpSocket->localPort();
		});


	//链接服务器是否成功
	auto sock = SNetworkConnect::mainCon();
	connect(sock, &SNetworkConnect::connectSucceed, this, &MainWindow::onConnectSucceed);
	connect(sock, &SNetworkConnect::connectFailed, this, &MainWindow::onConnectFailed);
	connect(sock, &SNetworkConnect::readReady, this, &MainWindow::onReadReady);
}

void MainWindow::initUdpSocket()
{
	_udpSocket = new QUdpSocket(this);

	connect(_udpSocket, &QUdpSocket::errorOccurred, [=]()
		{
			qWarning() << "UDP errorOccurred" << _udpSocket->errorString();
		});
	connect(_udpSocket, &QUdpSocket::readyRead,
		this, [=]()
		{
			while (_udpSocket->hasPendingDatagrams())
			{
				auto datagram = _udpSocket->receiveDatagram();
				qInfo() << "has datagram " << datagram.data()<<datagram.senderAddress()<<datagram.senderPort();
				_port = datagram.senderPort();
			}
			
		});

	if (!_udpSocket->bind(QHostAddress::AnyIPv4))
	{
		qWarning() << "UDP bind failed" << _udpSocket->errorString();
	}
	else
	{
		qWarning() << "UDP bind succeed"<< _udpSocket->localAddress()<<_udpSocket->localPort();
	}

}

void MainWindow::onConnectSucceed()
{
	ui->statusLab->setText("服务器连接成功");

	QSettings settings("DkDesk.ini", QSettings::Format::IniFormat);
	//请求UDID,如果配置文件保存了udid，则取出来，没有则创建一个新的
	auto udid = settings.value("udid", (qint32)SUDID::createUDID()).toInt();
	//qInfo() << id;
	//生成json数据
	SJsonData jd;
	jd.addValue("type", (qint32)MsgType::UdidCreated);
	jd.addValue("udid", udid);
	//发送json数据
	SNetworkConnect::mainCon()->write(jd.toJson().data());
}

void MainWindow::onConnectFailed()
{
	ui->statusLab->setText("服务器连接失败");
	ui->localUDIDEdit->setText("--- --- ---");
	ui->authCodeEdit->setText("------");
}

void  MainWindow::onReadReady()
{
	auto data = SNetworkConnect::mainCon()->readAll();
	qInfo() << data;
	SJsonData jd = SJsonData::fromString(data.data());
	switch ((MsgType)jd.intValue("type"))
	{
	case MsgType::UdidCreated:
		if (jd.intValue("state") == MsgState::Ok)
		{
			ui->localUDIDEdit->setText(QString::number(jd.intValue("udid")));
			ui->authCodeEdit->setText("Sd67S");

			QSettings settings("DkDesk.ini",QSettings::Format::IniFormat);
			settings.setValue("udid", jd.intValue("udid"));
			settings.setValue("authCode", ui->authCodeEdit->text());
		}
		break;
	case MsgType::UserLogin:
		break;
	case MsgType::UserRegister:
		break;
	case MsgType::Request_Connect:
	{
		qInfo() << "Request_Connect";
		//同意连接
		if (1)
		{		
			//开始监听TCP
			/*{
				QTcpServer* server = new QTcpServer(this);
				if (!server->listen(QHostAddress::AnyIPv4))
				{
					qInfo() << "listen failed" << server->errorString();
				}
				else
				{
					qInfo() << "listen succeed" << server->serverAddress() << server->serverPort() << server->serverError();
					connect(server, &QTcpServer::newConnection, this, []()
						{
							qInfo() << "和伙伴连接成功~~";
						});
				}
			}
			*/

			//{
			//	
			//	auto addr_list = QNetworkInterface::allAddresses();
			//	for (auto addr : addr_list)
			//	{
			//		bool ok = false;
			//		 addr.toIPv4Address(&ok) << ok;
			//		if (ok)
			//		{
			//			jd.addValue("address", addr.toString().toStdString());
			//			break;
			//		}
			//	}
			//
			//}
			
			{
				//读取对方的IP地址和端口号
				auto address = jd.stringValue("address");
				quint16 port = jd.numberValue("port");
				//写入自己端口号
				//jd.addValue("address", _udpSocket->());
				jd.addValue("port", _udpSocket->localPort());
				qInfo() <<"self port" << _udpSocket->localPort();
				_udpSocket->writeDatagram("Hello!,I Consent connection!", QHostAddress(address.data()), port);
				qInfo() << "Request_Connect list start" << address.data() << port;
				_address = address.data();
			}

			jd.addValue("type", MsgType::Ready_Connect);
			SNetworkConnect::mainCon()->write(jd.toJson().data());
		}
		//拒绝连接
		else
		{

		}

		break;
	}
	case MsgType::Ready_Connect:
	{
		qInfo() << "Ready_Connect";

		/*QTcpSocket* newSock = new QTcpSocket(this);

		connect(newSock, &QTcpSocket::connected, [=]()
			{
				qInfo() << "连接到伙伴非常的成功哦！"<<newSock->peerAddress();
			});
		connect(newSock, &QTcpSocket::errorOccurred, [=]()
			{
				qInfo() << "连接到伙伴非常的失败了哦！"<<newSock->errorString();
			});
		newSock->connectToHost(address.data(), port);
		*/
		//开始监听UDP
		{
			//拿到伙伴的IP地址和端口号
			auto address = jd.stringValue("address");
			auto port = jd.numberValue("port");
			_udpSocket->writeDatagram("Hello!you Consent connection,I'm very happy!", QHostAddress(address.data()), port);
			qInfo() << "Ready_Connect list start" << address.data() << port;
			_address = address.data();
		}
		break;
	}
	case MsgType::ConnectBuddy:
	{
		auto state = jd.intValue("state");
		if (state == MsgState::Ok)
		{
			
			auto address = jd.stringValue("address");
			auto port = (quint16)jd.numberValue("port");
			qInfo() << "准备连接" << address.data() << port;
			//auto con = new SNetworkConnect(this);
			//con->connectToHost(address.data(), port);
		}
		else if (state == MsgState::NotFound)
		{
			qInfo() << "设备不存在或不在线" << jd.intValue("toUdid");
		}

	}
		break;
	default:
		break;
	}


}
/*https://evilpan.com/2015/10/31/p2p-over-middle-box/*/