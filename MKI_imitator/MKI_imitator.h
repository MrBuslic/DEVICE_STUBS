#ifndef MKI_IMITATOR_H
#define MKI_IMITATOR_H

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
#include <QSettings>
#include <QVector>
#include "KPAdefines.h"
#include "../buses_imitator/frame_bus_rpc.h"
#include "rpc_ports.h"

/**
*	\class таймер
*
*/
class MyTimer
{
public:
	/**
	*	\brief конструктор
	*
	*/
	MyTimer() : CounterStart(0)
	{
		StartCounter();
		Sleep(1);
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		Freq_In_mSec = li.QuadPart - CounterStart;
	}
	/**
	*	\brief запуск счетчика
	*
	*/
	bool StartCounter()
	{
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
			return false;

		QueryPerformanceCounter(&li);
		CounterStart = li.QuadPart;
		return true;
	}
	/**
	*	\brief остановка счетчика
	*
	*/
	void StopCounter()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		CounterStop = li.QuadPart;
	}
	/**
	*	\brief получить секунды
	*
	*/
	long GetSec()
	{
		return static_cast<long>(
			std::ceil(double(CounterStop - CounterStart) / (Freq_In_mSec * 1000.0))
			);
	}
	/**
	*	\brief получить секунды
	*
	*/
	long GetUSec()
	{
		return static_cast<long>((CounterStop - CounterStart) * 1000 / Freq_In_mSec);
	}
private:
	__int64 CounterStart;
	__int64 CounterStop;
	__int64 Freq_In_mSec;
};

class MKI_imitator :
	public QMainWindow
{
	Q_OBJECT
public:
	MKI_imitator();
	~MKI_imitator();
	
private:
	QWidget* widg;
	
	QTcpServer *tcpServer;
	QTcpSocket* clientSocket;

	QLabel *control_label;
	QLabel *rezhim_label;
	QLabel *ip_label;
	QLabel *RPIK_label;	// Режим Приёма Кадра

	QTextEdit *log_edit;
	QScrollBar* _scroll_bar;

	QGridLayout *gridLayout;
	
	RPC_frame_bus_SLOT_Thread frame_slot_thr;
	RPC_frame_bus_SIGNAL_Thread frame_signal_thr;

	void log_msg(QString msg);
	QTcpSocket sock;

	//const char *control_names[] = { "МУ", "ЦУ"};
	const QVector<QString> control_names = {"", "МУ", "ЦУ" };
	const QVector<QString> rezhim_names = { "", "РР", "ПБР", "БР" };
	const QVector<QString> RPIK_names = { "выкл", "ВТФ", "ИК8", "ИК15" };

	unsigned int RUValue = 0;
	unsigned int RRValue = 0;
	unsigned int RPIKValue = 0;
	QString ipValue;
	
	const int msgHeaderLen = 96;
	const int header_addr_receiver_ind = 4;
	const int header_addr_sender_ind = 8;
	const int header_MsgTypeC_ind = 12;
	const int header_msg_reg_n_ind = 16;
	const int header_sec_time_ind = 20;
	const int header_usec_time_ind = 24;
	const int header_krp_timeout_ind = 28;
	const int header_kro_timeout_ind = 32;
	const int header_err_stat_ind = 40;
	const int header_LinkID_ind = 48;
	const int header_linkStat_ind = 52;
	const int header_datasize_ind = 56;
	const int header_RU = 60;
	const int header_RR = 64;
	const int header_count_abonents_MTUA = 68;
	const int header_count_abonents_MVI = 72;
	const int header_ust_rezh_PKU = 76;
	const int header_RPIK = 80;
	const int header_number_KA = 84;
	const int header_undefined = 88;
	const int header_crc_header_ind = 92;
	int MsgRegN = 0;
	MyTimer my_timer;

	QByteArray createCommandHeader(const int& command, const int& dataSize, int id = 0);
	void sendKRO(int MsgTypeC);
	unsigned long sp_getCRC32(unsigned char *buf, unsigned long len);

	template <typename T>
	/**
	*	\brief целое в БА
	* \param ba - массив БА
	* \param data - данные
	* \param start_ind - начальный индекс
	*/
	static void numberIntoBA(QByteArray& ba, T data, const int& start_ind)
	{
		int count = sizeof(T);

		if (start_ind + count > ba.size())
			ba.resize(start_ind + count);

		for (int i = 0; i < count; ++i)
		{
			ba[start_ind + i] = (unsigned char)(data);
			data = data >> 8;
		}
	}

	template <typename T>
	/**
	*	\brief номер из БА
	* \param ba - массив БА
	* \param start_ind - начальный индекс
	* \param bytes_count - количество байт
	*/
	static T numberFromBA(const QByteArray& ba, const int& start_ind, const int bytes_count = 0)
	{
		QDataStream tmp(ba);
		tmp.setByteOrder(QDataStream::LittleEndian);
		tmp.skipRawData(start_ind);

		T data = 0;
		tmp >> data;
		return data;
	}

protected:
	void closeEvent(QCloseEvent *event);
signals:
	void make_new_frame_rm07(QString mode, QVariant frame_data);

public slots :
	void error_Slot(QAbstractSocket::SocketError socketError);
	void read();
	void new_frame_07(QString mode_in, int psp_in, int lit_in, int _fm, QString _ant, QVariant frame_data);
	void newConn();
	void slotReadClient();
};

#endif

