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

#include <QRegularExpression>
#include <QMainWindow>
#include <QAction>
#include <QMessageBox>

#include "ZDLInterface.h"
#include "ZDLMainWindow.h"
#include "ZDLConfigurationManager.h"
#include "ZDLImportDialog.h"
#include "base/ZDLMapFile.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <wordexp.h>
#endif

ZDLMainWindow::~ZDLMainWindow()
{
	QSize sze = this->size();
	QPoint pt = this->pos();
	ZDLConf* zconf = ZDLConfigurationManager::getActiveConfiguration();
	if (zconf)
	{
		QString str = QString("%1,%2").arg(sze.width()).arg(sze.height());
		zconf->setValue("zdl.general", "windowsize", str);
		str = QString("%1,%2").arg(pt.x()).arg(pt.y());
		zconf->setValue("zdl.general", "windowpos", str);
	}
	LOGDATAO() << "Closing main window" << Qt::endl;
}

QString ZDLMainWindow::getWindowTitle()
{
	QString windowTitle = "ZDL";
	windowTitle += " " ZDL_VERSION_STRING;
	ZDLConfiguration* conf = ZDLConfigurationManager::getConfiguration();
	if (conf)
	{
		QString userConfPath = conf->getPath(ZDLConfiguration::CONF_USER);
		QString currentConf = ZDLConfigurationManager::getConfigFileName();
		if (userConfPath != currentConf)
		{
			windowTitle += " [" + ZDLConfigurationManager::getConfigFileName() + "]";
		}
	}
	else
	{
		windowTitle += ZDLConfigurationManager::getConfigFileName();
	}
	LOGDATAO() << "Returning main window title " << windowTitle << Qt::endl;
	return windowTitle;

}

ZDLMainWindow::ZDLMainWindow(QWidget* parent) :
	QMainWindow(parent)
{
	LOGDATAO() << "New main window " << DPTR(this) << Qt::endl;
	QString windowTitle = getWindowTitle();
	setWindowTitle(windowTitle);

	setWindowIcon(ZDLConfigurationManager::getIcon());

	setContentsMargins(0, 2, 0, 0);
	layout()->setContentsMargins(0, 0, 0, 0);
	auto* widget = new QTabWidget(this);

	ZDLConf* zconf = ZDLConfigurationManager::getActiveConfiguration();
	if (zconf)
	{
		int ok = 0;
		bool qtok = false;
		if (zconf->hasValue("zdl.general", "windowsize"))
		{
			QString size = zconf->getValue("zdl.general", "windowsize", &ok);
			if (size.contains(","))
			{
				QStringList list = size.split(",");
				int w = list[0].toInt(&qtok);
				if (qtok)
				{
					int h = list[1].toInt(&qtok);
					if (qtok)
					{
						LOGDATAO() << "Resizing to w:" << w << " h:" << h << Qt::endl;
						this->resize(QSize(w, h));
					}
				}
			}
		}
		if (zconf->hasValue("zdl.general", "windowpos"))
		{
			QString size = zconf->getValue("zdl.general", "windowpos", &ok);
			if (size.contains(","))
			{
				QStringList list = size.split(",");
				int x = list[0].toInt(&qtok);
				if (qtok)
				{
					int y = list[1].toInt(&qtok);
					if (qtok)
					{
						LOGDATAO() << "Moving to x:" << x << " y:" << y << Qt::endl;
						this->move(QPoint(x, y));
					}
				}
			}

		}
	}

	intr = new ZDLInterface(this);
	settings = new ZDLSettingsTab(this);

	widget->setDocumentMode(true);
	widget->addTab(intr, "Launch config");
	widget->addTab(settings, "General settings");
	setCentralWidget(widget);

	auto* qact = new QAction(widget);
	qact->setShortcut(Qt::Key_Return);
	widget->addAction(qact);
	connect(qact, SIGNAL(triggered()), this, SLOT(launch()));

	qact2 = new QAction(widget);
	qact2->setShortcut(Qt::Key_Escape);
	widget->addAction(qact2);

	connect(qact2, SIGNAL(triggered()), this, SLOT(quit()));

	connect(widget, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
	LOGDATAO() << "Main window created." << Qt::endl;
}

void ZDLMainWindow::handleImport()
{
#if !defined(NO_IMPORT)
	ZDLConfiguration* conf = ZDLConfigurationManager::getConfiguration();
	if (conf)
	{
		QString userConfPath = conf->getPath(ZDLConfiguration::CONF_USER);
		QString currentConf = ZDLConfigurationManager::getConfigFileName();
		if (userConfPath != currentConf)
		{
			LOGDATAO() << "Not currently using user conf" << Qt::endl;
			ZDLConf* zconf = ZDLConfigurationManager::getActiveConfiguration();
			if (zconf->hasValue("zdl.general", "donotimportthis"))
			{
				int ok = 0;
				if (zconf->getValue("zdl.general", "donotimportthis", &ok) == "1")
				{
					LOGDATAO() << "Don't import current config" << Qt::endl;
					return;
				}
			}
			QFile userFile(userConfPath);
			ZDLConf userconf;
			if (userFile.exists())
			{
				LOGDATAO() << "Reading user conf" << Qt::endl;
				userconf.readINI(userConfPath);
			}
			if (userconf.hasValue("zdl.general", "nouserconf"))
			{
				int ok = 0;
				if (userconf.getValue("zdl.general", "nouserconf", &ok) == "1")
				{
					LOGDATAO() << "Do not use user conf" << Qt::endl;
					return;
				}
			}
			if (ZDLConfigurationManager::getWhy() == ZDLConfigurationManager::USER_SPECIFIED)
			{
				LOGDATA() << "The user asked for this ini, don't go forward" << Qt::endl;
				return;
			}
			if (userFile.size() < 10)
			{
				LOGDATA() << "User conf is small, assuming empty" << Qt::endl;
				ZDLImportDialog importd(this);
				importd.exec();
				if (importd.result() == QDialog::Accepted)
				{
					switch (importd.getImportAction())
					{
					case ZDLImportDialog::IMPORTNOW:
						LOGDATAO() << "Importing now" << Qt::endl;
						if (!userFile.exists())
						{
							QStringList path = userConfPath.split("/");
							path.removeLast();
							QDir dir;
							if (!dir.mkpath(path.join("/")))
							{
								break;
							}
						}

						zconf->setValue("zdl.general", "importedfrom", currentConf);
						zconf->setValue("zdl.general", "isimported", "1");
						zconf->setValue("zdl.general", "importdate", QDateTime::currentDateTime().toString(Qt::ISODate));

						zconf->writeINI(userConfPath);
						ZDLConfigurationManager::setConfigFileName(userConfPath);
						break;
					case ZDLImportDialog::DONOTIMPORTTHIS:
						LOGDATAO() << "Tagging this config as not importable" << Qt::endl;
						zconf->setValue("zdl.general", "donotimportthis", "1");
						break;
					case ZDLImportDialog::NEVERIMPORT:
						LOGDATAO() << "Setting NEVERi IMPORT" << Qt::endl;
						userconf.setValue("zdl.general", "nouserconf", "1");
						if (!userFile.exists())
						{
							QStringList path = userConfPath.split("/");
							path.removeLast();
							QDir dir;
							if (!dir.mkpath(path.join("/")))
							{
								break;
							}

						}
						userconf.writeINI(userConfPath);
						break;
					case ZDLImportDialog::ASKLATER:

					case ZDLImportDialog::UNKNOWN:
					default:
						LOGDATAO() << "Not setting anything" << Qt::endl;
						break;
					}
				}
			}
		}
	}
#endif
}

void ZDLMainWindow::tabChange(int newTab)
{
	LOGDATAO() << "Tab changed to " << newTab << Qt::endl;
	if (newTab == 0)
	{
		settings->notifyFromParent(nullptr);
		intr->readFromParent(nullptr);
	}
	else if (newTab == 1)
	{
		intr->notifyFromParent(nullptr);
		settings->readFromParent(nullptr);
	}
}

void ZDLMainWindow::quit()
{
	LOGDATAO() << "quitting" << Qt::endl;
	writeConfig();
	close();
}

void ZDLMainWindow::launch()
{
	LOGDATAO() << "Launching" << Qt::endl;
	writeConfig();
	ZDLConf* zconf = ZDLConfigurationManager::getActiveConfiguration();

	QString exec = getExecutable();
	if (exec.length() < 1)
	{
		QMessageBox::warning(this, "ZDL", "Please select a source port.");
		return;
	}
	QFileInfo exec_fi(exec);
	bool no_err = true;

#ifdef _WIN32
	PROCESS_INFORMATION pi={};
	STARTUPINFO si={sizeof(STARTUPINFO), nullptr, nullptr, nullptr, 0, 0, 0, 0, 0, 0, 0, STARTF_USESHOWWINDOW, SW_SHOWNORMAL};

	QString cmdline="\""+QDir::toNativeSeparators(exec_fi.absoluteFilePath())+"\" "+getArgumentsString(true);
	QString cwd=QDir::toNativeSeparators(exec_fi.absolutePath());

	if (CreateProcess(nullptr, (LPWSTR)cmdline.toStdWString().c_str(), nullptr, nullptr, FALSE, NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT | CREATE_NEW_CONSOLE, nullptr, cwd.toStdWString().c_str(), &si, &pi)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	} else {
		QMessageBox::warning(this, "ZDL", "Failed to launch the application executable.");
		no_err=false;
	}
#else
	if (!QProcess::startDetached(exec_fi.absoluteFilePath(), getArgumentsList(), exec_fi.absolutePath()))
	{
		QMessageBox::warning(this, "ZDL", "Failed to launch the application executable.");
		no_err = false;
	}
#endif
	if (no_err)
	{
		QString aclose = zconf->getValue("zdl.general", "autoclose");
		if (aclose == "1" || aclose == "true")
		{
			LOGDATAO() << "Asked to exit... closing" << Qt::endl;
			close();
		}
	}
}

QStringList WarpBackwardCompat(const QString& iwad_path, const QString& map_name)
{
	if (iwad_path.length())
	{
		bool iwad_mapxx = false;

		if (ZDLMapFile* mapfile = ZDLMapFile::getMapFile(iwad_path))
		{
			iwad_mapxx = mapfile->isMAPXX();
			delete mapfile;
		}

		QRegularExpressionMatch match;
		if (iwad_mapxx)
		{
			QRegularExpression mapxx_re("^MAP(\\d\\d)$");
			match = mapxx_re.match(map_name, Qt::CaseInsensitive);
			if (match.hasPartialMatch())
				return QStringList() << "-warp" << match.captured(1);
		}
		else
		{
			QRegularExpression exmy_re("^E(\\d)M([1-9])$");
			match = exmy_re.match(map_name, Qt::CaseInsensitive);
			if (match.hasPartialMatch())
				return QStringList() << "-warp" << match.captured(1) << match.captured(2);
		}
	}

	return {};
}

#ifdef _WIN32

QString QuoteParam(const QString& param)
{
	//Based on "Everyone quotes command line arguments the wrong way" by Daniel Colascione
	//http://blogs.msdn.com/b/twistylittlepassagesallalike/archive/2011/04/23/everyone-quotes-arguments-the-wrong-way.aspx

	if (!param.isEmpty()&&param.indexOf(QRegularExpression("[\\s\"]"))<0) {
		return param;
	} else {
		QString qparam('"');

		for (QString::const_iterator it=param.constBegin();; it++) {
			int backslash_count=0;

			while (it!=param.constEnd()&&*it=='\\') {
				it++;
				backslash_count++;
			}

			if (it==param.constEnd()) {
				qparam.append(QString(backslash_count*2, '\\'));
				break;
			} else if (*it==L'"') {
				qparam.append(QString(backslash_count*2+1, '\\'));
				qparam.append(*it);
			} else {
				qparam.append(QString(backslash_count, '\\'));
				qparam.append(*it);
			}
		}

		qparam.append('"');

		return qparam;
	}
}

QString ExpandEnvironmentStringsWrapper(QString args)
{
	wchar_t dummy_buf;

	//Documentation says that lpDst parameter is optional but Win 95 version of this function actually fails if lpDst is nullptr
	//So using dummy buffer to get needed buffer length (function returns length in characters including terminating nullptr)
	//If returned length is 0 - it is an error
	if (DWORD buf_len=ExpandEnvironmentStrings(args.toStdWString().c_str(), &dummy_buf, 0)) {
		wchar_t* expanded_buf=new wchar_t[buf_len];

		//Ensuring that returned length is expected length
		if (ExpandEnvironmentStrings(args.toStdWString().c_str(), expanded_buf, buf_len)<=buf_len)
			args.setUtf16(reinterpret_cast<ushort*>(expanded_buf), static_cast<qsizetype>(buf_len) - 1);

		delete[] expanded_buf;
	}
	
	return args;
}

#define IF_NATIVE_SEP(p)	(native_sep?QDir::toNativeSeparators(p):p)

QString ZDLMainWindow::getArgumentsString(bool native_sep)
{
	LOGDATAO() << "Getting arguments" << Qt::endl;
	QString args;
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = nullptr;

	QString iwadName = zconf->getValue("zdl.save", "iwad");
	QString iwadPath;

	section = zconf->getSection("zdl.iwads");
	if (section&&iwadName.length()){
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^i[0-9]+n$", fileVctr);

		for(int i = 0; i < fileVctr.size(); i++){
			ZDLLine *line = fileVctr[i];
			if(line->getValue().compare(iwadName) == 0){
				QString var = line->getVariable();
				if(var.length() >= 3){
					var = var.mid(1,var.length()-2);
					QVector<ZDLLine*> nameVctr;
					var = QString("i") + var + QString("f");
					section->getRegex("^" + var + "$",nameVctr);
					if(nameVctr.size() == 1){
						iwadPath=nameVctr[0]->getValue();
						args.append("-iwad ");
						args.append(QuoteParam(IF_NATIVE_SEP(iwadPath)));
					}
				}
			}
		}
	}

	if (zconf->hasValue("zdl.save", "skill")){
		bool ok;
		QString s_skill=zconf->getValue("zdl.save", "skill");
		int i_skill=s_skill.toInt(&ok, 10);

		if (i_skill>0&&i_skill<6) {
			args.append(" -skill ");
			args.append(s_skill);
		} else if (i_skill==6) {
			args.append(" -nomonsters");
		}
	}

	if (zconf->hasValue("zdl.save", "warp")){
		QString map_arg=zconf->getValue("zdl.save", "warp");
		QStringList warp_args=WarpBackwardCompat(iwadPath, map_arg);

		if (warp_args.length()) {
			args.append(' ');
			args.append(warp_args.join(" "));
		} else {
			args.append(" +map ");
			args.append(QuoteParam(map_arg));
		}
	}

	section = zconf->getSection("zdl.save");
	QStringList pwads;
	QStringList dehs;
	QStringList bexs;
	QStringList lumps;
	QStringList autoexecs;
	char deh_last=1;
	if (section){
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^file[0-9]+$", fileVctr);

		if (fileVctr.size() > 0){
			for(int i = 0; i < fileVctr.size(); i++){
				if(fileVctr[i]->getValue().endsWith(".bex",Qt::CaseInsensitive)) {
					deh_last=0;
					bexs << fileVctr[i]->getValue();
				} else if(fileVctr[i]->getValue().endsWith(".deh",Qt::CaseInsensitive)) {
					deh_last=1;
					dehs << fileVctr[i]->getValue();
				} else if(fileVctr[i]->getValue().endsWith(".cfg",Qt::CaseInsensitive)) {
					autoexecs << fileVctr[i]->getValue();
				} else if(fileVctr[i]->getValue().endsWith(".lmp",Qt::CaseInsensitive)) {
					lumps << fileVctr[i]->getValue();
				} else {
					pwads << fileVctr[i]->getValue();
				}
			}
		}
	}

	if(pwads.size() > 0){
		args.append(" -file");
		for (const QString& str: pwads) {
			args.append(' ');
			args.append(QuoteParam(IF_NATIVE_SEP(str)));
		}
	}

	do {
		if (deh_last%2) {
			for (const QString& str: bexs) {
				args.append(" -bex ");
				args.append(QuoteParam(IF_NATIVE_SEP(str)));
			}
		} else {
			for (const QString& str: dehs) {
				args.append(" -deh ");
				args.append(QuoteParam(IF_NATIVE_SEP(str)));
			}
		}
		deh_last+=3;
	} while (deh_last<=4);

	for (const QString& str: autoexecs) {
		args.append(" +exec ");
		args.append(QuoteParam(IF_NATIVE_SEP(str)));
	}

	for (const QString& str: lumps) {
		args.append(" -playdemo ");
		args.append(QuoteParam(IF_NATIVE_SEP(str)));
	}

	if(zconf->hasValue("zdl.save","gametype")){
		QString tGameType = zconf->getValue("zdl.save","gametype");
		if(tGameType != "0"){
			if (zconf->hasValue("zdl.save", "dmflags")){
				args.append(" +set dmflags ");
				args.append(zconf->getValue("zdl.save", "dmflags"));
			}

			if (zconf->hasValue("zdl.save", "dmflags2")){
				args.append(" +set dmflags2 ");
				args.append(zconf->getValue("zdl.save", "dmflags2"));
			}

			if (tGameType == "2"){
				args.append(" -deathmath");
			} else if (tGameType == "3"){
				args.append(" -altdeath");
			}

			int players = 0;
			if(zconf->hasValue("zdl.save","players")){
				bool ok;
				players = zconf->getValue("zdl.save","players").toInt(&ok, 10);
			}
			if(players > 0){
				args.append(" -host ");
				args.append(QString::number(players));
				if(zconf->hasValue("zdl.save","mp_port")){
					args.append(" -port ");
					args.append(zconf->getValue("zdl.save","mp_port"));
				}
			}else if(players == 0){
				if(zconf->hasValue("zdl.save", "host")) {
					args.append(" -join ");
					if (zconf->hasValue("zdl.save", "mp_port")) {
						QRegularExpression trailing_port(":\\d*\\s*$");
						args.append(zconf->getValue("zdl.save", "host").remove(trailing_port)+":"+zconf->getValue("zdl.save", "mp_port"));
					} else {
						args.append(zconf->getValue("zdl.save", "host"));
					}
				}
			}
			if(zconf->hasValue("zdl.save","fraglimit")){
				args.append(" +set fraglimit ");
				args.append(zconf->getValue("zdl.save","fraglimit"));
			}
			if(zconf->hasValue("zdl.save","timelimit")){
				args.append(" +set timelimit ");
				args.append(zconf->getValue("zdl.save","timelimit"));
			}
			if(zconf->hasValue("zdl.save","extratic")){
				QString tVal = zconf->getValue("zdl.save","extratic");
				if(tVal == "1"){
					args.append(" -extratic");
				}
			}
			if(zconf->hasValue("zdl.save","netmode")){
				QString tVal = zconf->getValue("zdl.save","netmode");
				if(tVal != "-1"){
					args.append(" -netmode ");
					args.append(tVal);
				}
			}
			if(zconf->hasValue("zdl.save","dup")){
				QString tVal = zconf->getValue("zdl.save","dup");
				if(tVal != "0"){
					args.append(" -dup ");
					args.append(tVal);
				}
			}
			if(zconf->hasValue("zdl.save","savegame")){
				args.append(" -loadgame ");
				args.append(QuoteParam(IF_NATIVE_SEP(zconf->getValue("zdl.save","savegame"))));
			}
		}
	}

	if (zconf->hasValue("zdl.general", "alwaysadd")){
		args.append(' ');
		args.append(ExpandEnvironmentStringsWrapper(zconf->getValue("zdl.general", "alwaysadd")));
	}

	if (zconf->hasValue("zdl.save", "extra")){
		args.append(' ');
		args.append(ExpandEnvironmentStringsWrapper(zconf->getValue("zdl.save", "extra")));
	}

	LOGDATAO() << "args: " << args << Qt::endl;
	return args.trimmed();
}

QStringList ZDLMainWindow::getArgumentsList()
{
	return QStringList();
}

#else

QStringList ParseParams(const QString& params)
{
	QStringList plist;

	wordexp_t result;

	switch (wordexp(qPrintable(params), &result, 0))
	{
	case 0:
		for (size_t i = 0; i < result.we_wordc; i++)
		{
			plist << result.we_wordv[i];
		}
		[[fallthrough]];
	case WRDE_NOSPACE:    //If error is WRDE_NOSPACE - there is a possibilty that at least some part of wordexp_t.we_wordv was allocated
		wordfree(&result);
	}

	return plist;
}

QStringList ZDLMainWindow::getArgumentsList()
{
	LOGDATAO() << "Getting arguments" << Qt::endl;
	QStringList args;
	ZDLConf* zconf = ZDLConfigurationManager::getActiveConfiguration();

	QString iwadPath;
	QString iwadName = zconf->getValue("zdl.save", "iwad");

	ZDLSection* section = zconf->getSection("zdl.iwads");
	if (section && iwadName.length())
	{
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^i[0-9]+n$", fileVctr);

		for (auto line: fileVctr)
		{
			if (line->getValue().compare(iwadName) == 0)
			{
				QString var = line->getVariable();
				if (var.length() >= 3)
				{
					var = var.mid(1, var.length() - 2);
					QVector<ZDLLine*> nameVctr;
					var = QString("i") + var + QString("f");
					section->getRegex("^" + var + "$", nameVctr);
					if (nameVctr.size() == 1)
					{
						iwadPath = nameVctr[0]->getValue();
						args << "-iwad" << iwadPath;
					}
				}
			}
		}
	}

	if (zconf->hasValue("zdl.save", "skill"))
	{
		bool ok;
		QString s_skill = zconf->getValue("zdl.save", "skill");
		int i_skill = s_skill.toInt(&ok, 10);

		if (i_skill > 0 && i_skill < 6)
		{
			args << "-skill" << s_skill;
		}
		else if (i_skill == 6)
		{
			args << "-nomonsters";
		}
	}

	if (zconf->hasValue("zdl.save", "warp"))
	{
		QString map_arg = zconf->getValue("zdl.save", "warp");
		QStringList warp_args = WarpBackwardCompat(iwadPath, map_arg);

		if (warp_args.length())
		{
			args << warp_args;
		}
		else
		{
			args << "+map" << map_arg;
		}
	}

	section = zconf->getSection("zdl.save");
	QStringList pwads;
	QStringList dehs;
	QStringList bexs;
	QStringList autoexecs;
	QStringList lumps;
	char deh_last = 1;
	if (section)
	{
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^file[0-9]+$", fileVctr);

		if (!fileVctr.empty())
		{
			for (auto& i: fileVctr)
			{
				if (i->getValue().endsWith(".bex", Qt::CaseInsensitive))
				{
					deh_last = 0;
					bexs << i->getValue();
				}
				else if (i->getValue().endsWith(".deh", Qt::CaseInsensitive))
				{
					deh_last = 1;
					dehs << i->getValue();
				}
				else if (i->getValue().endsWith(".cfg", Qt::CaseInsensitive))
				{
					autoexecs << i->getValue();
				}
				else if (i->getValue().endsWith(".lmp", Qt::CaseInsensitive))
				{
					lumps << i->getValue();
				}
				else
				{
					pwads << i->getValue();
				}
			}
		}
	}

	if (!pwads.empty())
	{
		args << "-file";
		for (const QString& str: pwads)
		{
			args << str;
		}
	}

	do
	{
		if (deh_last % 2)
		{
			for (const QString& str: bexs)
			{
				args << "-bex" << str;
			}
		}
		else
		{
			for (const QString& str: dehs)
			{
				args << "-deh" << str;
			}
		}
		deh_last += 3;
	} while (deh_last <= 4);

	for (const QString& str: autoexecs)
	{
		args << "+exec" << str;
	}

	for (const QString& str: lumps)
	{
		args << "-playdemo" << str;
	}

	if (zconf->hasValue("zdl.save", "gametype"))
	{
		QString tGameType = zconf->getValue("zdl.save", "gametype");
		if (tGameType != "0")
		{
			if (zconf->hasValue("zdl.save", "dmflags"))
			{
				args << "+set" << "dmflags" << zconf->getValue("zdl.save", "dmflags");
			}

			if (zconf->hasValue("zdl.save", "dmflags2"))
			{
				args << "+set" << "dmflags2" << zconf->getValue("zdl.save", "dmflags2");
			}

			if (tGameType == "2")
			{
				args << "-deathmath";
			}
			else if (tGameType == "3")
			{
				args << "-altdeath";
			}

			int players = 0;
			if (zconf->hasValue("zdl.save", "players"))
			{
				bool ok;
				players = zconf->getValue("zdl.save", "players").toInt(&ok, 10);
			}
			if (players > 0)
			{
				args << "-host" << QString::number(players);
				if (zconf->hasValue("zdl.save", "mp_port"))
				{
					args << "-port" << zconf->getValue("zdl.save", "mp_port");
				}
			}
			else if (players == 0)
			{
				if (zconf->hasValue("zdl.save", "host"))
				{
					args << "-join";
					if (zconf->hasValue("zdl.save", "mp_port"))
					{
						QRegularExpression trailing_port(":\\d*\\s*$");
						args << zconf->getValue("zdl.save", "host").remove(trailing_port) + ":"
								+ zconf->getValue("zdl.save", "mp_port");
					}
					else
					{
						args << zconf->getValue("zdl.save", "host");
					}
				}
			}
			if (zconf->hasValue("zdl.save", "fraglimit"))
			{
				args << "+set" << "fraglimit" << zconf->getValue("zdl.save", "fraglimit");
			}
			if (zconf->hasValue("zdl.save", "timelimit"))
			{
				args << "+set" << "timelimit" << zconf->getValue("zdl.save", "timelimit");
			}
			if (zconf->hasValue("zdl.save", "extratic"))
			{
				QString tVal = zconf->getValue("zdl.save", "extratic");
				if (tVal == "1")
				{
					args << "-extratic";
				}
			}
			if (zconf->hasValue("zdl.save", "netmode"))
			{
				QString tVal = zconf->getValue("zdl.save", "netmode");
				if (tVal != "-1")
				{
					args << "-netmode" << tVal;
				}
			}
			if (zconf->hasValue("zdl.save", "dup"))
			{
				QString tVal = zconf->getValue("zdl.save", "dup");
				if (tVal != "0")
				{
					args << "-dup" << tVal;
				}
			}
			if (zconf->hasValue("zdl.save", "savegame"))
			{
				args << "-loadgame" << zconf->getValue("zdl.save", "savegame");
			}
		}
	}

	if (zconf->hasValue("zdl.general", "alwaysadd"))
	{
		args << ParseParams(zconf->getValue("zdl.general", "alwaysadd"));
	}

	if (zconf->hasValue("zdl.save", "extra"))
	{
		args << ParseParams(zconf->getValue("zdl.save", "extra"));
	}

	LOGDATAO() << "args: " << args << Qt::endl;
	return args;
}

QString ZDLMainWindow::getArgumentsString([[maybe_unused]] bool native_sep)
{
	QString args;

	for (const QString& str: getArgumentsList())
	{
		if (str.indexOf(QRegularExpression("\\s")) != -1)
		{
			args.append('"');
			args.append(str);
			args.append('"');
		}
		else
		{
			args.append(str);
		}
		args.append(' ');
	}

	return args;
}

#endif

QString ZDLMainWindow::getExecutable()
{
	LOGDATAO() << "Getting exec" << Qt::endl;
	ZDLConf* zconf = ZDLConfigurationManager::getActiveConfiguration();
	int stat;
	QString portName;
	if (zconf->hasValue("zdl.save", "port"))
	{
		ZDLSection* section = zconf->getSection("zdl.ports");
		portName = zconf->getValue("zdl.save", "port", &stat);
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^p[0-9]+n$", fileVctr);

		for (auto line: fileVctr)
		{
			if (line->getValue().compare(portName) == 0)
			{
				QString var = line->getVariable();
				if (var.length() >= 3)
				{
					var = var.mid(1, var.length() - 2);
					QVector<ZDLLine*> nameVctr;
					var = QString("p") + var + QString("f");
					section->getRegex("^" + var + "$", nameVctr);
					if (nameVctr.size() == 1)
					{
						LOGDATAO() << "Executable: " << nameVctr[0]->getValue() << Qt::endl;
						return QString(nameVctr[0]->getValue());
					}
				}
			}
		}
	}
	LOGDATAO() << "No executable" << Qt::endl;
	return { "" };
}

//Pass through functions.
void ZDLMainWindow::startRead()
{
	LOGDATAO() << "Starting to read configuration" << Qt::endl;
	intr->startRead();
	settings->startRead();
	QString windowTitle = getWindowTitle();
	setWindowTitle(windowTitle);
}

void ZDLMainWindow::writeConfig()
{
	LOGDATAO() << "Writing configuration" << Qt::endl;
	intr->writeConfig();
	settings->writeConfig();
}
