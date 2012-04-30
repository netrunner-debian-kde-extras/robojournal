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


#include "dblogin.h"
#include "ui_dblogin.h"
#include "mainwindow.h"
#include "buffer.h"
#include <iostream>
#include "configmanager.h"


DBLogin::DBLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBLogin)
{
    ui->setupUi(this);

    // hide question mark button in title bar when running on Windows
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    int width=this->width();
    int height=this->height();
    this->setMaximumSize(width,height);
    this->setMinimumSize(width,height);




}

DBLogin::~DBLogin()
{
    delete ui;
}


// function that checks if the config has changed
// and refreshes the form if necessary

void DBLogin::ResetPassword(){
    ui->Password->setText(NULL);
}

void DBLogin::Refresh(){
    // Check to see if defaults are being used
    if(Buffer::alwaysusedefaults){
        ui->UseDBDefault->setChecked(true);
        ui->DBHost->setText(Buffer::defaulthost);
        ui->WhichDB->setText(Buffer::defaultdatabase);
        ui->DBHost->setDisabled(true);
        ui->WhichDB->setDisabled(true);
        ui->UseUserDefault->setChecked(true);
        ui->Username->setText(Buffer::defaultuser);
        ui->Username->setDisabled(true);

    }
    else{
        ui->UseDBDefault->setChecked(false);
        ui->DBHost->setEnabled(true);
        ui->WhichDB->setEnabled(true);
        ui->DBHost->setText(NULL);
        ui->WhichDB->setText(NULL);
        ui->UseUserDefault->setChecked(false);
        ui->Username->setEnabled(true);
        ui->Username->setText(NULL);
    }

    if(ui->UseDBDefault->isChecked() && ui->UseUserDefault->isChecked()){
        ui->Password->setFocus(Qt::PopupFocusReason);
    }

}

void DBLogin::on_Username_textChanged(const QString &Username)

{
    if(Username=="root"){

        if(!Buffer::allowroot){
            QPixmap noicon("qrc:///icons/delete.png");
            ui->WarningLabel->setPixmap(noicon);
            ui->WarningLabel->setText("<font color=\"red\"><b>Root logins are not allowed!</b></font>");
            ui->buttonBox->setDisabled(true);
        }
        else{
            QPixmap noicon("qrc:///icons/delete.png");
            ui->WarningLabel->setPixmap(noicon);
            ui->WarningLabel->setText("<font color=\"red\"><b>Warning: Root logins can be dangerous!</b></font>");
        }

    }
    else{
        ui->WarningLabel->setText(NULL);
        ui->buttonBox->setEnabled(true);
    }
}


void DBLogin::on_UseUserDefault_clicked()
{

    if(ui->UseUserDefault->isChecked()){
        ui->Username->setDisabled(true);
        ui->Username->setText(Buffer::defaultuser);

    }
    else{
        ui->Username->setEnabled(true);
        ui->Username->setText(NULL);
    }
}

void DBLogin::on_UseDBDefault_clicked()
{
    if(ui->UseDBDefault->isChecked()){
        ui->DBHost->setDisabled(true);
        ui->WhichDB->setDisabled(true);
        ui->DBHost->setText(Buffer::defaulthost);
        ui->WhichDB->setText(Buffer::defaultdatabase);

    }
    else{
        ui->DBHost->setEnabled(true);
        ui->WhichDB->setEnabled(true);
        ui->DBHost->setText(NULL);
        ui->WhichDB->setText(NULL);
    }
}

void DBLogin::on_buttonBox_accepted()
{

    using namespace std;


    // get data from form and pass it to Buffer class
    Buffer::login_succeeded=true;
    Buffer::database_name=ui->WhichDB->text();
    Buffer::host=ui->DBHost->text();
    Buffer::username=ui->Username->text();
    Buffer::password=ui->Password->text();

}

void DBLogin::on_buttonBox_rejected()
{

    //login_succeeded=0;
    Buffer::login_succeeded=false;
}
