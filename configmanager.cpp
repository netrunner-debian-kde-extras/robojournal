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


#include "configmanager.h"
#include <config.h>
#include <ui_config.h>
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <stdlib.h>
#include <buffer.h>
#include <QErrorMessage>
#include <QMessageBox>
#include "newdatabase.h"
#include <QAbstractButton>
#include "firstrun.h"
#include "ui_firstrun.h"


// get user home directory
QString homedir=QDir::homePath();
QChar separator=QDir::separator();
QString configdir=".robojournal";
QDir directory;

// Config-specific:
QString Config::new_allow_root;
QString Config::new_entry_range;
QString Config::new_always_use_defaults;
QString Config::new_dbtype;
QString Config::new_default_db;
QString Config::new_default_host;
QString Config::new_default_port;
QString Config::new_default_user;
QString Config::new_show_all_entries;
QString Config::new_show_confirm;
QString Config::new_toolbar_pos;
QString Config::new_sort_by_day;
QString Config::new_alternate_rows;
QString Config::new_use_background;
QString Config::new_show_title;
QString Config::new_use_custom_theme;
QString Config::new_tile_bg;
QString Config::new_use_background_in_tree;
QString Config::new_text_hexcolor;
QString Config::new_font_face;
QString Config::new_background_image;
QString Config::new_font_size;
QString Config::new_record_time;
QString Config::new_use_local_time;
QString Config::new_24_hr;
QString Config::new_rich_text;
QString Config::new_date_mode;
QString Config::new_icon_labels;


//###################################################################################################
// Config home dir setup
QString configpath=homedir+separator+configdir;


// config file name
QString config_name=homedir+separator+configdir+separator+"robojournal.cfg";
QFile configfile(config_name);


ConfigManager::ConfigManager()
{
}

// Function that loads config data into buffer on startup

void ConfigManager::ReadConfig(){
    using namespace std;
    QString var;

    cout << "Output: Using home path: " + configpath.toStdString() << endl;


    // Check for config file when program starts. If it's not there, rebuild it.
    if(!configfile.exists()){
        cout << "ERROR: Config file not found, rebuilding..." << endl;

        // give the journal creator a way to know its the first run
        Buffer::firstrun=true;

        // Launch the firstrun tool

        FirstRun f;
        f.exec();

        // Don't use the old RebuildConfig anymore now that the FirstRun tool is ready
        //RebuildConfig();


        //cout << "Done!" << endl;
    }

    cout << "OUTPUT: Reading data from config file robojournal.cfg...";


    if (!configfile.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

    int linenum=0;

    while(!configfile.atEnd()){
        linenum++;
        QString thisline=configfile.readLine();
       //cout << thisline.toStdString();


        if(linenum==3){
            //int * location;
            var=thisline.remove(0,17);
            int toolbar_pos=var.toInt();
            Buffer::toolbar_pos=toolbar_pos;
        }

        if(linenum==4){
            var=thisline.remove(0, 18);
            //cout << "Allow root: " << var.toStdString();
            if(var.trimmed()=="1"){
                Buffer::allowroot=true;

            }
            else{
                Buffer::allowroot=false;

            }

        }

        if(linenum==5){
            var=thisline.remove(0, 17);
            //cout << "Allow all entries: " << var.toStdString();
            if(var.trimmed()=="0"){
                Buffer::allentries=false;

            }
            else{
                Buffer::allentries=true;

            }
        }

        if(linenum==6){
            var=thisline.remove(0, 20);
            //cout << "Always use defaults: " << var.toStdString();
            if(var.trimmed()=="0"){
                Buffer::alwaysusedefaults=false;
                //cout << "defaults are false";
            }
            else{
                Buffer::alwaysusedefaults=true;
                //cout << "defaults are true";
            }

        }

        if(linenum==7){
            var=thisline.remove(0, 8);
            //cout << "Database type: " << var.toStdString();
            Buffer::backend=var.trimmed();
        }

        if(linenum==8){
            var=thisline.remove(0, 14);
            //cout << "Always warn: " << var.toStdString();
            if(var.trimmed()=="0"){
                Buffer::showwarnings=false;
                //cout << "defaults are false";
            }
            else{
                Buffer::showwarnings=true;
                //cout << "defaults are true";
            }
        }

        if(linenum==9){
            var=thisline.remove(0, 12);
            //cout << "Years to show: " << var.toStdString();
            Buffer::entryrange=var.trimmed();
        }

        if(linenum==10){
            var=thisline.remove(0, 13);
            //cout << "Default host: " << var.toStdString();
            Buffer::defaulthost=var.trimmed();

        }

        if(linenum==11){
            var=thisline.remove(0, 11);
            //cout << "Default database: " << var.toStdString();
            Buffer::defaultdatabase=var.trimmed();
        }

        if(linenum==12){
            var=thisline.remove(0, 13);
            //cout << "Default user: " << var.toStdString();
            Buffer::defaultuser=var.trimmed();
        }

        if(linenum==13){
            var=thisline.remove(0,8);
            Buffer::databaseport=var.trimmed();
        }

        // sort by day
        if(linenum==14){
            var=thisline.remove(0,12);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="0"){
                Buffer::sortbyday=false;
            }
            else{
                 Buffer::sortbyday=true;
            }
        }

        // Use alternate colors
        if(linenum==15){
            var=thisline.remove(0,17);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="0"){
                Buffer::alternate_rows=false;
            }
            else{
                 Buffer::alternate_rows=true;
            }
        }

        // Use background
        if(linenum==16){
            var=thisline.remove(0,15);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="0"){
                Buffer::use_background=false;
            }
            else{
                 Buffer::use_background=true;
            }
        }

        // Use theme
        if(linenum==17){
            var=thisline.remove(0,10);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="0"){
                Buffer::use_custom_theme=false;
            }
            else{
                 Buffer::use_custom_theme=true;
            }
        }

        // tile Background
        if(linenum==18){
            var=thisline.remove(0,16);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="0"){
                Buffer::tile_bg=false;
            }
            else{
                 Buffer::tile_bg=true;
            }
        }

        // Apply background to tree
        if(linenum==19){
            var=thisline.remove(0,16);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="0"){
                Buffer::set_tree_background=false;
            }
            else{
                 Buffer::set_tree_background=true;
            }
        }

        // Font color
        if(linenum==20){
            var=thisline.remove(0,11);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="NULL"){
                Buffer::text_hexcolor.clear();
            }
            else{
                Buffer::text_hexcolor=var.trimmed();
            }
        }

        // Font face
        if(linenum==21){
            var=thisline.remove(0,10);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="NULL"){
                Buffer::font_face.clear();
            }
            else{
                Buffer::font_face=var.trimmed();
            }
        }

        // Font size
        if(linenum==22){
            var=thisline.remove(0,10);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="NULL"){
                Buffer::font_size="12";
            }
            else{
                Buffer::font_size=var.trimmed();
            }
        }

        // Record time of entries
        if(linenum==23){
            var=thisline.remove(0,9);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="0"){
                Buffer::keep_time=false;
            }
            else{
                 Buffer::keep_time=true;
            }
        }

        // Use Local Time
        if(linenum==24){
            var=thisline.remove(0,11);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="0"){
                Buffer::use_system_time=false;
            }
            else{
                 Buffer::use_system_time=true;
            }
        }

        // Show title
        if(linenum==25){
            var=thisline.remove(0,11);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="0"){
                Buffer::show_title=false;
            }
            else{
                 Buffer::show_title=true;
            }
        }

        // Use 24 hour clock
        if(linenum==26){
            var=thisline.remove(0,8);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="0"){
                Buffer::use_24_hour=false;
            }
            else{
                 Buffer::use_24_hour=true;
            }
        }

        // Load Date format
        if(linenum==27){
            var=thisline.remove(0,12);
            //cout << "Sort by day: " << var.toStdString();

            Buffer::date_format=var.trimmed();
        }

        // support rich text
        if(linenum==28){
            var=thisline.remove(0,10);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="0"){
                Buffer::rich_text=false;
            }
            else{
                 Buffer::rich_text=true;
            }
        }

        // Load background image
        if(linenum==29){
            var=thisline.remove(0,17);
            //cout << "Sort by day: " << var.toStdString();

            Buffer::background_image=var.trimmed();
        }

        // Load icon labels
        if(linenum==30){
            var=thisline.remove(0,12);
            //cout << "Sort by day: " << var.toStdString();

            if(var.trimmed()=="0"){
                Buffer::show_icon_labels=false;
            }
            else{
                 Buffer::show_icon_labels=true;
            }


        }

    }

    configfile.close();

    cout << "Done!" << endl;
    cout << "OUTPUT: Stage 1 completed, proceeding to Stage 2..." << endl;


}

// Save changes made in Config class to config file
void ConfigManager::WriteConfig(){

}

// Function to rebuild config file if it is missing
void ::ConfigManager::RebuildConfig(){

    using namespace std;
    directory.setPath(configpath);

    // directory check
    if(!directory.exists()){
        cout << "Rebuilding config file directory: " << configpath.toStdString() << endl;
        directory.mkdir(configpath);


    }
    else{
        cout << "OUTPUT: Config directory already exists, proceeding...";
    }

    // once the directory exists, proceed to create the config file and write some default values to it

     cout  << "OUTPUT: Creating new config file: robojournal.cfg..." << endl;
     cout  << "OUTPUT: Populating robojournal.cfg with default values..." << endl;
     QString sysuser=getenv("USERNAME");
     if (configfile.open(QIODevice::ReadWrite)) {
             QTextStream stream(&configfile);
             stream << "ROBOJOURNAL CONFIGURATION v0.2" << endl;
             stream << "#########################################" << endl;
             stream << "TOOLBAR_LOCATION:1" << endl;
             stream << "ALLOW_ROOT_LOGINS:0" << endl;
             stream << "SHOW_ALL_ENTRIES:1" << endl;
             stream << "ALWAYS_USE_DEFAULTS:1" << endl;
             stream << "DB_TYPE:MySQL" << endl;
             stream << "SHOW_WARNINGS:1" << endl;
             stream << "ENTRY_RANGE:2" << endl;
             stream << "DEFAULT_HOST:localhost" << endl;
             stream << "DEFAULT_DB:"+sysuser+"_journal" << endl;
             stream << "DEFAULT_USER:" + sysuser << endl; // use default system user name
             stream << "DB_PORT:3306" << endl;
             stream << "SORT_BY_DAY:0" << endl;
             stream << "ALTERNATE_COLORS:1"<< endl;
             stream << "USE_BACKGROUND:0"<< endl;
             stream << "USE_THEME:0"<< endl;
             stream << "TILE_BACKGROUND:1"<< endl;
             stream << "BACKGROUND_TREE:0"<< endl;
             stream << "FONT_COLOR:#000000"<< endl;
             stream << "FONT_FACE:"<< endl;
             stream << "FONT_SIZE:12"<< endl;
             stream << "USE_TIME:1"<< endl;
             stream << "LOCAL_TIME:1"<< endl;
             stream << "SHOW_TITLE:1"<< endl;
             stream << "24_HOUR:0"<< endl;
             stream << "DATE_FORMAT:0"<< endl;
             stream << "RICH_TEXT:0"<< endl;
             stream << "BACKGROUND_IMAGE:NULL"<< endl;
             stream << "ICON_LABELS:1"<< endl;

             configfile.close();
         }
     cout << "OUTPUT: Config file successfully replaced!" << endl;
     cout << "OUTPUT: Attempting re-read of config file..." <<endl;
     ReadConfig();
     configfile.close();
}

// This function updates the config file with new settings.
// It should always be invoked from the Config class, since the Config::$var
// static variables are assigned in the Config class.

void ConfigManager::UpdateConfig(){


    // rewrite values in config file.
    if(!configfile.isOpen()){
        if (configfile.open(QIODevice::ReadWrite)) {
            QTextStream stream(&configfile);
            stream << "ROBOJOURNAL CONFIGURATION v0.2" << endl;
            stream << "#########################################" << endl;
            stream << "TOOLBAR_LOCATION:" << Config::new_toolbar_pos.trimmed() << endl;
            stream << "ALLOW_ROOT_LOGINS:" << Config::new_allow_root.trimmed() << endl;
            stream << "SHOW_ALL_ENTRIES:" << Config::new_show_all_entries.trimmed() << endl;
            stream << "ALWAYS_USE_DEFAULTS:" << Config::new_always_use_defaults.trimmed() << endl;
            stream << "DB_TYPE:" << Config::new_dbtype.trimmed() << endl;
            stream << "SHOW_WARNINGS:" << Config::new_show_confirm.trimmed() << endl;
            stream << "ENTRY_RANGE:" << Config::new_entry_range.trimmed() << endl;
            stream << "DEFAULT_HOST:" << Config::new_default_host.trimmed() << endl;
            stream << "DEFAULT_DB:" << Config::new_default_db.trimmed() << endl;
            stream << "DEFAULT_USER:" << Config::new_default_user.trimmed() << endl;
            stream << "DB_PORT:" << Config::new_default_port.trimmed() << endl;
            stream << "SORT_BY_DAY:" << Config::new_sort_by_day.trimmed() << endl;
            stream << "ALTERNATE_COLORS:"<< Config::new_alternate_rows.trimmed() << endl;
            stream << "USE_BACKGROUND:"<< Config::new_use_background.trimmed() << endl;
            stream << "USE_THEME:"<< Config::new_use_custom_theme.trimmed() << endl;
            stream << "TILE_BACKGROUND:"<< Config::new_tile_bg.trimmed() << endl;
            stream << "BACKGROUND_TREE:"<< Config::new_use_background_in_tree.trimmed() << endl;
            stream << "FONT_COLOR:"<< Config::new_text_hexcolor.trimmed() << endl;
            stream << "FONT_FACE:"<< Config::new_font_face.trimmed() << endl;
            stream << "FONT_SIZE:"<< Config::new_font_size.trimmed() << endl;
            stream << "USE_TIME:"<< Config::new_record_time.trimmed() <<  endl;
            stream << "LOCAL_TIME:"<< Config::new_use_local_time.trimmed() << endl;
            stream << "SHOW_TITLE:"<< Config::new_show_title.trimmed() << endl;
            stream << "24_HOUR:"<< Config::new_24_hr.trimmed() << endl;
            stream << "DATE_FORMAT:"<< Config::new_date_mode.trimmed() << endl;
            stream << "RICH_TEXT:"<< Config::new_rich_text.trimmed() << endl;
            stream << "BACKGROUND_IMAGE:" << Config::new_background_image.trimmed() << endl;
            stream << "ICON_LABELS:" << Config::new_icon_labels.trimmed() << endl;

            // Bugfix: always overwrite any junk data at the end of the config file with blank lines
            stream << "" << endl;
            stream << "" << endl;
            stream << "" << endl;
            stream << "" << endl;

            configfile.close();

            // Replace old data in Buffer
            ConfigManager::ReadConfig();
        }
    }
    else{ // show error if robojournal.cfg is open
        QMessageBox h;
        h.critical(NULL,"Error","RoboJournal is not able to modify the configuration because " +
                   config_name + " is in use by another process!");

    } 
}

/*
  CustomSetup() takes data from the Journal Creator class or the Journal Locator
 class and uses it to create the config file. This function is supposed to replace
 RebuildConfig() for most purposes. Now, RebuildConfig() only gets called if you're connecting to
 an existing database at first run.
*/

void ConfigManager::CustomSetup(QString host,QString db_name, QString port, QString newuser){

    using namespace std;
    directory.setPath(configpath);

    // directory check
    if(!directory.exists()){
        cout << "Rebuilding config file directory: " << configpath.toStdString() << endl;
        directory.mkdir(configpath);


    }
    else{
        cout << "OUTPUT: Config directory already exists, proceeding...";
    }

    // once the directory exists, proceed to create the config file and write some default values to it

     cout  << "OUTPUT: Creating new config file: robojournal.cfg..." << endl;
     cout  << "OUTPUT: Populating robojournal.cfg with values from setup form..." << endl;

     if (configfile.open(QIODevice::ReadWrite)) {
             QTextStream stream(&configfile);
             stream << "ROBOJOURNAL CONFIGURATION v0.2" << endl;
             stream << "#########################################" << endl;
             stream << "TOOLBAR_LOCATION:1" << endl;
             stream << "ALLOW_ROOT_LOGINS:0" << endl;
             stream << "SHOW_ALL_ENTRIES:1" << endl;
             stream << "ALWAYS_USE_DEFAULTS:1" << endl;
             stream << "DB_TYPE:MySQL" << endl;
             stream << "SHOW_WARNINGS:1" << endl;
             stream << "ENTRY_RANGE:2" << endl;
             stream << "DEFAULT_HOST:" << host << endl;
             stream << "DEFAULT_DB:"<< db_name << endl;
             stream << "DEFAULT_USER:" + newuser << endl; // use default system user name
             stream << "DB_PORT:" << port << endl;
             stream << "SORT_BY_DAY:0" << endl;
             stream << "ALTERNATE_COLORS:1"<< endl;
             stream << "USE_BACKGROUND:0"<< endl;
             stream << "USE_THEME:0"<< endl;
             stream << "TILE_BACKGROUND:1"<< endl;
             stream << "BACKGROUND_TREE:0"<< endl;
             stream << "FONT_COLOR:#000000"<< endl;
             stream << "FONT_FACE:"<< endl;
             stream << "FONT_SIZE:12"<< endl;
             stream << "USE_TIME:1"<< endl;
             stream << "LOCAL_TIME:1"<< endl;
             stream << "SHOW_TITLE:1"<< endl;
             stream << "24_HOUR:0"<< endl;
             stream << "DATE_FORMAT:0"<< endl;
             stream << "RICH_TEXT:0"<< endl;
             stream << "BACKGROUND_IMAGE:NULL"<< endl;
             stream << "ICON_LABELS:1"<< endl;
             configfile.close();
         }
     cout << "OUTPUT: Config file successfully replaced!" << endl;
     cout << "OUTPUT: Attempting re-read of config file..." <<endl;
     ReadConfig();
     configfile.close();
}



