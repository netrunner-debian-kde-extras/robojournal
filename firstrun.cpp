/*
    This file is part of RoboJournal.
    Copyright (c) 2012 by Will Kraft <pwizard@gmail.com>.


    RoboJournal is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RoboJournal is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with RoboJournal.  If not, see <http://www.gnu.org/licenses/>.
  */


#include "firstrun.h"
#include "ui_firstrun.h"
#include "buffer.h"
#include "journalcreator.h"
#include "ui_journalcreator.h"
#include "journalselector.h"
#include "ui_journalselector.h"
#include "config.h"
#include "ui_config.h"
#include "configmanager.h"
#include <iostream>
#include <QMessageBox>


FirstRun::FirstRun(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirstRun)
{
    using namespace std;
    ui->setupUi(this);

#ifdef unix
    QIcon unixicon(":/icons/robojournal-icon.png");
    this->setWindowIcon(unixicon);

#endif

    int width=this->width();
    int height=this->height();
    this->setMaximumSize(width,height);
    this->setMinimumSize(width,height);

    cout << "OUTPUT: Configuration not set. (or manual reconfiguration requested) Starting first-run tool..." << endl;

    // make the new journal option default
    ui->NewJournal->click();

    // hide question mark button in title bar when running on Windows
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

}

FirstRun::~FirstRun()
{
    delete ui;
}




void FirstRun::Launcher(){

    this->close();

    if(ui->NewJournal->isChecked()){
        this->close();
        JournalCreator j;
        j.exec();
    }
    else{
        this->close();
        JournalSelector s;
        s.exec();
        // old code, deprecated as of 0.3
//        // Get rid of first run flag
//        Buffer::firstrun=false;

//        // create a basic config, doesn't matter what it says right now. We just need to have a config file present
//        ConfigManager m;
//        m.RebuildConfig();

//        // and let the user change the database information
//        Config c;
//        c.ManualConfig();
//        c.exec();
    }
}

void FirstRun::on_buttonBox_accepted()
{
    Launcher();
}

void FirstRun::on_buttonBox_rejected()
{
    if(Buffer::firstrun){
        // get confirmation about quitting
        QMessageBox g;
        int choice=g.question(this,"RoboJournal","You have not finished the RoboJournal configuration process. "
                              "Do you really want to quit?", QMessageBox::Yes|QMessageBox::No,QMessageBox::No);

        if(choice==QMessageBox::Yes){
            exit(0);
        }
        else{
            // do nothing
        }

    }
    else{
        this->close();
    }
}
