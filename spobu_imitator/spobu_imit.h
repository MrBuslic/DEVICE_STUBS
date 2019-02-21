#ifndef SPOBU_IMIT_H
#define SPOBU_IMIT_H
#include "interrupts.h"
#include "mko_imit.h"
#include "omnibus_rpc.h"
#include <QString>
#include <QtWidgets>
#include <QTextEdit>
#include "Integer.h"
#include "CrcUnit.h"
#include <QVector>
#include <QTimer>

struct MOC
{
	QString name;
	int mko;
	int adr;
	int subadr;
	int moc_position;
	int word_count_to_MOC;
	int word_count_to_receive;
	QVariantList not_write_word_RB;
};

class SPOBU_Widget : public QWidget
{
	Q_OBJECT
public:
	SPOBU_Widget();
private:
	QLineEdit* omnibus_edit_ip, *omnibus_edit_port, *foi_edit_ip, *foi_edit_slot_port, *foi_edit_signal_port;
	QTextEdit* edit;
	QLineEdit* edit_settings_file;
	QScrollBar* scroll_bar;
	QPushButton* omnibus_button_stop;
	QPushButton* omnibus_button_restart;
	QPushButton* foi_slot_button_stop;
	QPushButton* foi_slot_button_restart;
	QPushButton* foi_signal_button_stop;
	QPushButton* foi_signal_button_restart;
	QPushButton* choose_file;
	QPushButton* read_file;
	QPushButton* start;
	QPushButton* stop;
	QTimer* main_discret;
	QMutex mutex;
	MKO_IMIT_Object* mko_imit;
	Interrupt_Object* inter_imit;
	
	//Переменные по созданию массива оперативного контроля
	bool is_ready = false;
	bool set_is_done = false;
	QString setting_file;
	int setting_count;
	QVector<MOC> settings;
	QVariantList MOC_data;
	bool ASN_Exchange;

	//Переменные управления основным циклом ЦБК
	int board_time;
	bool bt_pause = true;
	bool wait_info_word = false;
	int buf_size;
	QVariantList words_buffer;

private slots:
	// Функции для кнопок
	void slot_omnibus_button_stop();
	void slot_omnibus_button_restart();
	void sl_foi_slot_button_stop();
	void sl_foi_slot_button_restart();
	void sl_foi_signal_button_stop();
	void sl_foi_signal_button_restart();
	void slot_choose_set_file();
	void read_settings();
	
	//Функции управления основным циклом ЦБК
	void main_cycle();
	void main_programm();
	void stop_main_programm();
	
	//Функции по созданию массива оперативного контроля

	void create_moc();
	QVariantList diapazon(QString str);
	QVariantList read_data(MOC tmp_struct);
	void moc_init();
	int get_asn_data();
	
	//Функции по обработке прерываний
	void handle_interrupt();
	void handle_inter_1();
	void handle_inter_5();
	void handle_inter_13();
	void handle_unknown_inter();

	//Функции КПИ при функционировании СПО-БУ
	

	void emergency_shutdown_mca();
	void pause_BT();
	void resume_BT();
	void perform_CPI();
	void get_cmd();
	void add_data_to_buffer(QVariantList& words);
	void read_ab(QVariantList& words);
	void write_ab();
	void send_receipt(int adr, int subadr, int answer, int& os);
signals:
	void moc_is_ready();
	void signal_send_msg_mko(int mko, int line, int cwd, QVariantList& words, int& os);
	void signal_send_msg_def_mko(int adr, int subadr, int direct, int count_words, QVariantList& words);

};

#endif