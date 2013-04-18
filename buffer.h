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


#ifndef BUFFER_H
#define BUFFER_H
#include <QtGui/QApplication>

class Buffer
{
public:
    Buffer();
    void OutputBuffer();

    static QString host;
    static QString database_name;
    static QString username;
    static QString password;

    static bool login_succeeded; // did the login attempt work?


    static QString defaulthost; // default host
    static QString defaultuser; // default user
    static QString defaultdatabase; // default db
    static QString backend; // database backend var
    static QString databaseport; // database port to use for connections
    static QString entryrange; // number of years to show, only used if allentries==false

    static int toolbar_pos; // Toolbar position... 0=left; 1=top; 2=right

    static bool alwaysusedefaults; // always use default values if true
    static bool allowroot; // allow root logins if true
    static bool allentries; // show all entries if true
    static bool showwarnings; // show warning dialogs if true
    static bool sortbyday; // sort list entires by day if true, by month if false
    static bool SSL; // use SSL encryption
    static bool use_indicator; // show entry indicator
    static bool needs_upgrade;

    static bool firstrun; // decide if the program is firstrun or not

    static bool alternate_rows; //whether to show alternate row colors
    static bool rich_text; // whether to support rich text
    static bool use_background; // whether to support a background image

    static bool keep_time; // keep  entry time
    static bool use_system_time; // use system time if true, use remote server time if false
    static bool use_24_hour; // use 24 hour clock
    static bool autoload; // autoload most recent journal entry
    static bool show_icon_labels; // should toolbar icons have text labels? checked == yes, unchecked == no

    // date format: 0=International, 1=USA, 2=Japan
    static QString date_format;

    // theme options; if true, use custom colors, if false use system theme
    static bool use_custom_theme;

    static bool tile_bg; // tile background if true
    static bool set_tree_background; // use background_image on entry tree if true;

    static QString text_hexcolor; // font color in hex format (#FFFFFF)
    static QString background_image; // path to background image on disk
    static bool show_title; // whether to show title for entries
    static QString font_face; // name of font to use in entries
    static QString font_size; //point size of font to use


    static bool editmode; // Start Editor in Edit Mode if true
    static QString editentry; // store entry id to be edited


    static QString version; // keep track of RoboJournal Version;


};

#endif // BUFFER_H
