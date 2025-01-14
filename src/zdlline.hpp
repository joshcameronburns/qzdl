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
#pragma once


#define FLAG_NORMAL   0    // Normal flag
#define FLAG_VIRTUAL  1    // Does not get read/written, not cloned
#define FLAG_NOWRITE  2    // Can not write to this value
#define FLAG_TEMP     4    // Read/write and cloned, but not written

class ZDLLine {
    friend class ZDLVariables;

public:
    explicit ZDLLine(const QString &inLine);

    ZDLLine();

    ~ZDLLine();

    static int getType();

    QString getValue();

    QString getVariable();

    QString getLine();

    void setValue(const QString &inValue);

    ZDLLine *clone();

    void setIsCopy(bool val);

    bool setFlags(int flag);

    [[nodiscard]] int getFlags() const {
        return flags;
    }

private:
    bool isCopy;

    void parse();

    int findComment(char delim);

    int type;
    QString line;
    QString comment;
    QString value;
    QString variable;
    int flags{};
};
