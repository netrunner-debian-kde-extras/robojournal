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

#ifndef MYSQLCORE_H
#define MYSQLCORE_H

#include <QtSql/QSqlDatabase>

class MySQLCore
{
public:
    MySQLCore();

    bool Connect();
    void Disconnect();
    bool AddEntry();
    bool DeleteEntry(QString id);
    bool SanityCheck();
    QString RetrieveEntry(QString id);

    QList<QString> RetrieveEntryFull(QString id);


    QString TimeStamp(QString id);
    QString Recent();
    QString GetPrevious();
    QList<QString> getYear();
    QList<QString> getMonth(QString nextyear);
    QList<QString> getDay(QString itemmonth, QString nextyear);
    QList<QString> getEntries(QString itemday, QString itemmonth);
    QList<QString> Create_ID_List(int year_range);

    QList<QString> getEntriesMonth(QString month, QString year);

    bool CreateDatabase(QString host,QString root_pass, QString db_name, QString port, QString newuser, QString newuser_pass);

    bool Update(QString title, int month, int day, int year, QString tags, QString body, QString id);

    bool UpgradeJournal(QString root_pass);

    QString GetTitle(QString id);
    QString GetTimestamp(QString id);



    static int ID;

    static QString recordnum; // used to get title for most recent entry

    static QSqlDatabase db;
    static int dialogX;
    static int dialogY;

    static QString error_code;



private:

};

#endif // MYSQLCORE_H
