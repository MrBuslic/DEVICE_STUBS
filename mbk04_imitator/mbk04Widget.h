#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

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



class MainWidget :
	public QWidget
{
	Q_OBJECT
public:
	MainWidget();
	~MainWidget();
	
private:
	QWidget* widg;
	
	QMainWindow* main_widg;
	
	QPushButton* o_rez_btn;
	
	QPushButton* r_rez_btn;
	
	QPushButton* ik15_btn;
	
	QPushButton* ik8_btn;
	
	QPushButton* vtf_btn;
	
	
public slots:
	
	
};

#endif

