#ifndef XMLCLASS_H
#define XMLCLASS_H

#include <qxmlstreamreader>
#include <QMessageBox>

struct mezanin_struct
{
	char desc[50];
	int slot = 0;
	int type = 0;
	int model = 0;
	int commapp = 0;
};

class XMLClass : public QObject
{
	Q_OBJECT
public:
	XMLClass();
	bool read(QIODevice *device, QList<mezanin_struct> &mezanin_list);
	void writeXML(QIODevice *device, QList<mezanin_struct> &mezanin_list);
	QString errorString() const;
	~XMLClass();
private:
	QXmlStreamReader xmlr;
	QXmlStreamWriter xmlw;
	
	void readXML(QList<mezanin_struct> &mezanin_list);
	void readCommapp(QString commapp, QList<mezanin_struct> &mezanin_list);
	void readMezanins(QString commapp, QList<mezanin_struct> &mezanin_list);
	void readMezanin(QString commapp, QList<mezanin_struct> &mezanin_list);
};
#endif