#pragma once

#include <QtCore>
#include "ZDLWidget.h"

class ZDLListEntry
{
 public:
	/* Originator allows us to see who created this entry */
	/* Type is something descriptive but FQDN-like */
	/* ie net.vectec.qzdl.lists.iwad */
	ZDLListEntry(const QString& originator, const QString& type);
	ZDLListEntry(ZDLWidget originator, const QString& type);

	/* Be stupidly flexible and allow any data */
	/* to be added without restricting */
	/* how much to accept */
	void addData(const QString& key, const QVariant& newData);
	void removeData(const QString& key);
	QVariant getData(const QString& key);

	QString getType()
	{
		return type;
	}
	QString getOriginator()
	{
		return originator;
	}
 protected:
	QString originator;
	QString type;
	QHash<QString, QVariant> data;
};
