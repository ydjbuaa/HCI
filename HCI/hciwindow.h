#ifndef HCIWINDOW_H
#define HCIWINDOW_H

#include <QObject>
#include<qlabel.h>
#include <QtWidgets/QMainWindow>
#include"CrabCut.h"
#include"DataProcess.h"
#include"hcithread.h"
#include"qthread"
#include"visualprocess.h"
class HCIWindow : public QMainWindow
{
	Q_OBJECT
	const int _WINDOW_HEIGHT = 800;
	const int _WINDOW_WINDTH = 1000;
public:
	HCIWindow();
	~HCIWindow();
	//QVector<QPoint*>*fixpoints;
	private slots:
	void setfileloaded();
	void open();
	void save();
	void icut();
	void ishow();  
	void vshow();
	void vcut();
	void about();
	void imageload();
	void videoload();
	void imagecut();
	void nextFrame();
	void showStatues();
	void finishsavefile();
private:
	int fixCount1 = 1;
	int fixCount2 = 1;
	//bool fixfileLoaded;
	//bool isimagecut = false;
	//int workTime = 0;
	//int fileType = 0;
	//int workState = 0;            // 0 not word 1 icut
	int curIntFrame = 0;
	void createActions();
	void createMenus();
	void transMatToQImage(Mat & mat);
	void displayVideo();
	void  disVisualFixVideo(CvCapture * capture);
	void playvideo();
	//void saveFile(QString filename);
	//QVector<QVector<QPoint>*>* fixpoints;
	
	bool isSavingFile = false;

	CvCapture * capture;          //视频和图像显示所需
	Mat   imat;                         //
	Point *lastpoint1;
	Point * lastpoint2;
	IplImage *iplImg;
	IplImage *frame;
	QImage *qImg;
	QTimer *timer;

	//QString curFile;                   //当前文件路径
	//QString curFilePath;            //路径名

	QLabel * label;                    //
	QStatusBar *statusBar;
	QMenuBar * menuBar;
	QWidget * centralWidget;
	QMenu *fileMenu;              //文件操作菜单
	QMenu *editMenu;             //文件编辑菜单
	QMenu *helpMenu;            // 帮助菜单

	//响应动作
	QAction * openAct;             //打开文件操作
	QAction * saveAct;             // 保存文件
	QAction * exitAct;              // 退出
	QAction * icutAct;              //crabcut 提取视点关注 for image
	QAction * vcutAct;             // crabcut 提取视点关注 for video
	QAction * ishowAct;          //图片的可视化显示
	QAction * vshowAct;         //video 的可视化显示
	QAction *aboutAct;        //显示关于眼动仪的消息

protected:
	void paintEvent(QPaintEvent *e);
signals:
	void fileloadsig();
	void savefilesig();
};
//extern HCIWindow * hciwindow;
#endif // HCIWINDOW_H
