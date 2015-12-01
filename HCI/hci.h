#ifndef HCI_H
#define HCI_H

#include <QtWidgets/QMainWindow>
#include "ui_hci.h"

class HCI : public QMainWindow
{
	Q_OBJECT

public:
	HCI(QWidget *parent = 0);
	~HCI();

private:
	Ui::HCIClass ui;
};

#endif // HCI_H
