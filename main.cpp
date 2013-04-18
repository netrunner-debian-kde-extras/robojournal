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


#include <QtGui/QApplication>
#include "mainwindow.h"
#include "editor.h"
#include "buffer.h"
#include "configmanager.h"
#include <iostream>
#include <QtSql/QSqlDatabase>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    using namespace std;



    // Show start-up text in terminal.
    ConfigManager cm;
    cout << "=============================================="<< endl;
    cout << "\tSTARTING ROBOJOURNAL " << Buffer::version.toStdString() << endl;
    cout << "\n   \"Free Journal Software For Everyone\"" << endl;
    cout << "=============================================="<< endl;
    cout << "STAGE 1: LOADING CONFIGURATION DATA..." << endl;
    cout << "=============================================="<< endl;


   // Read Configuration
    cm.ReadConfig();



    // Show Main GUI
    MainWindow w;
    cout << "=============================================="<< endl;
    cout << "Stage 2: SHOWING MAIN WINDOW" <<endl;
    cout << "=============================================="<< endl;
    w.show();



    return a.exec();
}
