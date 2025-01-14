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
#include <QFileDialog>
#include "ZDLSourcePortList.h"
#include "ZDLNameListable.h"
#include "ZDLConfigurationManager.h"
#include "ZDLNameInput.h"
#include "ZDLFileInfo.h"
#include "gph_ast.xpm"

#if defined(_WIN32)
const QString src_filters = "Executables (*.exe);;All files (*.*)";
#elif defined(Q_WS_MAC)
const QString src_filters = "Applications (*.app);;All files (*)";
#else
const QString src_filters = "All files (*)";
#endif

ZDLSourcePortList::ZDLSourcePortList(ZDLWidget *parent) : ZDLListWidget(parent) {
    auto *btnWizardAdd = new QPushButton(this);
    btnWizardAdd->setIcon(QPixmap(glyph_asterisk));
    btnWizardAdd->setToolTip("Add and name item");
    buttonRow->insertWidget(0, btnWizardAdd);

    QObject::connect(btnWizardAdd, SIGNAL(clicked()), this, SLOT(wizardAddButton()));
}

void ZDLSourcePortList::wizardAddButton() {
    ZDLAppInfo zdl_fi;
    ZDLNameInput diag(this, getSrcLastDir(), &zdl_fi, false, true);
    diag.setWindowTitle("Add source port");
    diag.setFilter(src_filters);
    if (diag.exec()) {
        saveSrcLastDir(diag.getFile());
        insert(new ZDLNameListable(pList, 1001, diag.getFile(), diag.getName()), -1);
    }
}

void ZDLSourcePortList::newConfig() {
    pList->clear();
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    ZDLSection *section = zconf->getSection("zdl.ports");
    if (section) {
        QVector<ZDLLine *> fileVctr;
        section->getRegex("^p[0-9]+f$", fileVctr);

        for (auto &i: fileVctr) {
            QString value = i->getVariable();

            QString number = "^p";
            number.append(value.mid(1, value.length() - 2));
            number.append("n$");

            QVector<ZDLLine *> nameVctr;
            section->getRegex(number, nameVctr);
            if (nameVctr.size() == 1) {
                QString disName = nameVctr[0]->getValue();
                QString fileName = i->getValue();
                auto *zList = new ZDLNameListable(pList, 1001, fileName, disName);
                insert(zList, -1);
            }
        }
    }
}

void ZDLSourcePortList::rebuild() {
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    ZDLSection *section = zconf->getSection("zdl.ports");
    if (section) {
        zconf->deleteSection("zdl.ports");
    }

    for (int i = 0; i < count(); i++) {
        QListWidgetItem *itm = pList->item(i);
        auto *fitm = (ZDLNameListable *) itm;
        QString sid = QString("p%1n").arg(i);
        zconf->setValue("zdl.ports", sid, fitm->getName());
        sid[sid.size() - 1] = 'f';
        zconf->setValue("zdl.ports", sid, fitm->getFile());
    }
}

void ZDLSourcePortList::newDrop(const QStringList &fileList) {
    LOGDATAO() << "newDrop" << Qt::endl;
    for (const QString &i: fileList)
        insert(new ZDLNameListable(pList, 1001, i, ZDLAppInfo(i).GetFileDescription()), -1);
}

void ZDLSourcePortList::addButton() {
    LOGDATAO() << "Adding new source ports" << Qt::endl;

    QStringList fileNames = QFileDialog::getOpenFileNames(this, "Add source ports", getSrcLastDir(), src_filters);
    for (const QString &fileName: fileNames) {
        LOGDATAO() << "Adding file " << fileName << Qt::endl;
        saveSrcLastDir(fileName);
        insert(new ZDLNameListable(pList, 1001, fileName, ZDLAppInfo(fileName).GetFileDescription()), -1);
    }
}

void ZDLSourcePortList::editButton(QListWidgetItem *item) {
    if (item) {
        auto *zitem = (ZDLNameListable *) item;
        ZDLAppInfo zdl_fi;
        ZDLNameInput diag(this, getSrcLastDir(), &zdl_fi, false, true);
        diag.setWindowTitle("Edit source port");
        diag.setFilter(src_filters);
        diag.basedOff(zitem);
        if (diag.exec()) {
            saveSrcLastDir(diag.getFile());
            zitem->setDisplayName(diag.getName());
            zitem->setFile(diag.getFile());
        }
    }
}
