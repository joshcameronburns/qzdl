/*
 * This file is part of qZDL
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
#pragma once

#include <QtCore>
#include "ZDLMapFile.h"

class ZLibDir : public ZDLMapFile {
private:
    QString file;
public:
    explicit ZLibDir(QString file);

    QString getIwadinfoName() override;

    QStringList getMapNames() override;

    bool isMAPXX() override;

    ~ZLibDir() override;
};
