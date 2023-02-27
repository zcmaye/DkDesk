#include<QApplication>
#include"MainWindow.h"
#include"common/SJsonData.h"
#include"network/SNetworkConnect.h"
#include"common/SUDID.h"
#include <QUUID>
int main(int argc,char* argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}

/*
B1B1B1
FF8D6B
*/
