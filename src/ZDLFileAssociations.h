/*
 * This file is part of qZDL
 * Copyright (C) 2007-2012  Cody Harris
 * Copyright (C) 2018-2019  Lcferrum
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

#include <QtGui>
#include <QObject>
#include <QListWidget>
#include <QDialog>
#include <string>
#include <vector>
#include <windows.h>

class AssocListWidget : public QListWidgetItem {
private:
    Qt::CheckState orig_state;
    QString prog_id;
    QString desc;
    QStringList extensions;
    UINT icon;
    bool hklm;
    bool remove;
public:
    AssocListWidget(const QString &text, QListWidget *parent, const QString &prog_id, const QString &desc,
                    const QString &exts, bool hklm, UINT icon);

    void Process(const QString &file_path);

    void SetRemove(int state);
};

class ZDLFileAssociations : public QDialog {
Q_OBJECT

private:
    QListWidget *assoc_list;
private slots:

    void ApplyAssociations();

    void ClearStateChanged(int state);

public:
    ZDLFileAssociations(QWidget *parent);
};

