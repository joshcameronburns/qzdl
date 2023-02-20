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

#include <QtGui>
#include <QObject>
#include <QListWidget>
#include <QComboBox>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include "ZDLWidget.h"

class ZDLSettingsPane : public ZDLWidget
{
 Q_OBJECT
 public:
	explicit ZDLSettingsPane(QWidget* parent = nullptr);
	void rebuild() override;
	void newConfig() override;
 protected slots:
	void currentRowChanged(int);
	void reloadMapList();
	void VerbosePopup();
	void HidePopup();
 protected:
	static QStringList getFilesMaps();
	QComboBox* diffList;
	QComboBox* sourceList;
	QListWidget* IWADList;
	QComboBox* warpCombo;
	static bool naturalSortLess(const QString& lm, const QString& rm);
};

class AlwaysFocusedDelegate : public QItemDelegate
{
 Q_OBJECT
 public:
	explicit AlwaysFocusedDelegate(QObject* parent = nullptr) : QItemDelegate(parent){}
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

class DeselectableListWidget : public QListWidget
{
 Q_OBJECT
 public:
	explicit DeselectableListWidget(QWidget* parent = nullptr) : QListWidget(parent){}
	void mousePressEvent(QMouseEvent* event) override;
};

