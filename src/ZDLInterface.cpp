/*
 * This file is part of qZDL
 * Copyright (C) 2007-2011  Cody Harris
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

#include <QApplication>
#include <QProcessEnvironment>
#include <QMessageBox>
#include <QMenu>
#include <QLabel>
#include <QFileDialog>
#include "ZDLConfigurationManager.h"
#include "ZDLAboutDialog.h"

#include "ZDLMultiPane.h"
#include "ZDLInterface.h"
#include "ZDLMainWindow.h"
#include "ZDLFilePane.h"
#include "ZDLSettingsPane.h"
#include "ZDLQSplitter.h"

#include "gph_dnt.xpm"
#include "gph_upt.xpm"

extern ZDLMainWindow *mw;

ZDLInterface::ZDLInterface(QWidget *parent) : ZDLWidget(parent) {
    LOGDATAO() << "New ZDLInterface" << Qt::endl;
    ZDLConfigurationManager::setInterface(this);

    box = new QVBoxLayout(this);

    QLayout *tpane = getTopPane();
    QLayout *bpane = getBottomPane();

    mpane = nullptr;
    setContentsMargins(4, 4, 4, 4);
    layout()->setContentsMargins(0, 0, 0, 0);

    box->addLayout(tpane);
    box->addLayout(bpane);
    box->setSpacing(2);
    LOGDATAO() << "Done creating interface" << Qt::endl;
}

QLayout *ZDLInterface::getTopPane() {
    auto *boxLayout = new QHBoxLayout();

    auto *split = new ZDLQSplitter(this);
    split->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    QSplitter *rsplit = split->getSplit();

    auto *fpane = new ZDLFilePane(rsplit);
    auto *spane = new ZDLSettingsPane(rsplit);

    split->addChild(fpane);
    split->addChild(spane);
    boxLayout->setSpacing(2);
    boxLayout->addWidget(rsplit);
    return boxLayout;
}

QLayout *ZDLInterface::getBottomPane() {
    auto *pBoxLayout = new QVBoxLayout();
    auto *box_inside_box = new QVBoxLayout();
    auto *ecla = new QLabel("Extra command line arguments", this);
    ecla->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
    extraArgs = new QLineEdit(this);
    QLayout *pan = getButtonPane();
    layout()->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);
    box_inside_box->addWidget(ecla);
    box_inside_box->addWidget(extraArgs);
    box_inside_box->setSpacing(2);
    pBoxLayout->addLayout(box_inside_box);
    pBoxLayout->addLayout(pan);
    pBoxLayout->setSpacing(4);
    return pBoxLayout;
}

void ZDLInterface::exitzdl() {
    LOGDATAO() << "Closing ZDL" << Qt::endl;
    mw->close();
}

void ZDLInterface::importCurrentConfig() {
    LOGDATAO() << "Asking if they'd really like to import" << Qt::endl;
    QString text(
            "Are you sure you'd like to <b>replace</b> the current <b>global</b> configurationw with the one currently loaded?");
    QMessageBox::StandardButton btnrc = QMessageBox::warning(this, "ZDL", text,
                                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (btnrc != QMessageBox::Yes) {
        LOGDATAO() << "They said no, bailing" << Qt::endl;
        return;
    }
    ZDLConfiguration *conf = ZDLConfigurationManager::getConfiguration();
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    QString userConfPath = conf->getPath(ZDLConfiguration::CONF_USER);
    QFileInfo userConf(userConfPath);
    if (!userConf.exists()) {
        LOGDATAO() << "File " << userConfPath << " doesn't exist" << Qt::endl;
        QDir dir = userConf.dir();
        if (!dir.exists()) {
            LOGDATAO() << "Nor does it's path" << Qt::endl;
            if (!dir.mkpath(dir.absolutePath())) {
                LOGDATAO() << "Couldn't create the path, bailing" << Qt::endl;
                QMessageBox::critical(this, "ZDL",
                                      QString("Unable to create the directories for a global user configuration at ")
                                      + dir.absolutePath());
                return;
            }
        }
    }
    LOGDATAO() << "Triggering write" << Qt::endl;
    mw->writeConfig();
    if (zconf->writeINI(userConfPath) != 0) {
        LOGDATAO() << "Couldn't write to configuration file " << userConfPath << Qt::endl;
        QMessageBox::critical(this, "ZDL", QString("Unable to write the configuration file at ") + userConfPath);
        return;
    }
    ZDLConfigurationManager::setConfigFileName(userConfPath);
    LOGDATAO() << "Triggering read" << Qt::endl;
    mw->startRead();
}

QLayout *ZDLInterface::getButtonPane() {
    auto *boxLayout = new QHBoxLayout();

    auto *btnExit = new QPushButton("Exit", this);
    btnZDL = new QPushButton("ZDL", this);
    btnEpr = new QPushButton(this);
    btnLaunch = new QPushButton("Launch", this);

    auto *context = new QMenu(btnZDL);
    auto *actions = new QMenu("Actions", context);

    QAction *showCommandline = actions->addAction("Show command line");
    QAction *clearAllPWadsAction = actions->addAction("Clear external file list");
    QAction *clearAllFieldsAction = actions->addAction("Clear all fields");
    clearAllFieldsAction->setShortcut(QKeySequence::New);
    QAction *clearEverythingAction = actions->addAction("Clear everything");
    actions->addSeparator();
#if !defined(NO_IMPORT)
    QAction *actImportCurrentConfig = actions->addAction("Import current config");
#endif
    QAction *clearCurrentGlobalConfig = actions->addAction("Clear current global config");
    clearCurrentGlobalConfig->setEnabled(false);

    context->addMenu(actions);
    context->addSeparator();
    QAction *loadZdlFileAction = context->addAction("Load .zdl");
    loadZdlFileAction->setShortcut(QKeySequence::Open);
    QAction *saveZdlFileAction = context->addAction("Save .zdl");
    saveZdlFileAction->setShortcut(QKeySequence::Save);
    context->addSeparator();
    QAction *loadAction = context->addAction("Load .ini");
    QAction *saveAction = context->addAction("Save .ini");
    context->addSeparator();
    QAction *aboutAction = context->addAction("About");
    aboutAction->setShortcut(QKeySequence::HelpContents);

    connect(loadAction, SIGNAL(triggered()), this, SLOT(loadConfigFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveConfigFile()));
    connect(loadZdlFileAction, SIGNAL(triggered()), this, SLOT(loadZdlFile()));
    connect(saveZdlFileAction, SIGNAL(triggered()), this, SLOT(saveZdlFile()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutClick()));
#if !defined(NO_IMPORT)
    connect(actImportCurrentConfig, SIGNAL(triggered()), this, SLOT(importCurrentConfig()));
#endif

    connect(clearAllPWadsAction, SIGNAL(triggered()), this, SLOT(clearAllPWads()));
    connect(clearAllFieldsAction, SIGNAL(triggered()), this, SLOT(clearAllFields()));
    connect(clearEverythingAction, SIGNAL(triggered()), this, SLOT(clearEverything()));

    connect(showCommandline, SIGNAL(triggered()), this, SLOT(showCommandline()));
    //connect(newDMFlagger, SIGNAL(triggered()),this,SLOT(showNewDMFlagger()));
    connect(btnExit, SIGNAL(clicked()), this, SLOT(exitzdl()));

    btnZDL->setMenu(context);

    int minBtnWidth = 50;

    btnExit->setMinimumWidth(minBtnWidth - 15);
    btnZDL->setMinimumWidth(minBtnWidth - 15);
    btnEpr->setMinimumWidth(20);
    btnLaunch->setMinimumWidth(minBtnWidth);

    btnExit->setMinimumHeight(26);
    btnZDL->setMinimumHeight(26);
    btnLaunch->setMinimumHeight(26);
    //26 is a cherry-picked value for Win32 system font
    //On other OSes we can have fonts with greater sizes so text buttons will be taller than 26 but glyph button will remain the same
    //To accomodate this, we set minimum height for glyph button dynamically
    btnEpr->setMinimumHeight(btnLaunch->sizeHint().height());

    connect(btnLaunch, SIGNAL(clicked()), this, SLOT(launch()));

    setContentsMargins(0, 0, 0, 0);
    layout()->setContentsMargins(0, 0, 0, 0);
    boxLayout->addWidget(btnExit, 1);
    boxLayout->addWidget(btnZDL, 1);
    boxLayout->addStretch(2);
    boxLayout->addWidget(btnEpr, 1);
    boxLayout->addWidget(btnLaunch, 1);
    boxLayout->setSpacing(4);
    connect(btnEpr, SIGNAL(clicked()), this, SLOT(mclick()));
    return boxLayout;
}

void ZDLInterface::clearAllPWads() {
    LOGDATAO() << "Clearing all PWads" << Qt::endl;
    mw->writeConfig();
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    ZDLSection *section = zconf->getSection("zdl.save");
    if (section) {
        QVector<ZDLLine *> vctr;
        section->getRegex("^file[0-9]+d?$", vctr);
        for (auto &i: vctr) {
            zconf->deleteValue("zdl.save", i->getVariable());
        }
    }
    mw->startRead();
}

void ZDLInterface::clearEverything() {
    LOGDATAO() << "Clearing everything question" << Qt::endl;
    QString text(
            "Warning!\n\nIf you proceed, you will lose <b>EVERYTHING</b>!\n All IWAD, PWAD, and source port settings will be wiped.\n\nWould you like to continue?");
    QMessageBox::StandardButton btnrc = QMessageBox::warning(this, "ZDL", text,
                                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (btnrc != QMessageBox::Yes) {
        return;
    }
    LOGDATAO() << "They said yes, clearing..." << Qt::endl;
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    ZDLConfigurationManager::setActiveConfiguration(new ZDLConf());
    delete zconf;
    LOGDATAO() << "Clearing done" << Qt::endl;
    mw->startRead();

}

void ZDLInterface::clearAllFields() {
    LOGDATAO() << "Clearing all of zdl.save" << Qt::endl;
    mw->writeConfig();
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    zconf->deleteSectionByName("zdl.save");
    mw->startRead();
    LOGDATAO() << "Complete" << Qt::endl;
}

void ZDLInterface::launch() {
    LOGDATAO() << "Launching" << Qt::endl;
    mw->launch();
}

void ZDLInterface::buttonPaneNewConfig() {
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    ZDLSection *section = zconf->getSection("zdl.save");
    if (section) {
        QVector<ZDLLine *> vctr;
        section->getRegex("^dlgmode$", vctr);
        for (auto &i: vctr) {
            if (i->getValue().compare("open", Qt::CaseInsensitive) == 0) {
                btnEpr->setIcon(QPixmap(glyph_down_trg));
            } else {
                btnEpr->setIcon(QPixmap(glyph_up_trg));
            }
        }
        if (vctr.empty()) {
            btnEpr->setIcon(QPixmap(glyph_down_trg));
        }
    } else {
        btnEpr->setIcon(QPixmap(glyph_down_trg));
    }

}

void ZDLInterface::mclick() {
    writeConfig();
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    int stat;
    if (zconf->hasValue("zdl.save", "dlgmode")) {
        QString txt = zconf->getValue("zdl.save", "dlgmode", &stat);
        if (txt == "closed") {
            btnEpr->setIcon(QPixmap(glyph_up_trg));
            zconf->setValue("zdl.save", "dlgmode", "open");
        } else {
            zconf->setValue("zdl.save", "dlgmode", "closed");
            btnEpr->setIcon(QPixmap(glyph_down_trg));
        }
    } else {
        btnEpr->setIcon(QPixmap(glyph_up_trg));
        zconf->setValue("zdl.save", "dlgmode", "open");
    }
    startRead();
}

void ZDLInterface::sendSignals() {
    rebuild();
    emit buildParent(this);
    emit buildChildren(this);
}

void ZDLInterface::saveConfigFile() {
    LOGDATAO() << "Saving config file" << Qt::endl;
    sendSignals();
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    QString filters =
            "INI files (*.ini);;"
            "All files (" QFD_FILTER_ALL ")";

    QString lastDir = getIniLastDir(zconf);
    QString fileName = QFileDialog::getSaveFileName(this, "Save configuration", lastDir, filters);

    if (!fileName.isNull() && !fileName.isEmpty()) {
        QFileInfo fi(fileName);
        if (!fi.fileName().contains(".")) {
            fileName += ".ini";
        }
        ZDLConfigurationManager::setConfigFileName(fileName);
        saveIniLastDir(fileName, zconf);
        zconf->writeINI(fileName);
        mw->startRead();
    }

}

void ZDLInterface::loadConfigFile() {
    LOGDATAO() << "Loading config file" << Qt::endl;
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    QString filters =
            "INI files (*.ini);;"
            "All files (" QFD_FILTER_ALL ")";

    QString lastDir = getIniLastDir(zconf);
    QString fileName = QFileDialog::getOpenFileName(this, "Load configuration", lastDir, filters);
    if (!fileName.isNull() && !fileName.isEmpty()) {
        QFileInfo fi(fileName);
        if (!fi.fileName().contains(".")) {
            fileName += ".ini";
        }
        delete zconf;
        zconf = nullptr;

        auto *tconf = new ZDLConf();
        ZDLConfigurationManager::setConfigFileName(fileName);
        tconf->readINI(fileName);
        saveIniLastDir(fileName, zconf);
        ZDLConfigurationManager::setActiveConfiguration(tconf);

        mw->startRead();
    }
}

void ZDLInterface::loadZdlFile() {
    LOGDATAO() << "Loading ZDL file" << Qt::endl;
    QString filters =
            "ZDL files (*.zdl);;"
            "All files (" QFD_FILTER_ALL ")";

    QString fileName = QFileDialog::getOpenFileName(this, "Load ZDL", getZdlLastDir(), filters);
    if (!fileName.isNull() && !fileName.isEmpty()) {
        ZDLConf *current = ZDLConfigurationManager::getActiveConfiguration();
        for (int i = 0; i < current->sections.size(); i++) {
            if (current->sections[i]->getName().compare("zdl.save") == 0) {
                ZDLSection *section = current->sections[i];
                current->sections.remove(i);
                delete section;
                break;
            }
        }
        auto *newConf = new ZDLConf();
        newConf->readINI(fileName);
        for (auto &i: newConf->sections) {
            if (i->getName().compare("zdl.save") == 0) {
                ZDLSection *section = i;
                current->addSection(section->clone());
                break;
            }
        }
        saveZdlLastDir(fileName);
        delete newConf;
        mw->startRead();
    }
}

void ZDLInterface::saveZdlFile() {
    LOGDATAO() << "Saving ZDL File" << Qt::endl;
    sendSignals();
    QString filters =
            "ZDL files (*.zdl);;"
            "All files (" QFD_FILTER_ALL ")";

    QString fileName = QFileDialog::getSaveFileName(this, "Save ZDL", getZdlLastDir(), filters);
    if (!fileName.isNull() && !fileName.isEmpty()) {
        ZDLConf *current = ZDLConfigurationManager::getActiveConfiguration();
        auto *copy = new ZDLConf();
        for (auto &section: current->sections) {
            if (section->getName().compare("zdl.save") == 0) {
                copy->addSection(section->clone());
                if (!fileName.contains(".")) {
                    fileName = fileName + QString(".zdl");
                }
                saveZdlLastDir(fileName);
                copy->writeINI(fileName);
            }
        }
    }

}

void ZDLInterface::aboutClick() {
    LOGDATAO() << "Opening About dialog" << Qt::endl;
    ZDLAboutDialog zad(this);
    zad.exec();
}

void ZDLInterface::showCommandline() {
    LOGDATAO() << "Showing command line" << Qt::endl;
    writeConfig();

    QString exec = mw->getExecutable();

    if (exec.isEmpty()) {
        QMessageBox::critical(this, "ZDL", "Please select a source port");
        return;
    }

    QFileInfo exec_fi(exec);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Command line and environment");
    QString dwd;
    QString args = mw->getArgumentsString();
    if (args.length())
        args = "\n\nArguments: " + args;
    if (QProcessEnvironment::systemEnvironment().contains("DOOMWADDIR"))
        dwd = "\n\nDOOMWADDIR: "
              + QDir::fromNativeSeparators(QProcessEnvironment::systemEnvironment().value("DOOMWADDIR"));
    msgBox.setText(
            "Executable: " + exec_fi.fileName() + args + "\n\nWorking directory: " + exec_fi.canonicalPath() + dwd);
    msgBox.setStandardButtons(QMessageBox::Cancel);
    QPushButton *launch_btn = msgBox.addButton("Execute", QMessageBox::AcceptRole);
    msgBox.setDefaultButton(launch_btn);
    msgBox.setEscapeButton(QMessageBox::Cancel);
    msgBox.exec();

    if (msgBox.clickedButton() == launch_btn) {
        LOGDATAO() << "Asked to launch" << Qt::endl;
        mw->launch();
        return;
    }
    LOGDATAO() << "Done" << Qt::endl;
}

void ZDLInterface::rebuild() {
    LOGDATAO() << "Saving config" << Qt::endl;
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    if (extraArgs->text().length() > 0) {
        zconf->setValue("zdl.save", "extra", extraArgs->text());

    } else {
        zconf->deleteValue("zdl.save", "extra");
    }
}

void ZDLInterface::bottomPaneNewConfig() {
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    if (zconf->hasValue("zdl.save", "extra")) {
        int stat;
        QString rc = zconf->getValue("zdl.save", "extra", &stat);
        if (rc.length() > 0) {
            extraArgs->setText(rc);
        }
    } else {
        extraArgs->setText("");
    }
}

//Called when there's a change to the configuration that we need to look at.
//The button changed the configuration, and then notifies us that we need
//to look at the configuration to see what we need to do.
void ZDLInterface::newConfig() {
    LOGDATAO() << "Loading new config" << Qt::endl;
    buttonPaneNewConfig();
    bottomPaneNewConfig();
    //Grab our configuration
    ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
    //Grab our section in the configuration
    ZDLSection *section = zconf->getSection("zdl.save");
    //Do we have it?
    if (section && !section->findVariable("dlgmode").compare("open", Qt::CaseInsensitive)) {
        if (mpane == nullptr) {
            mpane = new ZDLMultiPane(this);
            mpane->setLaunchButton(btnLaunch);
            mpane->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum));
            mpane->newConfig();
        }
        box->addWidget(mpane);
        mpane->setVisible(true);
    } else {
        if (mpane) {
            mpane->setVisible(false);
            box->removeWidget(mpane);
        }
        bool ok;
        if (section && section->findVariable("gametype").toInt(&ok, 10)) {
            if (section->findVariable("players").toInt(&ok, 10))
                btnLaunch->setText("Host");
            else
                btnLaunch->setText("Join");
        } else {
            btnLaunch->setText("Launch");
        }
    }
    this->update();
}

void ZDLInterface::startRead() {
    emit readChildren(this);
    newConfig();
}

void ZDLInterface::writeConfig() {
    rebuild();
    emit buildChildren(this);
}
