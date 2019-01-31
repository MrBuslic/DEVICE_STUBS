#ifndef BKUPI_H
#define BKUPI_H

#include <QDialog>
#include <QMap>
#include <QXmlStreamReader>
#include "ui_bkupi.h"
#include "../../../main_server/main_server_rpc.h"
//namespace Ui {
//	class bkupi_widg;
//}

struct OK_DATA
{
	int mkoAddr;
	int mkoSaddr;
	int mkoWord;
	int mkoBitStart;
	int mkoBitCount;
};

struct AB_data
{
	AB_data(int addr, QList<int> saddrs);
	QMap<int, QList<unsigned short>> words;
	unsigned short os;

	AB_data(){}
};

class BKUPI_widg : public QDialog
{
    Q_OBJECT

public:
	explicit BKUPI_widg(QWidget *parent = 0);
	~BKUPI_widg();
public slots:
	void vchm1_clicked();
	void vchm2_clicked();
	void vchm3_clicked();
	void vchm4_clicked();
	void _733_upi_clicked();
	void _732_vchm1_clicked();
	void _732_vchm2_clicked();
	void _732_vchm3_clicked();
	void _732_vchm4_clicked();
	//void cbk1on_clicked();
	//void cbk2on_clicked();
	//void cbk3on_clicked();
	//void cbk4on_clicked();
	//void cbk1off_clicked();
	//void cbk2off_clicked();
	//void cbk3off_clicked();
	//void cbk4off_clicked();
	//void cbk1bc_clicked();
	//void cbk2bc_clicked();
	//void cbk3bc_clicked();
	//void cbk4bc_clicked();
	//void cbk1wr_clicked();
	//void cbk2wr_clicked();
	//void cbk3wr_clicked();
	//void cbk4wr_clicked();
	void mbk07_fsmy(const QString& new_text);
	void mbk07_fsvy(const QString& new_text);
	void mbk07_lt(const QString& new_text);
	void mbk07_mode(const QString& new_text);
	void mbk07_psp(const QString& new_text);
	void mbk07_ant(const QString& new_text);
	void mbk07_stab(const QString& new_text);
	void mbk02_osn(const QString& new_text);
	void mbk02_lit(const QString& new_text);
	void mbk02_ant(const QString& new_text);
	void mbk02_vcs(const QString& new_text);
	void mbk04c(const QString& new_text);
	void mbk04m(const QString& new_text);
	void og(const QString& new_text);
	void finik(const QString& new_text);
	void lka(const QString& new_text);
	void kp(const QString& new_text);
	void k(const QString& new_text);
	void pi(const QString& new_text);
	void rns(const QString& new_text);
	void km(const QString& new_text);
	void rku(const QString& new_text);
	void _733_ch(const QString& new_text);
	void _733_rg(const QString& new_text);
	void p732_km(const QString& new_text);
	//void og(const QString& new_text);vchm4_clicked
	void BYP_kan(const QString& new_text);
	void BYP_pitan_shdU(const QString& new_text);
	void BYP_pitan_shdF(const QString& new_text);
	void BYP_pitan_DUU(const QString& new_text);
	void BYP_pitan_DUF(const QString& new_text);
	void BYP_ispr(const QString& new_text);
signals:
	void new_data(uint addr, uint saddr, uint w_Count, QVariantList words);
private:
	void change_ok_data(int ok_num, int new_value);
	void parse_xml();
	QVariantList toVariant(QList<unsigned short> _words);
private:
	QMap<int, AB_data> ab_data;
	RPC_main_server_SLOT_Thread server_thr;

	Ui::bkupi_widg *ui;
	QXmlStreamReader xml_reader;
	QMap<int, OK_DATA> oks;
};

#endif // DIALOG_H
