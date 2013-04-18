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


#include "buffer.h"
#include <QtGui/QApplication>

// Static variable declaration
QString Buffer::defaulthost;
QString Buffer::defaultuser;
QString Buffer::defaultdatabase;
QString Buffer::databaseport;
QString Buffer::entryrange;
QString Buffer::backend;

int Buffer::toolbar_pos;
bool Buffer::alwaysusedefaults;
bool Buffer::allowroot;
bool Buffer::allentries;
bool Buffer::showwarnings;
bool Buffer::sortbyday;
bool Buffer::firstrun;
bool Buffer::alternate_rows;
bool Buffer::rich_text;
bool Buffer::use_custom_theme;
bool Buffer::use_24_hour;
bool Buffer::use_background;
bool Buffer::use_system_time;
bool Buffer::tile_bg;
bool Buffer::set_tree_background;
bool Buffer::show_icon_labels;

bool Buffer::login_succeeded;


QString Buffer::username;
QString Buffer::password;
QString Buffer::host;
QString Buffer::database_name;
QString Buffer::background_image;
QString Buffer::font_size;
QString Buffer::text_hexcolor;
QString Buffer::font_face;
bool Buffer::keep_time;
bool Buffer::show_title;
QString Buffer::date_format;

bool Buffer::needs_upgrade; // if true, journal needs to be upgraded to 0.2

bool Buffer::editmode; //if true, start the Editor in revision mode. If False, create a blank entry
QString Buffer::editentry; // the entry id that needs to be edited

// Current RoboJournal Version used globally in Program.
QString Buffer::version="0.2.1";


Buffer::Buffer()
{


}

void Buffer::OutputBuffer(){

}
