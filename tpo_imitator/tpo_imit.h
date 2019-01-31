#ifndef TPO_IMIT_H
#define TPO_IMIT_H
#include "mko_imit.h"
#include "interrupts.h"
#include "VIP_imit.h"
#include "omnibus_rpc.h"
#include <QString>
#include <QtWidgets>
#include <QTextEdit>
#include "Integer.h"
#include "CrcUnit.h"

class TPO_Widget : public QWidget
{
	Q_OBJECT
public:
	TPO_Widget();
private:
	QLineEdit*  mds_32_edit_ip, *mds_32_edit_port;
	QLineEdit*  mfsk24_edit_ip, *mfsk24_edit_port;
	QLineEdit*  foi_edit_ip, *foi_signal_port, *foi_slot_port;
	QLineEdit* omnibus_edit_ip, *omnibus_edit_port;
	QTextEdit* edit;
	QScrollBar * scroll_bar;
	QString log_filename;
	QPushButton* mds32_button;
	QPushButton* mfsk24_button;
	QPushButton* foi_button;
	QPushButton* omnibus_button;
	MKO_Widget* mko_imit;
	SimInterruptWid* inter_imit;
	mds32_exchange* mds32_imit;
	mfsk24_exchange* mfsk24_imit;
	void write_data(QVariantList& words);
	void read_data(QVariantList& words);
	void unknown_com();
	int count_crc_for_cwd(QVariantList& words);
	int count_crc_for_words(QVariantList& words);
	int count_byte_for_words(QVariantList& words);
	void miss_receipt(int& os);
	bool pitanie[5];
	void vip_init();
public slots:
	void send_request();
	void send_receipt(int answer, int& os);
	void send_read_receipt(int answer, int byte_count, int crc_for_words, int& os);
	void slot_process_msg(QVariantList& words);
	void slot_mds32_button();
	void slot_mfsk24_button();
	void mds32_send_sample(int channel, uint& buf, int& flag);
	void slot_mfsk24_impulse_change(int channel, int duration);
	//void slot_foi_button();
	//void slot_omnibus_button();
signals:
	void signal_send_message(int adr, int subadr, int direct, int count_words, QVariantList& words);
	void signal_process_msg(QVariantList& words);
	void signal_send_new_msg(int mko, int line, int cwd, QVariantList& words, int& os);
	void signal_send_receipt(int answer, int& os);
	void signal_send_read_receipt(int answer, int byte_count, int crc_for_words, int& os);
};
#endif