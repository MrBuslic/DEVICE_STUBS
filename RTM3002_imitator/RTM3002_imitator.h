#ifndef RTM3002_IMITATOR_H
#define RTM3002_IMITATOR_H

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QTextEdit>
#include <QScrollBar>
#include <QList>
#include <QMessageBox>
#include <QUDPSocket>
#include <QTCPSocket>
#include <QTcpServer>

#include <boost/range/combine.hpp>
#include <boost/tuple/tuple.hpp>
#include <QGridLayout>

class RTM3002_imitator :
	public QMainWindow
{
	Q_OBJECT
public:
	RTM3002_imitator();
	~RTM3002_imitator();
	

	double GetDELay() const;
	void SetDELay();
	double GetPositivePeak() const;
	void SetPositivePeak();
	int GetWCO() const;
	void SetWCO();
	int GetMKO_DATA() const;
	void SetMKO_DATA();
	double GetPEAK() const;
	void SetPEAK();
	double GetCURS1_VAL() const;
	void SetCURS1_VAL();	
	double GetCURS2_VAL() const;
	void SetCURS2_VAL();
private:
	QWidget* widg;
	
	QUdpSocket _sock;
	QTcpSocket* _ag_sock = nullptr;
	QTcpServer serv_sock;

	QGridLayout *gridlay;

	QLabel DELay_label;
	QTextEdit *DELay_edit;	//Значение задержки между включениями источников питания СИБП 
	double DELay;

	QLabel PositivePeak_label;
	QTextEdit *PositivePeak_edit; // Пульсация напряжения помехи
	double PositivePeak;

	QLabel WCO_label;
	QTextEdit *WCO_edit; // Количество слов на МКО
	int WCO;

	QLabel MKO_DATA_label;
	QTextEdit *MKO_DATA_edit; // Слова на МКО
	int MKO_DATA;

	QLabel PEAK_label;
	QTextEdit *PEAK_edit; // Значение напряжения на канале D1
	double PEAK;

	QLabel CURS1_VAL_label;
	QTextEdit *CURS1_VAL_edit; // Разница во времени между двумя вертикальными линиями курсора 1
	double CURS1_VAL;

	QLabel CURS2_VAL_label;
	QTextEdit *CURS2_VAL_edit; // Разница во времени между двумя вертикальными линиями курсора 2
	double CURS2_VAL;


protected:
	void closeEvent(QCloseEvent *event);


signals:


public slots:
	void error_Slot(QAbstractSocket::SocketError socketError);
	void read();
	void connect_ag();
	void read_ag();


};

#endif

