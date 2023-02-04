/*
 * This file is part of qZDL
 * Copyright (C) 2007-2012  Cody Harris
 * Copyright (C) 2019  Lcferrum
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

#include "ZDLConfiguration.h"

ZDLConfiguration::ZDLConfiguration()
{
	//TODO These need to migrate
	QSettings system(QSettings::IniFormat, QSettings::SystemScope, "Vectec Software", "qZDL", nullptr);
	QSettings user(QSettings::IniFormat, QSettings::UserScope, "Vectec Software", "qZDL", nullptr);
	paths[CONF_SYSTEM] = system.fileName();
	paths[CONF_USER] = user.fileName();
	paths[CONF_FILE] = "zdl.ini";
	confs[CONF_SYSTEM] = new ZDLConf();
	confs[CONF_USER] = new ZDLConf();
	confs[CONF_FILE] = new ZDLConf();
}

QString ZDLConfiguration::getPath(ConfScope scope)
{
	if (scope >= NUM_CONFS)
	{
		return { };
	}
	
	return paths[scope];
}

ZDLConf* ZDLConfiguration::getConf(ConfScope scope)
{
	if (scope >= NUM_CONFS)
	{
		return nullptr;
	}
	
	return confs[scope];
}

QString ZDLConfiguration::getString(
	[[maybe_unused]] const QString& section,
	[[maybe_unused]] const QString& key,
	int* ok,
	[[maybe_unused]] ConfScope scope,
	[[maybe_unused]] ScopeRules rules)
{
	if (ok)
	{ *ok = false; }
	return {};
}

int ZDLConfiguration::getInt(
	[[maybe_unused]] const QString& section,
	[[maybe_unused]] const QString& key,
	int* ok,
	[[maybe_unused]] ConfScope scope,
	[[maybe_unused]] ScopeRules rules)
{
	if (ok)
	{ *ok = false; }
	return 0;
}

bool ZDLConfiguration::setString(
	[[maybe_unused]] const QString& section,
	[[maybe_unused]] const QString& key,
	[[maybe_unused]] const QString& value,
	[[maybe_unused]] ConfScope scope,
	[[maybe_unused]] ScopeRules rules)
{
	return false;
}

bool ZDLConfiguration::setInt(
	[[maybe_unused]] const QString& section,
	[[maybe_unused]] const QString& key,
	[[maybe_unused]] int value,
	[[maybe_unused]] ConfScope scope,
	[[maybe_unused]] ScopeRules rules)
{
	return false;
}

bool ZDLConfiguration::hasVariable(
	[[maybe_unused]] const QString& section,
	[[maybe_unused]] const QString& key,
	[[maybe_unused]] ConfScope scope,
	[[maybe_unused]] ScopeRules rules)
{
	return false;
}
