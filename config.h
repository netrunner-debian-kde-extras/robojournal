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


#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>

namespace Ui {
    class Config;
}

class Config : public QDialog
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0);
    ~Config();

    void ManualConfig();
    void SaveConfig();

    QString ColorSelect();
    QString BackgroundSelect();



    bool MadeChanges;

    // new default settings that can be called from any class through Config::$var.
    // These should be null unless initiated elsewhere.
    // even bools and ints should be treated as QStrings b/c they are written to a config file for later use.

    static QString new_default_host;
    static QString new_default_user;
    static QString new_default_db;
    static QString new_default_port;
    static QString new_always_use_defaults;
    static QString new_show_confirm;
    static QString new_dbtype;
    static QString new_allow_root;
    static QString new_entry_range;
    static QString new_show_all_entries;
    static QString new_toolbar_pos;
    static QString new_sort_by_day;

    // new for version 0.2
    static QString new_alternate_rows;
    static QString new_rich_text;
    static QString new_use_background;
    static QString new_use_custom_theme;
    static QString new_text_hexcolor;
    static QString new_background_image;
    static QString new_show_title;
    static QString new_font_face;
    static QString new_font_size;
    static QString new_date_mode;
    static QString new_tile_bg;
    static QString new_use_background_in_tree;
    static QString new_record_time;
    static QString new_use_local_time;
    static QString new_24_hr;
    static QString new_icon_labels;









private slots:
    void on_DefaultMenuPos_currentIndexChanged(int index);

    void on_ShowAllEntries_clicked();

    void on_ShowEntryRange_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_DBEngine_currentIndexChanged(int index);

    void on_RecordTime_clicked(bool checked);

    void on_DateFormat_currentIndexChanged(int index);



    void on_ColorButton_clicked();

    void on_BrowseButton_clicked();

    void on_NativeAppearance_clicked();

    void on_CustomAppearance_clicked();

    void on_ClearButton_clicked();

private:
    Ui::Config *ui;
    void Populate();
};

#endif // CONFIG_H
