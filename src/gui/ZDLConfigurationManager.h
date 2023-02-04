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

#include <QComboBox>
#include <QValidator>
#include "ZDLWidget.h"
#include "base/ZDLConfiguration.h"
#include "base/zdlcommon.h"

class ZDLConfigurationManager{
	public:
		enum WhyConfig {UNKNOWN, USER_SPECIFIED, USER_CONF, IN_EXEC_DIR, IN_CWD};
		static void init();
		static ZDLWidget* getInterface();
		static void setInterface(ZDLWidget *widget);
		static void setWhy(WhyConfig whyConfig);		
		static WhyConfig getWhy();	

		// Deprecating this soon!
		static void setActiveConfiguration(ZDLConf *zconf);
		static ZDLConf* getActiveConfiguration();
		
		static ZDLConfiguration* getConfiguration();
		
		static void setCurrentDirectory(const QString &dir);
		static QString getCurrentDirectory();
		static QPixmap getIcon();
		static QString getConfigFileName();
		static void setConfigFileName(QString name);
		static QStringList getArgv();
		static void setArgv(QStringList args);
		static QString getExec();
		static void setExec(QString execu);
	protected:
		static QString exec;
		static QStringList argv;
		static QString filename;
		static ZDLWidget* zinterface;
		static ZDLConf *activeConfig;
		static QString cdir;
		static ZDLConfiguration *conf;
		static WhyConfig why;
};

QString getLastDir(ZDLConf *zconf=nullptr);
void saveLastDir(const QString& fileName, ZDLConf *zconf=nullptr);
QString getWadLastDir(ZDLConf *zconf=nullptr, bool dwd_first=false);
void saveWadLastDir(const QString& fileName, ZDLConf *zconf=nullptr, bool is_dir=false);
QString getSrcLastDir(ZDLConf *zconf=nullptr);
void saveSrcLastDir(const QString& fileName, ZDLConf *zconf=nullptr);
QString getSaveLastDir(ZDLConf *zconf=nullptr);
void saveSaveLastDir(const QString& fileName, ZDLConf *zconf=nullptr);
QString getZdlLastDir(ZDLConf *zconf=nullptr);
void saveZdlLastDir(const QString& fileName, ZDLConf *zconf=nullptr);
QString getIniLastDir(ZDLConf *zconf=nullptr);
void saveIniLastDir(const QString& fileName, ZDLConf *zconf=nullptr);

class VerboseComboBox: public QComboBox {
	Q_OBJECT
public:
	explicit VerboseComboBox(QWidget *parent=nullptr): QComboBox(parent) {}
	void showPopup() override;
	void hidePopup() override;
signals:
	void onPopup();
	void onHidePopup();
};

class EvilValidator: public QValidator {
	Q_OBJECT
public:
	explicit EvilValidator(QObject *parent): QValidator(parent) {}
	QValidator::State validate(QString &input, int &pos) const override;
};
