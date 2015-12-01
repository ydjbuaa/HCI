#ifndef HCITHREAD_H
#define HCITHREAD_H

#include <QObject>
#include"visualprocess.h"
class HCIThread : public QObject
{
	Q_OBJECT

	
public:
	//bool fixfileloaded;
	HCIThread();
	~HCIThread();
	//void setCurFilePath(QString filepath);

	private slots:
	void avifileload(); 
	void savevideofile();
signals :
	void fileloaded();
	void finishsavefilesig();
private :
	bool saveVisualVideoFile();
	bool saveCutVideoFile();
	Mat cutTheImage(Mat &, QVector<Point*>*);
	
};

#endif // HCITHREAD_H
