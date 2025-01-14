/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * Copyright (C) 2018-2019  Lcferrum
 * Copyright (C) 2023  spacebub
 * 
 * qZDL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <QtCore>
#include "zdlcommon.h"
#include "zdlsection.hpp"

class ZDLConf
{
 public:
	enum modes
	{
		ReadOnly = 0x01,
		WriteOnly = 0x02,
		ReadWrite = ReadOnly | WriteOnly,
		FileRead = 0x04,
		FileWrite = 0x08,
		FileReadWrite = FileRead | FileWrite,
		Default = ReadWrite | FileReadWrite
	};

	int readINI(const QString& file);
	int writeINI(const QString& file);
	QString getValue(const QString& section, const QString& variable);
	QString getValue(const QString& lsection, const QString& variable, int* status);
	int hasValue(const QString& section, const QString& variable);
	void deleteValue(const QString& lsection, const QString& variable);
	void setValue(const QString& lsection, const QString& variable, int value);
	void setValue(const QString& lsection, const QString& variable, const QString& szBuffer);
	~ZDLConf();
	explicit ZDLConf(int mode = ZDLConf::Default);
	int reopen(int imode);
	QVector<ZDLSection*> sections;
	int writeStream(QIODevice* stream);
	ZDLSection* getSection(const QString& section);
	void deleteSection(const QString& section);
	ZDLConf* clone();
	void deleteSectionByName(const QString& section);
	void addSection(ZDLSection* section)
	{
		sections.push_back(section);
	}
	int getFlagsForValue(const QString& section, const QString& var);
	bool setFlagsForValue(const QString& section, const QString& var, int value);
	bool deleteRegex(const QString& section, const QString& regex);
 protected:
	void readLock()
	{
		LOGDATAO() << "ReadLockGet" << Qt::endl;
		GET_READLOCK(mutex);
	}
	void writeLock()
	{
		LOGDATAO() << "WriteLockGet" << Qt::endl;
		GET_WRITELOCK(mutex);
	}
	void releaseReadLock()
	{
		LOGDATAO() << "ReadLockRelease" << Qt::endl;
		RELEASE_READLOCK(mutex);
	}
	void releaseWriteLock()
	{
		LOGDATAO() << "WriteLockRelease" << Qt::endl;
		RELEASE_WRITELOCK(mutex);
	}
	bool tryReadLock(int timeout = 999999999)
	{
		LOGDATAO() << "ReadLockTryGet" << Qt::endl;
		return TRY_READLOCK(mutex, timeout);
	}
	bool tryWriteLock(int timeout = 999999999)
	{
		LOGDATAO() << "WriteLockTryGet" << Qt::endl;
		return TRY_WRITELOCK(mutex, timeout);
	}
 private:
	int mode;
	int reads;
	int writes;
	void parse(QString in, ZDLSection* current);
	LOCK_CLASS* mutex;
};
