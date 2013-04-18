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


#include "config.h"
#include "ui_config.h"
#include <iostream>
#include <QButtonGroup>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configmanager.h"
#include "buffer.h"
#include <QDebug>
#include "configmanager.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QFont>
#include <QDate>




Buffer b;
static ConfigManager m;


Config::Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    using namespace std;
    ui->setupUi(this);

    // hide question mark button in title bar when running on Windows
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    int width=this->width();
    int height=this->height();
    this->setMaximumSize(width,height);
    this->setMinimumSize(width,height);

    Populate();



}

// Save settings changes. This function is triggered when user clicks "OK"
void Config::SaveConfig(){
    Config::new_default_host=ui->DefaultHost->text();
    Config::new_default_port=ui->DBPort->text();
    Config::new_dbtype=ui->DBEngine->currentText();
    Config::new_default_user=ui->DefaultUser->text();
    Config::new_default_db=ui->DefaultDB->text();
    Config::new_dbtype=ui->DBEngine->currentText();

    if(ui->AllowRoot->isChecked()){
        Config::new_allow_root="1";
    }
    else{
        Config::new_allow_root="0";
    }

    if(ui->ShowAllEntries->isChecked()){
        Config::new_show_all_entries="1";
    }
    else{
        Config::new_show_all_entries="0";
    }

    if(ui->ShowConfirmation->isChecked()){
        Config::new_show_confirm="1";
    }
    else{
        Config::new_show_confirm="0";
    }


    if(ui->UseDefaults->isChecked()){
        Config::new_always_use_defaults="1";
    }
    else{
        Config::new_always_use_defaults="0";
    }

    if(ui->SortByDay->isChecked()){
        Config::new_sort_by_day="1";
    }
    else{
        Config::new_sort_by_day="0";
    }

    int toolbar_pos=ui->DefaultMenuPos->currentIndex();
    switch(toolbar_pos){
        case 0:
        Config::new_toolbar_pos="0";
        break;

        case 1:
        Config::new_toolbar_pos="1";
        break;

        case 2:
        Config::new_toolbar_pos="2";
        break;
    }

    if(ui->AlternateRows->isChecked()){
        Config::new_alternate_rows="1";
    }
    else{
        Config::new_alternate_rows="0";
    }

    if(ui->BGImage->text().isEmpty()){
        Config::new_use_background="0";
        Config::new_background_image="";
    }
    else{
        Config::new_use_background="1";
        Config::new_background_image=ui->BGImage->text();
    }

    if(ui->ShowTitle->isChecked()){
        Config::new_show_title="1";
    }
    else{
        Config::new_show_title="0";
    }

    if(ui->CustomAppearance->isChecked()){
        Config::new_use_custom_theme="1";
    }
    else{
        Config::new_use_custom_theme="0";
    }

    if(ui->TileBackground->isChecked()){
        Config::new_tile_bg="1";
    }
    else{
        Config::new_tile_bg="0";
    }

    if(ui->IconText->isChecked()){
        Config::new_icon_labels="1";
    }
    else{
        Config::new_icon_labels="0";
    }

    if(ui->Entrylist_Background->isChecked()){
        Config::new_use_background_in_tree="1";
    }
    else{
        Config::new_use_background_in_tree="0";
    }

    if(ui->ColorValue->text().isEmpty()){
        Config::new_text_hexcolor="NULL";
    }
    else{
        Config::new_text_hexcolor=ui->ColorValue->text();
    }

    // only save font face && font size if custom appearance is enabled
    if(ui->CustomAppearance->isChecked()){

        Config::new_font_face=ui->Font->currentText();
        Config::new_font_size=ui->FontSize->currentText();
    }
    else{
        Config::new_font_face="NULL";
        Config::new_font_size="NULL";
    }

    if(ui->RecordTime->isChecked()){
        Config::new_record_time="1";
    }
    else{
        Config::new_record_time="0";
    }

    if(ui->UseLocalTime->isChecked()){
        Config::new_use_local_time="1";
    }
    else{
        Config::new_use_local_time="0";
    }

    if(ui->Use24Clock->isChecked()){
        Config::new_24_hr="1";
    }
    else{
        Config::new_24_hr="0";
    }

    if(ui->ShowTitle->isChecked()){
        Config::new_show_title="1";
    }
    else{
        Config::new_show_title="0";
    }

    if(ui->RichText->isChecked()){
        Config::new_rich_text="1";
    }
    else{
        Config::new_rich_text="0";
    }

    int datemode=ui->DateFormat->currentIndex();

    switch(datemode){
    case 0:
        Config::new_date_mode="0";
        break;

    case 1:
        Config::new_date_mode="1";
        break;

    case 2:
        Config::new_date_mode="2";
        break;
    }

    int year_num=ui->YearRange->currentIndex();
    year_num++; // offset index difference with actual count
    Config::new_entry_range=QString::number(year_num);
    m.UpdateConfig();
    MadeChanges=true;
}

// Background selection function
QString Config::BackgroundSelect(){
    QFileDialog f;
    QString file=f.getOpenFileName(this,"Choose Background Image","","Image Files (*.png *.jpg *.bmp *.gif)");
    return file;
}

//Function that allows the user to select a color and returns the color as RGB hex value
QString Config::ColorSelect(){
    QColorDialog d;
    d.exec();
    QColor  choice=d.selectedColor();
    QString color=choice.name();
    return color;
}


// Private function that populates the form with data from Buffer class
void Config::Populate(){
    using namespace std;

    // Make file and RGB fields non-editable
    ui->ColorValue->setReadOnly(true);
    ui->BGImage->setReadOnly(true);

    // clear date/time example
    ui->DateExample->clear();

    // set local time option invisible for now
    ui->UseLocalTime->setVisible(false);
    //ui->RichText->setVisible(false);


    // temporarily disable the backend settings
    ui->DBEngine->setDisabled(true);

    // disable rich text until it is ready
    ui->RichText->setEnabled(false);



    cout << "Output: Populating configuration form with data from buffer...";

    if(Buffer::databaseport=="5432"){
        ui->DBPort->setToolTip("Using default port");
    }

    if(Buffer::backend=="MySQL"){
        ui->DBEngine->setCurrentIndex(0);

        if(!ui->DBPort->isVisible()){
            ui->DBPort->setVisible(true);
            ui->label_7->setText("Port");
        }
    }

    if(Buffer::backend=="Postgres"){
        ui->DBEngine->setCurrentIndex(2);

        if(!ui->DBPort->isVisible()){
            ui->DBPort->setVisible(true);
            ui->label_7->setText("Port");
        }
    }

    if(Buffer::backend=="SQLite"){
        ui->DBEngine->setCurrentIndex(1);
        ui->DBPort->setVisible(false);
        ui->label_7->setText("Failsafe option");

    }



    // Populate form with data from Buffer class
    ui->DefaultHost->setText(Buffer::defaulthost);
    ui->DefaultUser->setText(Buffer::defaultuser);
    ui->DefaultDB->setText(Buffer::defaultdatabase);
    ui->DBPort->setText(Buffer::databaseport);

    int year_range=Buffer::entryrange.toInt();
    year_range--;
    ui->YearRange->setCurrentIndex(year_range);

    int toolbar_pos=Buffer::toolbar_pos;
    ui->DefaultMenuPos->setCurrentIndex(toolbar_pos);

    if(Buffer::alwaysusedefaults){
        ui->UseDefaults->setChecked(true);
    }

    if(Buffer::showwarnings){
        ui->ShowConfirmation->setChecked(true);
    }

    if(Buffer::allowroot){
        ui->AllowRoot->setChecked(true);
    }

    if(Buffer::allentries){
        ui->ShowAllEntries->setChecked(true);
        ui->YearRange->setDisabled(true);
    }
    else{ // show entry range instead
        ui->ShowEntryRange->setChecked(true);
        ui->YearRange->setEnabled(true);
    }

    if(Buffer::sortbyday){
        ui->SortByDay->setChecked(true);
    }
    else{
        ui->SortByDay->setChecked(false);
    }

    if(Buffer::alternate_rows){
        ui->AlternateRows->setChecked(true);
    }

    if(Buffer::use_24_hour){
        ui->Use24Clock->setChecked(true);
    }

    if(Buffer::use_system_time){
        ui->UseLocalTime->setChecked(true);
    }

    if(Buffer::show_title){
        ui->ShowTitle->setChecked(true);
    }

    if(Buffer::tile_bg){
        ui->TileBackground->setChecked(true);
    }

    if(Buffer::show_icon_labels){
        ui->IconText->setChecked(true);
    }

    if(Buffer::set_tree_background){
        ui->Entrylist_Background->setChecked(true);
    }

    if(Buffer::use_custom_theme){
        ui->BGImage->setText(Buffer::background_image);
        ui->ColorValue->setText(Buffer::text_hexcolor);
        ui->CustomAppearance->setChecked(true);

    }
    else{
        ui->NativeAppearance->setChecked(true);
        ui->CustomizeSettings->setEnabled(false);
    }

    if(Buffer::keep_time){
        ui->RecordTime->setChecked(true);
    }
    else{
        ui->Use24Clock->setEnabled(false);
        ui->UseLocalTime->setEnabled(false);

    }

    int index=Buffer::date_format.toInt();
    ui->DateFormat->setCurrentIndex(index);

    // disable background options if background is not being used
    if(!Buffer::use_background){
        ui->TileBackground->setEnabled(false);
        ui->Entrylist_Background->setEnabled(false);
    }

    // choose correct font
    QFont currentfont(Buffer::font_face);
    ui->Font->setCurrentFont(currentfont);

    //and font size



    for(int i=0; i<ui->FontSize->count(); i++){

        if(ui->FontSize->itemText(i)==Buffer::font_size){
            //cout << "Item: " << i  << endl;
            ui->FontSize->setCurrentIndex(i);
            break;
        }
    }


    QButtonGroup year;
    year.addButton(ui->ShowEntryRange);
    year.addButton(ui->ShowAllEntries);
    cout << "Done!" << endl;
}

Config::~Config()
{
    delete ui;
}

void Config::on_DefaultMenuPos_currentIndexChanged(int index)
{
    index=-1;

}

void Config::on_ShowAllEntries_clicked()
{
    ui->YearRange->setDisabled(true);
}

void Config::on_ShowEntryRange_clicked()
{
    ui->YearRange->setEnabled(true);
}


// Update config
void Config::on_buttonBox_accepted()
{
    SaveConfig();
}

void Config::on_buttonBox_rejected()
{
    MadeChanges=false;

    if(Buffer::firstrun){
        exit(0);
    }
}

void Config::on_DBEngine_currentIndexChanged(int index)
{
    switch(index){
        case 0:
        ui->DBPort->setText("3306");
        if(!ui->DBPort->isVisible()){
            ui->DBPort->setVisible(true);
            ui->label_7->setText("Port");
        }
        break;

        case 1:
            ui->DBPort->setVisible(false);
            ui->label_7->setText("Failsafe option");

        break;

        case 2:
        ui->DBPort->setText("5432");
        if(!ui->DBPort->isVisible()){
            ui->DBPort->setVisible(true);
            ui->label_7->setText("Port");
        }
        break;


    }
}

/* Special function that only gets called from the FirstRun class.
  This lets the user connect RJ to an existing database. When this is called, there isn't supposed to be
  a config file, so it becomes necessary to create one first before this function is called and then load its data
  into the Config class. Because we're setting up the DB stuff, that tab should be selected by default.

  */

void Config::ManualConfig(){
    ui->TabBox->setCurrentIndex(1);
    ui->DefaultDB->clear();
    ui->DefaultHost->clear();
    ui->DefaultUser->clear();
    ui->DBPort->clear();
}

void Config::on_RecordTime_clicked(bool checked)
{
    if(checked){
        ui->Use24Clock->setEnabled(true);
        ui->UseLocalTime->setEnabled(true);
    }
    else{
        ui->Use24Clock->setChecked(false);
        ui->UseLocalTime->setChecked(false);
        ui->Use24Clock->setEnabled(false);
        ui->UseLocalTime->setEnabled(false);
    }
}


void Config::on_DateFormat_currentIndexChanged(int index)
{
    // get today's date for example
    QDate today;
    today=today.currentDate();

    int d=today.day();
    int y=today.year();
    int m=today.month();

    QString month=today.longMonthName(m);
    QString year=QString::number(y);
    QString day=QString::number(d);

    if(index==0){
        ui->DateExample->setText("Example: " + day + " " + month + ", " + year + " (DD-MM-YYYY)");
    }

    if(index==1){
        ui->DateExample->setText("Example: " + month + " " + day + ", " + year + " (MM-DD-YYYY)");
    }

    if(index==2){
        ui->DateExample->setText("Example: " + year + ", " + month + " " + day + " (YYYY-MM-DD)");
    }
}



void Config::on_ColorButton_clicked()
{
    QString color=ColorSelect();
    color=color.toUpper();
    ui->ColorValue->setText(color);

}

void Config::on_BrowseButton_clicked()
{
    QString file=BackgroundSelect();

    if(!file.isEmpty()){
        ui->BGImage->setText(file);
        // re-enable background options
        ui->TileBackground->setEnabled(true);
        ui->Entrylist_Background->setEnabled(true);
    }

}

void Config::on_NativeAppearance_clicked()
{
    ui->CustomizeSettings->setEnabled(false);
}

void Config::on_CustomAppearance_clicked()
{
    ui->CustomizeSettings->setEnabled(true);

    if(Buffer::tile_bg){
        ui->TileBackground->setChecked(true);
    }

    if(Buffer::set_tree_background){
        ui->Entrylist_Background->setChecked(true);
    }

    ui->BGImage->setText(Buffer::background_image);
    ui->ColorValue->setText(Buffer::text_hexcolor);
    ui->CustomAppearance->setChecked(true);

    // set default text color to black
    ui->ColorValue->setText("#000000");

}

void Config::on_ClearButton_clicked()
{
    ui->BGImage->clear();
    ui->TileBackground->setEnabled(false);
    ui->Entrylist_Background->setEnabled(false);
    ui->TileBackground->setChecked(false);
    ui->Entrylist_Background->setChecked(false);
}
