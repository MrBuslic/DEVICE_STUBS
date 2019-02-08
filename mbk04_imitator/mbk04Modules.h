#ifndef MBK04_MODULES_H
#define MBK04_MODULES_H

#include <QString>
#include <QPushButton>
#include <QLayout>
#include <QFileDialog>
#include <QFile>
#include <QThread>
#include <QProgressBar>
#include <QLabel>
#include <QFrame>
#include <QPixmap>
#include <QMainWindow>
#include <QVariant>

enum CURRENT_DEV
{
	MAIN = 1,
	RESERVE = 2,
	OFF = 3
};
enum REZH_FRAME
{
	PI15 = 1,
	PI8 = 2,
	VTF = 4,
	OFF_REZH = 7

};
struct MV_DEV
{
	//double u_kom = 27.0;
	//int dt_kom = 1;
	//int length_kom = 200;
	bool working = true;
};


#endif

