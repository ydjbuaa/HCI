#include <QtWidgets/QApplication>
#include"hciwindow.h"
#include"CrabCut.h"
#include"DataProcess.h"
#include"imageprocess.h"
#include"visualprocess.h"
#include"hcithread.h"
#include"qobject.h"
CrabCut * crabcut = new CrabCut();
DataProcess * datapro = new DataProcess();
ImageProcess * imgpro = new ImageProcess();
VisualProcess * visualthread = new VisualProcess();
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QThread thread;
	HCIThread * hcithread = new HCIThread();
	hcithread->moveToThread(&thread);

	HCIWindow * hciwindow = new HCIWindow();
	//load video 
	QObject::connect(hciwindow, SIGNAL(fileloadsig()), hcithread, SLOT(avifileload()));
	//save vide file
	QObject::connect(hciwindow, SIGNAL(savefilesig()), hcithread, SLOT(savevideofile()));
	//finish save file
	QObject::connect(hcithread, SIGNAL( finishsavefilesig()), hciwindow, SLOT(finishsavefile()));

	//启动线程
	thread.start();

	//显示
    hciwindow->show();

	//imgpro->preProcessVideo("E:/Programs/Visual Studio 2013/HCI/data/233");
	//return 0;
	return app.exec();
}
