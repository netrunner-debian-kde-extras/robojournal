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


#include "editor.h"
#include "ui_editor.h"
#include <iostream>
#include <buffer.h>
#include <stdlib.h>
#include <QDate>
#include "mysqlcore.h"
#include <QDateEdit>
#include <QMessageBox>
#include <QMenu>
#include <QFont>
#include <QStatusBar>

QString Editor::body;
QString Editor::title;
int Editor::day;
int Editor::month;
int Editor::year;


void Editor::reject(){
    ConfirmExit();
}

// Ask the user to confirm the desire to exit the editor window. Triggered if ESC is pressed, Abort is clicked,
// or window is closed.
void Editor::ConfirmExit(){

    if(Buffer::showwarnings){
        QMessageBox m(this);
        m.setWindowTitle("RoboJournal");
        m.setIcon(QMessageBox::Question);
        m.setText("This entry will not be saved if you close the editor now. Are you sure you want to do this?");
        m.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        m.setDefaultButton(QMessageBox::No);
        int choice=m.exec();

        switch(choice){
        case QMessageBox::Yes:
            this->done(0);
            break;

        case QMessageBox::No:
            // do nothing
            break;
        }
    }
    else{
        this->done(0);
    }

}

Editor::Editor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editor)
{

    ui->setupUi(this);

    // disable format options for now
    ui->FormatButton->setDisabled(true);

    // hide question mark button in title bar when running on Windows
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    if(Buffer::editmode){
        InitiateEditMode();
        LoadEntry(Buffer::editentry);
    }
    else{
        // set todays date if we're writing a new entry
        SetDate();
    }


    SetupMenu();

    int width=this->width();
    int height=this->height();
    this->setMaximumSize(width,height);
    this->setMinimumSize(width,height);

    // Only zero everything out if we are creating a new entry. Otherwise, DocumentStats()
    // gets called when the editor starts up in edit mode and the proper values are pre-loaded.

    if(!Buffer::editmode){
        ui->WordCount->setText(" Word count: 0 ");
        ui->CharacterCount->setText(" Character count: 0 ");
        ui->RemainingCharacters->setText(" Remaining characters: 65535 ");
        ui->DocumentMode->setText(" Write Mode ");
    }

    // create status bar and populate it
    QStatusBar *bar=new QStatusBar(this);
    ui->GUI->addWidget(bar);

    bar->addWidget(ui->WordCount, 0);
    bar->addWidget(ui->CharacterCount,0);
    bar->addWidget(ui->RemainingCharacters,0);

    // empty slot to keep everything spaced properly
    QLabel *spacer=new QLabel(" ");
    bar->addWidget(spacer,1);

    bar->addWidget(ui->DocumentMode,0);
    bar->setSizeGripEnabled(false);

}

// Setup the form in EditMode
void Editor::InitiateEditMode(){

    ui->PostEntry->setText("Update Entry");

    ui->DocumentMode->setText(" Edit Mode ");
    ui->DocumentMode->setAlignment(Qt::AlignRight);

    // let the user change the date in edit mode -- NOT YET... that stil lcauses problems
    //ui->EntryDate->setReadOnly(false);
}

// Load an entry and populate the form with it if EditMode is true
void Editor::LoadEntry(QString entry){

    if(Buffer::backend=="MySQL"){

        MySQLCore m;
        QList<QString> list=m.RetrieveEntryFull(entry);

        ui->EntryTitle->setText(list.at(0));
        ui->EntryPost->setPlainText(list.at(5));

        QDate postdate;
        postdate.setDate(list.at(3).toInt(),list.at(1).toInt(),list.at(2).toInt());

        ui->EntryDate->setDate(postdate);


        // Do document statistics
        DocumentStats();
    }

}

//Update entry in the database
bool Editor::UpdateEntry(){

    //Bugfix for 0.2
    // clean commas out of the title so it doesn't mess up the array in MainWindow::CreateTree()
    QString title=ui->EntryTitle->text();
    title=title.replace(QString(","),QString(""));

    QString body=ui->EntryPost->toPlainText();
    QDate post_date=ui->EntryDate->date();
    QString tags="null";


    int day=post_date.day();
    int month=post_date.month();
    int year=post_date.year();

    MySQLCore m;
    bool success=m.Update(title,month,day,year,tags,body,Buffer::editentry);
    return success;

}

// Prepare a new Entry and post it to database
bool Editor::NewEntry(){
    using namespace std;
    Editor::body=ui->EntryPost->toPlainText();

    //Bugfix for 0.2
    // clean commas out of the title so it doesn't mess up the array in MainWindow::CreateTree()
    QString title=ui->EntryTitle->text();
    title=title.replace(QString(","),QString(""));


    Editor::title=title;
    QDate post_date=ui->EntryDate->date();


    Editor::day=post_date.day();
    Editor::month=post_date.month();
    Editor::year=post_date.year();


    bool success=false;
    cout << "OUTPUT: Adding new entry \"" + Editor::title.toStdString() + "\" to database " << Buffer::database_name.toStdString() << "...";

    if(Buffer::backend=="MySQL"){
        MySQLCore my;
        success=my.AddEntry();
    }

    if(success){
        cout << "SUCCESS!" << endl;


    }
    else{
        cout << "FAILED" << endl;
    }

return success;
}

Editor::~Editor()
{
    delete ui;
}

//get current date
void Editor::SetDate(){
    ui->EntryDate->setDate(QDate::currentDate());

}

// Automatically update window title with post title
void Editor::setTitle(QString title){
    this->setWindowTitle(title + " -- RoboJournal");

    if(title==NULL){
        this->setWindowTitle("Untitled Entry -- RoboJournal");
    }
}


// Set up Format button menu
void Editor::SetupMenu(){

    QFont b("sans", 10, QFont::Bold);
    b.setBold(true);

    QFont i("sans", 10, QFont::StyleItalic);
    i.setItalic(true);

    QFont u("sans", 10, QFont::Normal);
    u.setUnderline(true);

    QMenu *formatmenu=new QMenu();
    QAction *bold = new QAction("Bold", this);
    QAction *italic = new QAction("Italic", this);
    QAction *underline = new QAction("Underline", this);
    QAction *h2 = new QAction("Subheader", this);

    bold->setFont(b);
    italic->setFont(i);
    underline->setFont(u);


    bold->setToolTip("Bold selected text");
    italic->setToolTip("Italicize selected text");
    underline->setToolTip("Underline selected text");
    h2->setToolTip("Format selected text as sub-heading");


    formatmenu->addAction(bold);
    formatmenu->addAction(italic);
    formatmenu->addAction(underline);
    formatmenu->addSeparator();
    formatmenu->addAction(h2);

    ui->FormatButton->setMenu(formatmenu);
}



void Editor::on_EntryTitle_textChanged(const QString &title)
{
 // pass title string to Editor::SetTitle() function... don't confuse this with QDialog::SetWindowTitle!!!
 setTitle(title);
}

// keep track of document statistics
void Editor::DocumentStats(){
    using namespace std;
    QString data=ui->EntryPost->toPlainText();
    int maxlength=65535;
    int num=data.size();
    int remaining=maxlength-num;

    QString str_num;
    QString str_rem;

    str_num.append(QString("%1").arg(num));
    str_rem.append(QString("%1").arg(remaining));

    QString wordcount=WordCount(data);


    ui->WordCount->setText(" Word count: " + wordcount + " ");
    ui->CharacterCount->setText(" Character count: " + str_num + " ");
    ui->RemainingCharacters->setText(" Remaining characters: " + str_rem + " ");

    // prevent mysql data overflows, though it must have been one hell of a day if you can't say
    // what's on your mind in less than 65k chars

    if(remaining<0){

         ui->RemainingCharacters->setText("<font color=\"red\"> Remaining characters: " +
                 str_rem +"</font> ");


        ui->PostEntry->setDisabled(true);
    }
    else{
        ui->PostEntry->setEnabled(true);
    }

}

// Get word count from body. This needs to return a QString instead
// of Int because it needs to be displayed in a QLabel. Sure, we could
// return an int but that would require an extra conversion step.

QString Editor::WordCount(QString data){

    // get rid of extra whitespace that would throw off the correct count
    data=data.trimmed();
    data=data.simplified();


    // split the string into an array of words and count the size of array. That's our word count.
    QStringList words=data.split(" ");
    int count=words.count();

    QString wordcount="0";

    // solve a bug that always returns wordcount as 1 even if the body is blank
    if(data.size()==0){
        wordcount="0";
    }
    else{
    wordcount=QString::number(count);
    }


    return wordcount;
}



void Editor::on_EntryPost_textChanged()
{
    DocumentStats();
}

void Editor::on_UndoButton_clicked()
{
    ui->EntryPost->undo();

}

void Editor::on_RedoButton_clicked()
{
    ui->EntryPost->redo();
}



void Editor::on_Cancel_clicked()
{
    if(Buffer::showwarnings){
        ConfirmExit();
    }
    else{
        this->done(0);
    }

}

void Editor::on_CutButton_clicked()
{
    ui->EntryPost->cut();
}

void Editor::on_CopyButton_clicked()
{
    ui->EntryPost->copy();
}


void Editor::on_PasteButton_clicked()
{
    ui->EntryPost->paste();
}



void Editor::on_PostEntry_clicked()
{
    QMessageBox m;
    if(Buffer::editmode){ // edit mode

        if((ui->EntryPost->toPlainText()==NULL) || (ui->EntryTitle->text()==NULL)){
            m.critical(this,"Error","The title and body of this entry must be filled in before RoboJournal can update it.");
        }
        else{
            bool success=UpdateEntry();

            if(success){
                this->hide();
                m.information(this,"RoboJoural", "<em>" + ui->EntryTitle->text() + "</em> was successfully updated.");
                close();
            }
        }
    }
    else{ // new entry mode

        if((ui->EntryPost->toPlainText()==NULL) || (ui->EntryTitle->text()==NULL)){
            m.critical(this,"Error","The title and body of this entry must be filled in before RoboJournal can post it to the database.");
        }
        else{
            bool success=NewEntry();

            if(success){
                this->hide();
                m.information(this,"RoboJoural", "<em>" + ui->EntryTitle->text() + "</em> was successfully added to your journal.");
                close();
            }
        }
    }

}
