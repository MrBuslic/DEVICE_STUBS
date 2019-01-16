#ifndef TPO_IMIT_H
#define TPO_IMIT_H
#include "mko_imit.h"
#include "interrupts.h"
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
	QTextEdit* edit;
	QScrollBar * scroll_bar;
	QString log_filename;
	QPushButton* write_button;
	QPushButton* read_button;
	MKO_Widget* mko_imit;
	SimInterruptWid* inter_imit;
	bool count_crc(bool flag, int CRC, QVariantList& words); // flag = true - сичтаем контрольную сумму для командного слова. // flag = false - считаем контрольную сумму для слов
public slots:
	void send_request();
	void send_receipt(int answer, QVariantList& ans_word);
	void slot_process_msg(QVariantList& words);
signals:
	void signal_send_message(int adr, int subadr, int direct, int count_words, QVariantList& words);
	void signal_process_msg(QVariantList& words);
	void signal_send_new_msg(int mko, int line, int cwd, QVariantList& words, int& os);
	void signal_send_receipt(int answer, QVariantList& ans_word);
};
#endif