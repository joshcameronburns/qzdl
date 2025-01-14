/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
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

#include <QFileInfo>
#include <utility>
#include "ZDLListable.h"
#include "ZDLNameListable.h"
#include "ZDLConfigurationManager.h"

ZDLNameListable::ZDLNameListable(
        QListWidget *parent,
        int type,
        QString file,
        QString name)
        :
        ZDLListable(parent, type) {
    setFile(std::move(file));
    setDisplayName(std::move(name));
    setName(generateName());
}

ZDLNameListable::~ZDLNameListable()
= default;

QString ZDLNameListable::getFile() {
    return fileName;
}

QString ZDLNameListable::getName() {
    return displayName;
}

void ZDLNameListable::setDisplayName(QString name) {
    displayName = std::move(name);
    setName(generateName());
}

void ZDLNameListable::setFile(QString file) {
    fileName = std::move(file);
    setName(generateName());
}

QString ZDLNameListable::generateName() {
    return generateName(ZDLConfigurationManager::getActiveConfiguration());
}

QString ZDLNameListable::generateName(ZDLConf *zconf) {
    bool showPath = true;
    if (zconf->hasValue("zdl.general", "showpaths")) {
        int ok = 0;
        QString rc = zconf->getValue("zdl.general", "showpaths", &ok);
        if (!rc.isNull()) {
            if (rc == "0") {
                showPath = false;
            }
        }
    }
    QString list = "";
    if (showPath) {
        list = QString("%1 [%2]").arg(displayName, fileName);
    } else {
        list = displayName;
    }
    return list;
}

