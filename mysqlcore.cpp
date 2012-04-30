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


#include "mysqlcore.h"
#include "buffer.h"
#include <iostream>
#include <QtSql/QSqlDatabase>
#include <QtSql/QMYSQLDriver>
#include <QSqlQuery>
#include <QMessageBox>
#include "editor.h"
#include <QSqlResult>
#include <QStringList>
#include <QDebug>
#include <QTime>


QSqlDatabase MySQLCore::db;
int MySQLCore::dialogX;
int MySQLCore::dialogY;
int MySQLCore::ID;
QString MySQLCore::error_code;

QString MySQLCore::recordnum;
//
MySQLCore::MySQLCore()
{
}


// upgrade journal to RoboJournal 0.2 compatible version by adding [time varchar(5)] column
bool MySQLCore::UpgradeJournal(QString root_pass){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(Buffer::host);
    db.setDatabaseName(Buffer::database_name);
    db.setPort(Buffer::databaseport.toInt());
    db.setUserName("root");
    db.setPassword(root_pass);
    db.open();

    QSqlQuery q("ALTER TABLE entries ADD time VARCHAR(5)");
    bool success=q.exec();



    db.close();

    return success;
}



// make sure the current database is "sane" (has the right structure) before attempting to use it.

bool MySQLCore::SanityCheck(){
    using namespace std;
    db.open();
    QString query="DESCRIBE entries";
    QSqlQuery q(query);
    q.exec();
    bool is_sane=false;

    QString pattern="idtitlemonthdayyeartagsbodytime";
    QList<QString> match;
    QString assembler;

    QString output;
    while(q.next()){

        QVariant v=q.value(0);
        output=v.toString();
        //qDebug() << output;
        int pos=0;
        match.append(output);
        pos++;


    }

    for(int m=0; m<match.length(); m++){
        assembler.append(match[m]);

    }

    // Only allow the database to load if the table structure matches the
    // predefined pattern. Other databases are invalid and very bad things will happen if we
    // let RJ use them.
    if(assembler==pattern){
        is_sane=true;
        cout << "OUTPUT: Database " << Buffer::database_name.toStdString() <<
                " passed database sanity check, proceeding with load..." << endl;
    }
    else{

        // triggered if user loads an old journal.
        if(assembler=="idtitlemonthdayyeartagsbody"){
            Buffer::needs_upgrade=true;
        }

    }

    return is_sane;
}


// Connect to the database
bool MySQLCore::Connect(){

    using namespace std;
    cout << "OUTPUT: Attempting MySQL database connection on \"" << Buffer::host.toStdString()
         << "\" as user \"" << Buffer::username.toStdString() << "\"...";

    QString host=Buffer::host;
    QString database=Buffer::database_name;
    QString user=Buffer::username;
    QString password=Buffer::password;
    int port=Buffer::databaseport.toInt();

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setPort(port);
    db.setUserName(user);
    db.setDatabaseName(database);
    db.setPassword(password);
    bool success=db.open();

    // The idea here just to see if we can create a connection at all.
    // if success returns true, the login form class DBLogin proceeds and the rest of the
    // program is unlocked. Other parts of the program can then call MysqlCore directly as needed.

    if(success){
        cout << "SUCCESS!"<< endl;
        db.close();


    }
    else{
        cout << "FAILED!" << endl;
        db.close();
    }

    return success;
}


void MySQLCore::Disconnect(){
    using namespace std;
    cout << "OUTPUT: Attempting to close MySQL connection...";

    db.removeDatabase("QMYSQL");
    db.close();

    cout << "Done!" << endl;


}

// Update a row with new data. This is invoked from the Editor class if editmode==true.
bool MySQLCore::Update(QString title, int month, int day, int year, QString tags, QString body, QString id){
    using namespace std;
    db.open();

    QSqlQuery update("UPDATE entries set title=?, month=?, day=?, year=?, tags=?, body=? WHERE id=" + id);
    update.bindValue(0, title);
    update.bindValue(1, month);
    update.bindValue(2, day);
    update.bindValue(3, year);
    update.bindValue(4, tags);
    update.bindValue(5, body);

    bool success=update.exec();
    db.close();

    return success;
}



bool MySQLCore::AddEntry(){
    using namespace std;
    db.open();

    QString title=Editor::title;
    int month=Editor::month;
    int day=Editor::day;
    int year=Editor::year;
    QString tags="Null";
    QString body=Editor::body;


    bool success;
    // if the user opts to record time, write extra time field
    if(Buffer::keep_time){
        QString insert="INSERT INTO entries(title, month, day, year, tags, body, time) VALUES(?,?,?,?,?,?,?)";
        QSqlQuery addentry(insert);
        addentry.bindValue(0, title);
        addentry.bindValue(1, month);
        addentry.bindValue(2, day);
        addentry.bindValue(3, year);
        addentry.bindValue(4, tags);
        addentry.bindValue(5, body);

        // get current system time in 24 hour style
        QTime t=QTime::currentTime();
        QString timestamp=t.toString("HH:mm");
        addentry.bindValue(6, timestamp);
        //cout << "OUTPUT: Current timestamp: " << timestamp.toStdString() << endl;
        success=addentry.exec();

        db.close();

    }
    else{
        QString insert="INSERT INTO entries(title, month, day, year, tags, body) VALUES(?,?,?,?,?,?)";
        QSqlQuery addentry(insert);
        addentry.bindValue(0, title);
        addentry.bindValue(1, month);
        addentry.bindValue(2, day);
        addentry.bindValue(3, year);
        addentry.bindValue(4, tags);
        addentry.bindValue(5, body);
        success=addentry.exec();

        db.close();
    }






    return success;

}

bool MySQLCore::DeleteEntry(QString id){

    db.open();
    QString query="DELETE FROM entries WHERE id=" + id;
    QSqlQuery q(query);

    bool success=q.exec();
    return success;
}

// Get day and year based on entry
QString MySQLCore::TimeStamp(QString id){
    using namespace std;
    db.open();
    QString query="SELECT month,day,year FROM entries WHERE id=" + id;
    //cout << query.toStdString() << endl;
    QSqlQuery q(query);

    q.exec();
    QString month;
    QString day;
    QString year;
    QString timestamp;
    while(q.next()){
        QVariant Vmonth=q.value(0);
        month=Vmonth.toString();

        QVariant Vday=q.value(1);
        day=Vday.toString();

        QVariant Vyear=q.value(2);
        year=Vyear.toString();

        switch(Buffer::date_format.toInt()){
        case 0: // European

            timestamp= day + "-" + month + "-" + year;
            break;

        case 1: // USA Style
            timestamp= month + "-" + day + "-" + year;
            break;
        case 2: // ISO
            timestamp= year + "-" + month + "-" + day;

            break;
        }


        //cout << entry.toStdString() << endl;
    }

    db.close();

    return timestamp;
}


// Function that retrieves entry $index from the database and
// returns data from "body" as a QString. This is meant to be called from the EntryList tree and
// index should be the value from the second hidden column.
QString MySQLCore::RetrieveEntry(QString id){
    using namespace std;
    db.open();
    QString query="SELECT body FROM entries WHERE id=" + id;
    //cout << query.toStdString() << endl;
    QSqlQuery q(query);

    q.exec();
    QString entry;
    while(q.next()){
        QVariant result=q.value(0);
        entry=result.toString();
        //cout << entry.toStdString() << endl;
    }

    db.close();

    return entry;
}

/*

 Retrieve full entry from database to populate Editor form. This function is ONLY
 used for update functions.
  */

QList<QString> MySQLCore::RetrieveEntryFull(QString id){
    using namespace std;
    db.open();
    QString query="SELECT title, month, day, year, tags, body FROM entries WHERE id=" + id;
    //cout << query.toStdString() << endl;

    QSqlQuery q(query);

    q.exec();

    db.close();

    QList<QString> EntryArray;

    while(q.next()){
        QVariant t=q.value(0);
        QString title=t.toString();
        QString month=q.value(1).toString();
        QString day=q.value(2).toString();
        QString year=q.value(3).toString();
        QString tags=q.value(4).toString();
        QString body=q.value(5).toString();

        EntryArray.append(title);
        EntryArray.append(month);
        EntryArray.append(day);
        EntryArray.append(year);
        EntryArray.append(tags);
        EntryArray.append(body);

    }

    return EntryArray;
}


// Get the most recent entry from the db and return it as a QString
QString MySQLCore::Recent(){

    db.open();
    QString q="SELECT id,body FROM entries ORDER BY id DESC";
    QSqlQuery get;
    get.prepare(q);
    get.exec();
    get.first();
    QVariant result=get.value(1);

    QVariant id=get.value(0);

    QString body=result.toString();
    ID=id.toInt();


    recordnum=id.toString();

    db.close();

    return body;
}

QString MySQLCore::GetPrevious(){
    db.open();
    ID--;
    QString q="SELECT body FROM entries where id=?";

    QSqlQuery get(q);
    get.bindValue(0, ID);
    get.exec();
    QVariant result=get.value(0);

    QString body=result.toString();
    db.close();
    return body;

}


QList<QString> MySQLCore::getYear(){
    using namespace std;
    QList <QString> YearList;

    db.open();
    QString query="SELECT year FROM entries ORDER BY id DESC";
    QSqlQuery getyear(query);
    getyear.exec();

    int pos=0;
    QString recent;

    // Add all years to YearList
    if(Buffer::allentries){

        while(getyear.next()){

            QVariant result=getyear.value(0);
            QString year=result.toString();

            if(year!=recent){

                //cout << "Added Year: " << year.toStdString() << endl;
                YearList.insert(pos,year);
                pos++;
                recent=year;
            }
        }

    }

    // Only add entries from a specific year range to YearList
    else{

        int max_range=Buffer::entryrange.toInt();
        //cout << "Entry range: " << max_range << endl;
        int range=0;

        while(getyear.next()){

            QVariant result=getyear.value(0);
            QString year=result.toString();

            if(year!=recent){

                //cout << "Added Year: " << year.toStdString() << endl;
                YearList.insert(pos,year);
                pos++;
                range++;

                // force the loop to stop after we get the year range we want
                if(range==max_range){
                    break;
                }

                recent=year;

            }
        }

    }

    db.close();

    return YearList;
}

QList<QString> MySQLCore::getMonth(QString nextyear){
    using namespace std;
    QList <QString> MonthList;
    db.open();
    QString query="SELECT month FROM entries WHERE year=" + nextyear +  " ORDER BY id DESC";
    QSqlQuery getmonth(query);
    getmonth.exec();


    int pos=0;
    QString recent;
    while(getmonth.next()){
        QVariant result=getmonth.value(0);
        QString month=result.toString();

          if(month!=recent){
            //cout << "Inserting month: " << month.toStdString() << endl;
            MonthList.append(month);
            pos++;
        }
        recent=month;

    }
    db.close();
    return MonthList;
}

QList<QString> MySQLCore::getDay(QString itemmonth, QString nextyear){
    using namespace std;
    QList <QString> DayList;

    db.open();
    QString query="SELECT day FROM entries WHERE MONTH=" + itemmonth + " AND year=" + nextyear + " ORDER BY id DESC";
    //cout << query.toStdString() << endl;
    QSqlQuery getday(query);
    getday.exec();
    int pos=0;
    QString previous;
    while(getday.next()){
        QVariant result=getday.value(0);
        QString day=result.toString();

        if(day!=previous){
            //cout << "Inserting day: " << day.toStdString() << endl;
            DayList.insert(pos,day);
        }
        pos++;
        previous=day;

    }
    db.close();

    return DayList;

}

QList<QString> MySQLCore::getEntries(QString itemday, QString itemmonth){
    db.open();
    QList <QString> EntryList;

    // Even though we want to display results from newest-> oldest,
    // we sort by ascending here because QList::Insert adds them in reverse order.
    // running ORDER BY id DESC will actually cause entries to be sorted by ascending order.

    QString query="SELECT title,id FROM entries WHERE day=" + itemday + " AND month=" +
            itemmonth + " ORDER BY id ASC";
    QSqlQuery getentry(query);


    getentry.exec();

    int pos=0;

    while(getentry.next()){
        QVariant result=getentry.value(0);
        QString entry=result.toString();

        QVariant rownum=getentry.value(1);
        QString id=rownum.toString();


        QString previous=NULL;

        entry=entry+"|"+id;




        if(entry!=previous){
            EntryList.insert(pos,entry);
        }
        previous=entry;
    }
    db.close();

    return EntryList;

}

// get and process timestamp
QString MySQLCore::GetTimestamp(QString id){

    using namespace std;
    db.open();
    QString query="SELECT time FROM entries WHERE id=" + id;
    //cout << query.toStdString() << endl;
    QSqlQuery q(query);

    q.exec();
    QString timestamp="test";
    while(q.next()){
        QVariant result=q.value(0);
        timestamp=result.toString();

    }

    // Process null times
    if(timestamp==""){
        timestamp="[unknown time]";
    }
    else{
        // default time is 24 hour style. If user wants AM/PM, we must convert it
        if(!Buffer::use_24_hour){
            QTime time=QTime::fromString(timestamp,"HH:mm");
            timestamp=time.toString("h:mm ap");

        }
    }

    //cout << "Timestamp: " + timestamp.toStdString() << endl;
    return timestamp;
}


// Get title from database
QString MySQLCore::GetTitle(QString id){
    using namespace std;
    db.open();
    QString query="SELECT title FROM entries WHERE id=" + id;
    //cout << query.toStdString() << endl;
    QSqlQuery q(query);

    q.exec();
    QString title;
    while(q.next()){
        QVariant result=q.value(0);
        title=result.toString();
        //cout << entry.toStdString() << endl;
    }

    db.close();

    return title;
}


// This creates the list of all entries in the database so we can browse backward and forward

QList<QString> MySQLCore::Create_ID_List(int year_range){
    using namespace std;
    QList <QString> IDList;
    db.open();
    QString query="SELECT id FROM entries ORDER BY id ASC";
    //cout << "Creating id list";
    QSqlQuery id(query);
    id.exec();
    year_range=0;  // not used, so set to null for now

    int pos=0;
    while(id.next()){
        QVariant result=id.value(0);
        QString entry=result.toString();
        //cout << entry.toStdString() << endl;
        IDList.insert(pos, entry);
        pos++;
    }
    db.close();

    return IDList;
}

/*

  MySQLcore::getEntriesMonth() is only called from MainWindow if Buffer::sortbyday==false.
  This function sorts all entries by the month they were made and returns a list.
  The default RJ behavior is to sort entries by month AND day instead.

*/


QList<QString> MySQLCore::getEntriesMonth(QString month, QString year){
    using namespace std;
    QList <QString> EntryList;
    db.open();
    QString query="SELECT id,title,day FROM entries WHERE month=" +
            month + " AND year="+ year +" ORDER BY ID DESC";
    QSqlQuery b(query);
    b.exec();

    int pos=0;
    while(b.next()){
        QVariant id=b.value(0);
        QString StrID=id.toString();

        QVariant title=b.value(1);
        QString StrTitle=title.toString();

        QVariant day=b.value(2);
        QString StrDay=day.toString();

        QString item=StrID + "," + StrTitle + "," + StrDay;
        EntryList.insert(pos, item);
        pos++;
    }

    return EntryList;
}


/*
  *THE* most important function, for without this none of the other MySQL commands are of any use whatsoever
  unless you know how to set everything up yourself. This function uses the root account on $host to
  create the $journal database, create the ENTRIES table in $database, and create a new user account with access
  permissions for $journal. Wow.
  */

bool MySQLCore::CreateDatabase(QString host,QString root_pass, QString db_name,
                               QString port, QString newuser, QString newuser_pass){
    using namespace std;



    int db_port=port.toInt();

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setPort(db_port);
    db.setUserName("root");
    db.setPassword(root_pass);
    db.open();

    // create database
    QSqlQuery create_db("CREATE DATABASE IF NOT EXISTS " + db_name);
    bool success1=create_db.exec();
    if(success1){
        cout << "OUTPUT: Successfully created new journal "  << db_name.toStdString() << endl;
    }
    else{
        cout << "ERROR: Could not create " << db_name.toStdString() << endl;
    }
    db.close();


    db.setDatabaseName(db_name);
    db.open();
    // create the entries table now

    QSqlQuery create_entries("CREATE TABLE entries(id int not null auto_increment, "
                             "primary key (id), title VARCHAR(500), month VARCHAR(2), "
                             "day VARCHAR(2), year VARCHAR(4), tags VARCHAR(300), "
                             "body TEXT, time VARCHAR(5))");


    bool success2=create_entries.exec();
    if(success2){
        cout << "OUTPUT: Successfully created new ENTRIES table on journal " << db_name.toStdString() << endl;
    }
    else{
        cout << "ERROR: Could not create ENTRIES table on " << db_name.toStdString() << endl;
    }
    db.close();

    // Grant rights to new user
    db.open();
    QString grant="GRANT SELECT, INSERT, DROP, UPDATE, DELETE ON " + db_name +
            ".* TO " + newuser + "@'" + host + "' IDENTIFIED BY '" + newuser_pass + "'";

    QSqlQuery grant_rights(grant);
    //cout << grant.toStdString() << endl;
    bool success3=grant_rights.exec();

    if(success3){
        cout << "OUTPUT: Rights granted to new user " << newuser.toStdString() <<
                " on journal " << db_name.toStdString() << endl;
    }
    else{
        cout << "ERROR: Could not grant rights to user " <<  newuser.toStdString() <<
                " on journal " << db_name.toStdString() << endl;
    }

    db.close();


    bool success=true;
    if(db.isOpenError()){
        success=false;
    }


    return success;
}
