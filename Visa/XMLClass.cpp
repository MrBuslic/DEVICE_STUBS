#include "XMLClass.h"


XMLClass::XMLClass()
{
}

XMLClass::~XMLClass()
{
}

bool XMLClass::read(QIODevice *device, QList<mezanin_struct> &mezanin_list)
{
	xmlr.setDevice(device);

	if (xmlr.readNextStartElement()) {
		if (xmlr.name() == "root")// && xmlr.attributes().value("version") == "1.0")
			readXML(mezanin_list);
		else
			xmlr.raiseError(QObject::tr("Ошибка! Xml файл не верен!"));
	}

	return !xmlr.error();
}

QString XMLClass::errorString() const
{
	return QObject::tr("%1\nLine %2, column %3")
		.arg(xmlr.errorString())
		.arg(xmlr.lineNumber())
		.arg(xmlr.columnNumber());
}


void XMLClass::writeXML(QIODevice * device, QList<mezanin_struct>& mezanin_list)
{
	xmlw.setDevice(device);
	xmlw.setAutoFormatting(true);
	xmlw.writeStartDocument();

	if (!mezanin_list.empty())
	{
		xmlw.writeStartElement("root");
		xmlw.writeStartElement("comapp");
		xmlw.writeStartElement("mezanins");
		foreach(mezanin_struct mez, mezanin_list)
		{
			//QString str = QString::fromUtf8(mez.desc);
			if (mez.commapp != 1)
				continue;
			xmlw.writeStartElement("mezanin");
			xmlw.writeAttribute("desc", QString::fromUtf8(mez.desc));
			xmlw.writeAttribute("model", QString::number(mez.model));
			xmlw.writeAttribute("slot", QString::number(mez.slot));
			xmlw.writeEndElement();	//mezanin
		}
		xmlw.writeEndElement();	//mezanins
		xmlw.writeEndElement();	//comappS

		xmlw.writeEndDocument();	//root
	}
	return;
}

void XMLClass::readXML(QList<mezanin_struct> &mezanin_list)
{
	while (xmlr.readNextStartElement())
	{
		if (xmlr.name() == "comapp")
			readCommapp("comapp" ,mezanin_list);
		else
			xmlr.skipCurrentElement();
	}
}

void XMLClass::readCommapp(QString commapp, QList<mezanin_struct> &mezanin_list)
{
	while (xmlr.readNextStartElement())
	{
		if (xmlr.name() == "mezanins")
			readMezanins(commapp, mezanin_list);
		else
			xmlr.skipCurrentElement();
	}
}

void XMLClass::readMezanins(QString commapp, QList<mezanin_struct>& mezanin_list)
{
	while (xmlr.readNextStartElement())
	{
		if (xmlr.name() == "mezanin")
			readMezanin(commapp, mezanin_list);
		else
			xmlr.skipCurrentElement();
	}
}

void XMLClass::readMezanin(QString commapp, QList<mezanin_struct> &mezanin_list)
{
	QString desc = xmlr.attributes().value("desc").toString();
	bool ok;
	int model = xmlr.attributes().value("model").toInt(&ok, 16);
	int slot = xmlr.attributes().value("slot").toInt();

	QString model2 = xmlr.attributes().value("model").toString();
	QString slot2 = xmlr.attributes().value("slot").toString();
	mezanin_struct mez;

	//QByteArray ba = desc.toLocal8Bit().data();
	//const char *c_str2 = ba.data();

	strcpy(mez.desc, desc.toLocal8Bit().data());
	mez.type = 2;
	mez.model = model; // FOI
	mez.slot = slot;
	mezanin_list.push_back(mez);

	//mezanin_list

	xmlr.readNextStartElement();
}
