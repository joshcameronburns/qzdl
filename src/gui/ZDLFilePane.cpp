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

#include <QApplication>
#include <QLabel>
#include "ZDLListWidget.h"
#include "ZDLFilePane.h"

ZDLFilePane::ZDLFilePane(QWidget* parent) : ZDLWidget(parent)
{
	auto* box = new QVBoxLayout(this);
	box->setSpacing(2);

	box->addWidget(new QLabel("External files", this));

	fList = new ZDLFileList(this);
	fList->doDragDrop(true);
	box->addWidget(fList);
	setContentsMargins(0, 0, 0, 0);
	layout()->setContentsMargins(0, 0, 0, 0);
}

void ZDLFilePane::rebuild()
{
}
