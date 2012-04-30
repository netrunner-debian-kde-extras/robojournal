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

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QtGui/QApplication>
#include <QDir>

class ConfigManager
{
public:
    ConfigManager();
    void ReadConfig();
    void WriteConfig();
    void RebuildConfig();
    void UpdateConfig();

    void CustomSetup(QString host,QString db_name, QString port, QString newuser);


    QString * pDUser;
    QString * pDefaultHost;


    QString defaulthost; // default host
    QString defaultuser; // default user
    QString defaultdatabase; // default db
    QString backend; // database backend var

    int entryrange; // number of years to show
    QString toolbar_pos; // Toolbar position... 0=left; 1=top; 2=right

    bool alwaysusedefaults; // always use default values if true
    bool allowroot; // allow root logins if true
    bool allentries; // show all entries if true
    bool showwarnings; // show warning dialogs if true

private:



};

#endif // CONFIGMANAGER_H
