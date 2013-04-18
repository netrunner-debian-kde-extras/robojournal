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


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_editor.h"
#include "editor.h"
#include "entrysearch.h"
#include "ui_entrysearch.h"
#include <iostream>
#include "buffer.h"
#include "dblogin.h"
#include "ui_dblogin.h"
#include "config.h"
#include "ui_config.h"
#include <QtSql/QSqlDatabase>
#include "configmanager.h"
//#include <QDebug>
#include "psqlcore.h"
#include <QMessageBox>
#include "mysqlcore.h"
#include "sqlitecore.h"
#include "aboutrj.h"
#include "ui_aboutrj.h"
#include <QTreeWidgetItem>
#include <QListIterator>
#include <QTreeWidgetItem>
#include <QList>
#include <QSqlError>
#include "firstrun.h"
#include "ui_firstrun.h"
#include <QPalette>
#include <QInputDialog>
#include <QCloseEvent>
#include <QPrinter>
#include <QPrintDialog>
#include <QAbstractButton>

//################################################################################################
// Confirm the user's intention to quit the program. This function replaces Quit();
void MainWindow::closeEvent(QCloseEvent *event){
    using namespace std;
    event->ignore();

    // Make RoboJournal show confirmation dialogs if config says so
    if(Buffer::showwarnings){

        // dialog that gets shown if user is connected to a journal
        if(ui->DisconnectButton->isEnabled()){
            QMessageBox q(this);
            q.setText("Currently connected to <b>" + Buffer::database_name + "@" + Buffer::host + "</b>!");
            q.setWindowTitle("Quit Robojournal");
            q.setInformativeText("Do you really want to quit?");
            q.setStandardButtons(QMessageBox::Cancel);

            QPushButton *quit=q.addButton("Quit",QMessageBox::AcceptRole);

            q.setIcon(QMessageBox::Question);
            q.setDefaultButton(QMessageBox::Cancel);

            q.exec();

            if(q.clickedButton() == quit){
                cout << "OUTPUT: User quit the program" << endl;
                event->accept();
            }
            else{
                //do nothing
                event->ignore();
            }
        }

        // dialog that gets shown if user is NOT connected to a journal
        // update: Don't show dialog if there is no journal active b/c it can get annoying after awhile
        else{
            cout << "OUTPUT: User quit the program" << endl;
            event->accept();
        }
    }

    // just quit silently if user has turned off confirm dialogs
    else{
        cout << "OUTPUT: User quit the program" << endl;
        event->accept();
    }
}

//################################################################################################
// Private function to auto-update Currentid whenever new function is selected
void MainWindow::UpdateSelectedEntry(QString id){
    using namespace std;

    // CurrentID is called by several different functions so it should ALWAYS be up
    // to date so you don't accidentally change the wrong entry

    CurrentID=id;

    //cout << "Current index: " << CurrentID.toStdString() << endl;
}

//################################################################################################

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    using namespace std;
    ui->setupUi(this);



    // Set current version in TitleBar
    this->setWindowTitle("RoboJournal " + Buffer::version);

    // Do initial UI configuration (set up toolbar, statusbar, etc.) before showing the main window
    PrimaryConfig();
}

//################################################################################################
// Create Stylesheets for Decorate_GUI() to allow the user to apply a custom
// background and font to RoboJournal GUI. This feature was requested by Sandra Goodhew
// because she thought people would want to use their own backgrounds and fonts.

QString MainWindow::CreateStyleSheet(bool for_entrylist, bool is_TextEdit){
    using namespace std;
    QList<QString> stylelist;

    // if the user wants to use a background image, apply it
    if(Buffer::use_background){

        // apply style to entire QTextEdit class type
        if(is_TextEdit){
            stylelist.append("QTextEdit { ");
        }
//        else{
//            stylelist.append("QTreeWidget { ");
//        }

        // create style for QTextEdit

        // get system colors for border/background.  That way, it will look
        //good no matter what scheme user has

        QPalette pal;
        QBrush bg=pal.window();
        QBrush border=pal.dark();

        QColor swatch=bg.color();
        QColor c_border=border.color();


        stylelist.append("background: " + swatch.name() + " url(" + Buffer::background_image + "); ");
        stylelist.append("border: solid 2px " + c_border.name() + ";  ");


        // don't tile background if  config says not to
        if(!Buffer::tile_bg){
            stylelist.append("background-repeat: no-repeat; ");
        }
    }

    stylelist.append("color: " + Buffer::text_hexcolor + "; ");

    // prevent font size/style from being applied to entrylist if user doesn't want it to be
    if(!for_entrylist){
        stylelist.append("font-family: " + Buffer::font_face + "; ");
        stylelist.append("font-size:" + Buffer::font_size + "pt; ");
    }

    //close the QTextEdit class
    if(is_TextEdit){
        stylelist.append(" }");
    }

    QString stylesheet;
    QListIterator<QString> styleiterator(stylelist);

    while(styleiterator.hasNext()){
        stylesheet.append(styleiterator.next());
    }

    //cout << "Style: " << stylesheet.toStdString() << endl;
    return stylesheet;
}

//################################################################################################
// Call updater
void MainWindow::UpgradeJournal(){
    using namespace std;


    QInputDialog d(this);
    d.setWindowTitle("Authenticate");
    d.setLabelText("Provide root password for " + Buffer::host);
    d.setTextEchoMode(QLineEdit::Password);
    bool accept=d.exec();

    if(accept){
        QString root_pass=d.textValue();
        MySQLCore my;
        my.UpgradeJournal(root_pass);

        QMessageBox m;

        if(my.db.isOpenError()){
            m.critical(this,"RoboJournal","Could not connect! Please make sure the root password is correct and try again.");
            UpgradeJournal();
        }
        else{
            m.information(this,"RoboJournal", "<b>" + Buffer::database_name +
                          "</b> has been upgraded successfully. Click <b>Connect</b> (or hit F2) to log in.");
            cout << "DONE!" << endl;
        }
    }
    else{
        cout << "FAILED!" << endl;
        d.close();
    }
}

//################################################################################################
// Print current entry
void MainWindow::Print(){
    QPrinter p;
    QPrintDialog *d=new QPrintDialog(&p, this);
    d->setWindowTitle("Print Current Entry");

    int choice=d->exec();

    switch(choice){

    case QPrintDialog::Accepted:
        ui->Output->print(&p);
        break;

    case QPrintDialog::Rejected:
        // do nothing
        break;
    }
}

//################################################################################################
// Save current entry as a text file
bool MainWindow::SaveEntry(){
    bool saved=true;
    return saved;
}


//################################################################################################
// Function that returns the inverse Hex color of the provided text color.
// Since the text color is used for the date bar bg if custom theme is used, we want to make sure
// that the text displayed  is always readable. Inverse colors provide the best contrast.

QString MainWindow::InvertColor(QString textcolor){

    using namespace std;
    QColor input;
    input.setNamedColor(textcolor);

    int ired, igreen, iblue;

    ired=input.red();
    igreen=input.green();
    iblue=input.blue();

    //cout << "Input RGB: " << ired << igreen << iblue << endl;

    int ored, ogreen, oblue;

    ored=255-ired;
    ogreen=255-igreen;
    oblue=255-iblue;

    //cout << "Output RGB: " << ored << ogreen << oblue << endl;

    QColor output=QColor::fromRgb(ored,ogreen,oblue);

    QString invertcolor=output.name();
    //cout << "Inverse hex: "  << invertcolor.toStdString() << endl;
    return invertcolor;
}

//################################################################################################
// Function to style GUI with custom background, font, etc.
void MainWindow::Decorate_GUI(){

    // Alternating Row colors can be very good or very bad depending on color scheme.
    //Users have a way to turn it off at long last
    if(!Buffer::alternate_rows){
        ui->EntryList->setAlternatingRowColors(false);
    }
    else{
        ui->EntryList->setAlternatingRowColors(true);
    }


    // remove icon labels if user doesn't want them
    if(!Buffer::show_icon_labels){
        ui->ConnectButton->setText("");
        ui->DisconnectButton->setText("");
        ui->LastEntry->setText("");
        ui->WriteButton->setText("");
        ui->EditEntryButton->setText("");
        ui->DeleteEntry->setText("");
        ui->NextEntry->setText("");
        ui->TodayButton->setText("");
        ui->ConfigButton->setText("");
    }
    else{ // restore values in case user wishes to enable them at runime
        ui->ConnectButton->setText("Connect");
        ui->DisconnectButton->setText("Disconnect");
        ui->LastEntry->setText("Previous");
        ui->WriteButton->setText("Write in Journal");
        ui->EditEntryButton->setText("Modify Entry");
        ui->DeleteEntry->setText("Delete Entry");
        ui->NextEntry->setText("Next");
        ui->TodayButton->setText("Latest");
        ui->ConfigButton->setText("Preferences");
    }

    if(Buffer::use_custom_theme){


         QString stylesheet=CreateStyleSheet(false,true);

         ui->Output->setStyleSheet(stylesheet);

         if(Buffer::set_tree_background){
             stylesheet=CreateStyleSheet(true,false);
             ui->EntryList->setStyleSheet(stylesheet);
         }

    }
    // clear all styles if the user disables them during runtime
    else{
        ui->Output->setStyleSheet(NULL);

        if(!Buffer::set_tree_background){
            ui->EntryList->setStyleSheet(NULL);
        }
    }
}

//################################################################################################
// this function will eventually launch the help file. Right now, just show a warning message.
void MainWindow::ShowHelp(){
    QMessageBox b;
    b.critical(this,"RoboJournal","The help file is not available in this release. It will be included in RoboJournal 0.3");
}


//################################################################################################
// Fetch entry based on ID
void MainWindow::GetEntry(QString id){
    using namespace std;
    // Current record should be the ID of the most recent selected entry
    Record=id;

    if(Buffer::backend=="MySQL"){
        MySQLCore my;
        QString entry=my.RetrieveEntry(id);
        QString datestamp=my.TimeStamp(id);

        // convert plain text linebreaks to html
        entry.replace("\n","<br>");

        // find and use system colors
        QPalette pal;
        QBrush bg=pal.highlight();
        QBrush fg=pal.highlightedText();

        QColor bgcolor=bg.color();
        QColor fgcolor=fg.color();

        QString color1=bgcolor.name();
        QString color2=fgcolor.name();

        // set masthead and display text
        QString text;

        // use timestamp?

        if(Buffer::keep_time){

            // fetch timestamp
            QString timestamp=my.GetTimestamp(Record);
            //cout << "Record: " + Record.toStdString() << endl;


            // get title if buffer has it
            if(Buffer::show_title){

                QString title=my.GetTitle(id);
                title="<h2>" + title + "</h2>";

                if(Buffer::use_custom_theme){
                    QString invertcolor=InvertColor(Buffer::text_hexcolor);

                    text= title + "<div style=\"background-color:" + Buffer::text_hexcolor + ";  color: " +
                            invertcolor +"; width=100%;\"><small>&nbsp;&nbsp;On " +
                            datestamp + " at "+ timestamp + ",  " +   Buffer::username +
                            " wrote:</small></div><div><br>" + entry + "</div>";

                }
                else{

                    text= title + "<div style=\"background-color:" + color1 + "; margin: -3px; color: " +
                            color2 +"; width=100%; \"><small>&nbsp;&nbsp; On " +
                            datestamp + " at "+ timestamp + ", " +   Buffer::username +
                            " wrote:</small></div><div><br>" + entry + "</div>";
                }
            }
            // do not show title
            else{
                if(Buffer::use_custom_theme){
                    QString invertcolor=InvertColor(Buffer::text_hexcolor);

                    text="<div style=\"background-color:" + Buffer::text_hexcolor + "; margin: -3px; color: " +
                            invertcolor +"; width=100%; padding: 20px 80px;\"><small>&nbsp;&nbsp; On " +
                            datestamp + " at "+ timestamp + ", " + Buffer::username +
                            " wrote:</small></div><div><br>" + entry + "</div>";

                }
                else{

                    text="<div style=\"background-color:" + color1 + "; margin: -3px; color: " +
                            color2 +"; width=100%; padding: 20px 80px;\"><small>&nbsp;&nbsp; On " +
                            datestamp + " at "+ timestamp + ", " +  Buffer::username +
                            " wrote:</small></div><div><br>" + entry + "</div>";
                }

            }

        }


        //no timestamp

        else{
            // get title if buffer has it
            if(Buffer::show_title){

                QString title=my.GetTitle(id);
                title="<h2>" + title + "</h2>";

                if(Buffer::use_custom_theme){
                    QString invertcolor=InvertColor(Buffer::text_hexcolor);

                    text= title + "<div style=\"background-color:" + Buffer::text_hexcolor + "; margin: -3px; color: " +
                            invertcolor +"; width=100%; \"><small>&nbsp;&nbsp; On " +
                            datestamp + ", " +   Buffer::username +
                            " wrote:</small></div><div><br>" + entry + "</div>";

                }
                else{

                    text= title + "<div style=\"background-color:" + color1 + "; margin: -3px; color: " +
                            color2 +"; width=100%; \"><small>&nbsp;&nbsp; On " +
                            datestamp + ", " +   Buffer::username +
                            " wrote:</small></div><div><br>" + entry + "</div>";
                }
            }
            // do not show title
            else{
                if(Buffer::use_custom_theme){
                    QString invertcolor=InvertColor(Buffer::text_hexcolor);

                    text="<div style=\"background-color:" + Buffer::text_hexcolor + "; margin: -3px; color: " +
                            invertcolor +"; width=100%; \"><small>&nbsp;&nbsp; On " +
                            datestamp + ", " +   Buffer::username +
                            " wrote:</small></div><div><br>" + entry + "</div>";

                }
                else{

                    text="<div style=\"background-color:" + color1 + "; margin: -3px; color: " +
                            color2 +"; width=100%; \"><small>&nbsp;&nbsp; On " +
                            datestamp + ", " +   Buffer::username +
                            " wrote:</small></div><div><br>" + entry + "</div>";
                }

            }

        }

        // set entry text
        ui->Output->setText(text);
    }
}

//################################################################################################
// Function that sets up main window GUI when mainwindow is called at startup
void MainWindow::PrimaryConfig(){
    switch(Buffer::toolbar_pos){

        case 0:
            this->addToolBar(Qt::LeftToolBarArea,ui->mainToolBar);
        break;

        case 1:
            this->addToolBar(Qt::TopToolBarArea,ui->mainToolBar);
        break;

        case 2:
            this->addToolBar(Qt::RightToolBarArea,ui->mainToolBar);
        break;
    }



    // Setup Statusbar
    ui->TotalCount->clear();
    ui->statusBar->addWidget(ui->StatusMessage,1);
    ui->statusBar->addPermanentWidget(ui->TotalCount,0);


    // setup toolbars
    ui->mainToolBar->addWidget(ui->ConnectButton);
    ui->mainToolBar->addWidget(ui->DisconnectButton);
    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addWidget(ui->TodayButton);
    ui->mainToolBar->addWidget(ui->LastEntry);
    ui->mainToolBar->addWidget(ui->NextEntry);

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(ui->WriteButton);
    ui->mainToolBar->addWidget(ui->EditEntryButton);

    // This function should be invisible in the beta until the feature is fully built
    ui->SearchButton->setVisible(false);

    // hide export entry feature for now... we need to include it in 0.3
    ui->actionSave_Current_Entry->setVisible(false);

    ui->mainToolBar->addWidget(ui->SearchButton);


    ui->mainToolBar->addWidget(ui->DeleteEntry);

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(ui->ConfigButton);


    ui->WriteButton->setDisabled(true);
    ui->actionPrint->setDisabled(true);

    ui->NextEntry->setDisabled(true);
    ui->LastEntry->setDisabled(true);
    ui->TodayButton->setDisabled(true);
    ui->SearchButton->setDisabled(true);
    ui->DeleteEntry->setDisabled(true);
    ui->EditEntryButton->setDisabled(true);
    ui->DisconnectButton->setDisabled(true);

    ui->actionLatest_Entry->setDisabled(true);
    ui->actionNext_Entry->setDisabled(true);
    ui->actionPrevious_Entry->setDisabled(true);
    ui->actionDisconnect->setDisabled(true);
    ui->actionWrite->setDisabled(true);
    ui->actionDelete_Current_Entry->setDisabled(true);
    ui->actionEdit_Selected_Entry->setDisabled(true);

    QPixmap noicon("qrc:///icons/database_error.png");
    ui->StatusMessage->setPixmap(noicon);
    ui->StatusMessage->setText("Click <b>Connect</b> (or hit <b>F2</b>) to begin working with a journal database.");

    ui->EntryList->setColumnWidth(0,450);

    //Column 1 (Entry ID) is important since it contains the DB row ID, that we check the DB with
    // but it's not necessary for the user to see the column since it just
    // clutters the form
    ui->EntryList->setColumnWidth(1,70);
    ui->EntryList->setColumnHidden(1,true);

    // flip the connection active switch
    ConnectionActive=false;


    // keep the program from crashing if the user clicks Modify before selecting an entry
    CurrentID="-1";

    //Decorate the GUI
    Decorate_GUI();
}

//################################################################################################
// Connect to database function
void MainWindow::Connect(){
    using namespace std;
    DBLogin l(this);
    l.setWindowTitle("New Connection [" + Buffer::backend + "]");

    bool is_connected=false; // check to see if connection is true


    l.Refresh();
    l.exec();

    // if user provided login data
    if(Buffer::login_succeeded){

        this->setCursor(Qt::WaitCursor);

        if(Buffer::backend=="MySQL"){
            this->setCursor(Qt::WaitCursor);
            MySQLCore my;


            MySQLCore::dialogX=l.x();
            MySQLCore::dialogY=l.y();

            bool success=my.Connect();
            bool is_sane=my.SanityCheck();

            if(!success){

                // check to make sure the MYSQL driver is installed, return error if false
                // If you're using a static build of QT you're probably never going to see this error
                if(!my.db.isDriverAvailable("QMYSQL")){
                    this->setCursor(Qt::ArrowCursor);
                   QMessageBox j;
                   j.critical(this,"Database Error","The MySQL driver (QT 4.7) is not installed correctly! RoboJournal"
                              " will not be able to use MySQL databases until this problem is fixed.");
                }

                // if the user got this far, the driver is good but there was a different problem encountered
                //logging in. (permission error, bad passwd, etc.)
                if(my.db.isOpenError()){
                    this->setCursor(Qt::ArrowCursor);
                    QMessageBox m;
                    QString reason="<ul><li><li>Make sure that you are authorized to access <b>" +
                            Buffer::database_name + "</b> from this workstation.<li>Make sure the port, username," +
                            " and password you provided are correct.</ul>Please check and/or rectify these problems and try again.";
                    m.critical(this,"Connection Error","Connection attempt with <b>" +
                               Buffer::database_name + "</b> on <b>" +
                               Buffer::host + "</b> failed." + reason );

                    // allow user to try again
                    l.ResetPassword();
                    Connect();

                }

            }
            else{

                if(!is_sane){

                    // does the journal fail sanity check because it is old....
                    if(Buffer::needs_upgrade){

                        // show upgrade dialog
                        QMessageBox u(this);
                        u.setIcon(QMessageBox::Question);
                        u.setWindowTitle("Journal Upgrade Required");
                        u.setText("<b>" + Buffer::database_name + "</b> is a valid journal but it must be upgraded before RoboJournal "
                        + Buffer::version + " can use it. After this journal has been upgraded to the new format, it will no longer be "
                        "compatible with older versions of RoboJournal.<br><br> Do you want to upgrade this journal now?");

                        u.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                        u.setDefaultButton(QMessageBox::Yes);
                        int choice=u.exec();

                        switch(choice){
                        case QMessageBox::Yes:
                            // Update journal
                            cout << "OUTPUT: Starting journal upgrade process...";
                            UpgradeJournal();
                            break;
                        case QMessageBox::No:
                            // do nothing
                            break;
                        }

                        // reset upgrade flag
                        Buffer::needs_upgrade=false;
                    }

                    // or because it is just 100% invalid due to structure
                    else{
                        QMessageBox m;
                        m.critical(this,"Integrity Error","Database <b>" + Buffer::database_name +
                                   "</b> is not a valid journal!");

                        cout << "ERROR: Database " << Buffer::database_name.toStdString() <<
                                " FAILED sanity check, aborting load sequence!" << endl;

                    }
                }
                else{


                    ui->WriteButton->setEnabled(true);


                    ui->TodayButton->setEnabled(true);
                    ui->SearchButton->setEnabled(true);
                    ui->EditEntryButton->setEnabled(true);
                    ui->DisconnectButton->setEnabled(true);
                    ui->NextEntry->setEnabled(true);
                    ui->LastEntry->setEnabled(true);
                    ui->DeleteEntry->setEnabled(true);
                    ui->actionWrite->setEnabled(true);
                    ui->actionLatest_Entry->setEnabled(true);
                    ui->actionNext_Entry->setEnabled(true);
                    ui->actionPrevious_Entry->setEnabled(true);
                    ui->actionDisconnect->setEnabled(true);
                    ui->actionDelete_Current_Entry->setEnabled(true);
                    ui->actionEdit_Selected_Entry->setEnabled(true);
                    ui->actionPrint->setEnabled(true);

                    ui->actionConnect->setDisabled(true);
                    ui->ConnectButton->setDisabled(true);

                    // Get ID list
                    int year_range=Buffer::entryrange.toInt();
                    IDList=my.Create_ID_List(year_range);

                    CreateTree();
                    ui->StatusMessage->setText("Connected to " + Buffer::backend  + " database <b>" +
                    Buffer::database_name + "</b> on <b>" +  Buffer::host + "</b> as user <b>\"" +
                    Buffer::username + "\"</b>");

                    is_connected=true;

                }
                this->setCursor(Qt::ArrowCursor);

            }


        }

        if(Buffer::backend=="Postgres"){
            PSQLCore po;
            bool success=po.Connect();

            if(!success){
                QMessageBox m;
                m.critical(this,"Error","Connection attempt with <b>" +
                           Buffer::database_name + "</b> on <b>" +
                           Buffer::host + "</b> failed.");
            }
            else{
                ui->WriteButton->setEnabled(true);


                ui->TodayButton->setEnabled(true);
                ui->SearchButton->setEnabled(true);
                ui->EditEntryButton->setEnabled(true);
                ui->DisconnectButton->setEnabled(true);
                ui->NextEntry->setEnabled(true);
                ui->LastEntry->setEnabled(true);

                ui->ConnectButton->setDisabled(true);
                ui->StatusMessage->setText("Connected to <b>" + Buffer::database_name + "</b> on <b>" +
                Buffer::host + "</b> as user <b>\"" + Buffer::username + "\"</b>");
            }

        }
    }
    if(Buffer::backend=="SQLite"){

        SQLiteCore sl;
        bool success=sl.Connect();

        if(!success){
            QMessageBox m;
            m.critical(this,"Error", Buffer::backend + " connection attempt with <b>" +
                       Buffer::database_name + "</b> on <b>" +
                       Buffer::host + "</b> failed.");
        }
        else{
            ui->WriteButton->setEnabled(true);


            ui->TodayButton->setEnabled(true);
            ui->SearchButton->setEnabled(true);
            ui->EditEntryButton->setEnabled(true);
            ui->DisconnectButton->setEnabled(true);
            ui->NextEntry->setEnabled(true);
            ui->LastEntry->setEnabled(true);

            ui->ConnectButton->setDisabled(true);
            ui->StatusMessage->setText("Connected to <b>" + Buffer::database_name + "</b> on <b>" +
            Buffer::host + "</b> as user <b>\"" + Buffer::username + "\"</b>");
        }



        // create the entry tree
        CreateTree();



        this->setCursor(Qt::ArrowCursor);

        // flip the connection active switch
        ConnectionActive=true;



    }

    // connect to latest entry automatically

    if((is_connected && num_records != 0) || (ConnectionActive)){

        ui->NextEntry->setEnabled(true);
        ui->LastEntry->setEnabled(true);
        ui->TodayButton->setEnabled(true);

        MostRecent();
        Record=CurrentID;
    }
    // if there are no entries at login, disable a few buttons to prevent crashes
    else{

        if(num_records==0){
            ui->TodayButton->setEnabled(false);
            ui->NextEntry->setEnabled(false);
            ui->LastEntry->setEnabled(false);
            ui->DeleteEntry->setEnabled(false);
            ui->EditEntryButton->setEnabled(false);
            ui->actionLatest_Entry->setDisabled(true);
            ui->actionNext_Entry->setDisabled(true);
            ui->actionPrevious_Entry->setDisabled(true);
            ui->actionDelete_Current_Entry->setDisabled(true);
            ui->actionEdit_Selected_Entry->setDisabled(true);
        }

    }
 }

//################################################################################################
/*
  Launch editor in revision mode. QString entry is the selected index.

  */
void MainWindow::Modify(){

    if(CurrentID != "-1"){

        Buffer::editentry=CurrentID;
        Buffer::editmode=true;

        Editor e(this);
        e.exec();

        // Update Tree
        CreateTree();

        // refresh entry in Output Pane
        GetEntry(CurrentID);

        ui->StatusMessage->setText("Connected to " + Buffer::backend  + " database <b>" +
        Buffer::database_name + "</b> on <b>" +  Buffer::host + "</b> as user <b>\"" +
        Buffer::username + "\"</b>");

        // keep current entry selected
        HighlightCurrentSelection(CurrentID);


    }
    else{ // show error if no  valid entries are selected
        QMessageBox a;
        a.critical(this,"RoboJournal","Please select an entry to modify before clicking the <b>Modify Entry</b> button.");

    }
}
//################################################################################################
// Delete an entry
void MainWindow::DeleteSelectedEntry(){

    MySQLCore my;

    // make sure we're not working with a node, since nodes always have -1 as Current ID
    if(CurrentID!="-1"){

        if(Buffer::showwarnings){
            QMessageBox b;
            int choice=b.question(this,"Delete entry?","Do you really want to delete the selected journal entry?"
            " This action can't be undone.",QMessageBox::Cancel,QMessageBox::Ok);

            switch(choice){
                case QMessageBox::Ok:
                    if(Buffer::backend=="MySQL"){
                        my.DeleteEntry(CurrentID);
                        CreateTree();
                        ui->StatusMessage->setText("Connected to " + Buffer::backend  + " database <b>" +
                        Buffer::database_name + "</b> on <b>" +  Buffer::host + "</b> as user <b>\"" +
                        Buffer::username + "\"</b>");
                        ui->Output->setPlainText(NULL);
                    }
                break;

                case QMessageBox::Cancel:
                // do nothing
                break;


            }
        }
        else{
            if(Buffer::backend=="MySQL"){
                my.DeleteEntry(CurrentID);
                CreateTree();
                ui->StatusMessage->setText("Connected to " + Buffer::backend  + " database <b>" +
                Buffer::database_name + "</b> on <b>" +  Buffer::host + "</b> as user <b>\"" +
                Buffer::username + "\"</b>");
                ui->Output->setPlainText(NULL);
            }
        }



    }
}

//################################################################################################
void MainWindow::Disconnect(){

    /*
      This function doesn't really terminate a connection since there is no persistent connection...
      all SQL queries are called directly as needed. it just resets the MainWindow class to its
      intial state so a new connection could be made later.
      */

    ui->WriteButton->setDisabled(true);

    ui->NextEntry->setDisabled(true);
    ui->LastEntry->setDisabled(true);

    ui->TodayButton->setDisabled(true);
    ui->SearchButton->setDisabled(true);
    ui->DeleteEntry->setDisabled(true);
    ui->EditEntryButton->setDisabled(true);
    ui->DisconnectButton->setDisabled(true);
    ui->ConnectButton->setEnabled(true);

    ui->Output->setText(NULL);
    ui->EntryList->clear();

    ui->actionWrite->setDisabled(true);
    ui->actionDisconnect->setDisabled(true);
    ui->actionLatest_Entry->setDisabled(true);
    ui->actionNext_Entry->setDisabled(true);
    ui->actionPrevious_Entry->setDisabled(true);
    ui->actionDelete_Current_Entry->setDisabled(true);
    ui->actionEdit_Selected_Entry->setDisabled(true);
    ui->actionConnect->setEnabled(true);
    ui->EntryList->setHeaderHidden(true);
    ui->actionWrite->setDisabled(true);
    ui->actionPrint->setDisabled(true);


    // Clear the ID list so we don't have data from previous connections on it. That sure would cause problems....
    IDList.clear();

    // flip the connection active switch
    ConnectionActive=false;


    ui->StatusMessage->setText("Terminated connection with <b>" + Buffer::host + "</b>");
    ui->TotalCount->clear();
    ui->TotalCount->setToolTip(NULL);
}

//################################################################################################
// show preferences window
void MainWindow::Preferences(){
    Config c(this);
    c.setWindowTitle("RoboJournal Preferences");

//    if(!this->isMaximized()){
//        int posX=this->x()+200;
//        int posY=this->y()+80;
//        //c.move(posX,posY);
//    }


    c.exec();



    if(c.MadeChanges){
        ConfigManager cm;
        cm.ReadConfig();

        if(!ConnectionActive && ui->DisconnectButton->isEnabled()){
            CreateTree();
            ui->StatusMessage->setText("Connected to " + Buffer::backend  + " database <b>" +
            Buffer::database_name + "</b> on <b>" +  Buffer::host + "</b> as user <b>\"" +
            Buffer::username + "\"</b>");
        }

        // Setup toolbar position again in case user changed it
        switch(Buffer::toolbar_pos){

            case 0:
                this->addToolBar(Qt::LeftToolBarArea,ui->mainToolBar);
            break;

            case 1:
                this->addToolBar(Qt::TopToolBarArea,ui->mainToolBar);
            break;

            case 2:
                this->addToolBar(Qt::RightToolBarArea,ui->mainToolBar);
            break;
        }
    }

    //Re-decorate the GUI
    Decorate_GUI();

}
//################################################################################################
// show credits window
void MainWindow::ShowCredits(){
    AboutRJ a(this);

  int width=a.width();
  int height=a.height();
  a.setMaximumSize(width, height);
  a.setMinimumSize(width, height);
  a.setWindowTitle("About RoboJournal");
  a.exec();
}

//################################################################################################
// Call editor dialog class
void MainWindow::Write(){
    using namespace std;

    // Start Editor in new entry mode
    Buffer::editmode=false;

    // Declare and show a journal editor object
    Editor e(this);
    e.setWindowTitle("Untitled Entry -- RoboJournal");
    e.exec();

    // Refresh the entry list
    cout << "OUTPUT: Re-creating entry list to include newest entry." << endl;
    CreateTree();

    // rebuild ID list
    MySQLCore my;
    int year_range=Buffer::entryrange.toInt();
    IDList.clear();
    IDList=my.Create_ID_List(year_range);


    // Reset Status bar message
    ui->StatusMessage->setText("Connected to " + Buffer::backend  + " database <b>" +
    Buffer::database_name + "</b> on <b>" +  Buffer::host + "</b> as user <b>\"" +
    Buffer::username + "\"</b>");
}

//################################################################################################
// Function that highlights the tree item corresponding to current entry being viewed
void MainWindow::HighlightCurrentSelection(QString CurrentID){
    using namespace std;
    ui->EntryList->clearSelection();
    QTreeWidgetItemIterator it(ui->EntryList,QTreeWidgetItemIterator::NoChildren);

        while (*it) {
            if ((*it)->text(1) == CurrentID)
                (*it)->setSelected(true);
                ++it;

                // Make sure CurrentID always reflects current highlighted index
                // This is necessary in case the user wants to edit an entry
                this->CurrentID=CurrentID;
        }

}

//################################################################################################
// All-important function that sets up the Entry tree list.
void MainWindow::CreateTree(){
    using namespace std;
    ui->StatusMessage->setText("Populating entry list, please wait...");
    QIcon rooticon(":/icons/database.png");
    //QIcon monthicon(":/icons/bullet_black.png");
    //QIcon yearicon(":/icons/bullet_blue.png");

    QIcon entryicon(":/icons/pencil.png");

    QFont heavy("sans",10);
    heavy.setWeight(QFont::DemiBold);

    int totalcount=0;

    // clear message and tree panes
    ui->EntryList->clear();
    ui->Output->clear();

    // MySQL code
    if(Buffer::backend=="MySQL"){

        // sort entries by day
        if(Buffer::sortbyday){
            cout << "OUTPUT: Creating entry list from contents of MySQL database " << Buffer::database_name.toStdString() << " ...";
            MySQLCore my;

            bool NewJournal=true;

            QList<QString> YearList;
            QList<QString> MonthList;
            QList<QString> DayList;
            QList<QString> EntryList;

            YearList=my.getYear();


            QListIterator<QString> IteratorYear(YearList);


            ui->EntryList->setHeaderLabel("Database contents (chronological)");
            ui->EntryList->setToolTip(NULL);
            ui->EntryList->setHeaderHidden(false);

            QString db=Buffer::database_name;



            QTreeWidgetItem *root = new QTreeWidgetItem(ui->EntryList);
            root->setText(0, db);
            root->setIcon(0,rooticon);
            root->setToolTip(0, "<b>" + Buffer::database_name + "</b>@<b>" + Buffer::host + "</b>");
            //root->setFont(0,heavy);


            for(int y=0; y<YearList.count(); y++){
                QTreeWidgetItem *year = new QTreeWidgetItem(root);

                QString nextyear=IteratorYear.next();
                year->setText(0, nextyear);


                MonthList=my.getMonth(nextyear);
                QListIterator<QString> IteratorMonth(MonthList);


                for(int m=0; m < MonthList.count(); m++){
                    QTreeWidgetItem *month = new QTreeWidgetItem(year);
                    //month->setFont(0,heavy);

                    //month->setIcon(0,monthicon);

                    QString itemmonth=IteratorMonth.next();

                    //cout << "Item month:" << itemmonth.toStdString() <<endl;
                    int switchmonth=itemmonth.toInt();

                    QString longmonth;


                    switch(switchmonth){
                        case 1:
                        longmonth="January";
                        break;

                        case 2:
                        longmonth="February";
                        break;

                        case 3:
                        longmonth="March";
                        break;

                        case 4:
                        longmonth="April";
                        break;

                        case 5:
                        longmonth="May";
                        break;

                        case 6:
                        longmonth="June";
                        break;

                        case 7:
                        longmonth="July";
                        break;

                        case 8:
                        longmonth="August";
                        break;

                        case 9:
                        longmonth="September";
                        break;

                        case 10:
                        longmonth="October";
                        break;

                        case 11:
                        longmonth="November";
                        break;

                        case 12:
                            longmonth="December";
                        break;
                    }

                    DayList=my.getDay(itemmonth,nextyear);
                    //cout << "Daylist: " << DayList.length();
                    QListIterator<QString> IteratorDay(DayList);


                    int EntryCount=0; // keep track of the number of entries for each month

                    for(int d=0; d<DayList.length(); d++){
                        QString itemday=IteratorDay.next();
                        QTreeWidgetItem *day = new QTreeWidgetItem(month);

                        // Organize Date depending on Buffer data

                        int dateformat=Buffer::date_format.toInt();

                        switch(dateformat){

                        case 0: // international
                            day->setText(0, itemday + " " + longmonth  );
                            break;

                        case 1:  // usa
                            day->setText(0,longmonth + " " + itemday);
                            break;

                        case 2: // japan
                            day->setText(0,longmonth + " " + itemday);
                            break;
                        }





                        EntryList=my.getEntries(itemday,itemmonth);

                        QListIterator<QString> IteratorEntry(EntryList);



                        for(int e=0; e<EntryList.length(); e++){
                            QString entry=IteratorEntry.next();
                            QTreeWidgetItem *EntryItem = new QTreeWidgetItem(day);
                            QStringList item=entry.split("|");
                            QString tooltip=longmonth + " " + itemday + ", " +
                                    nextyear + " : " + item[0];

                            EntryItem->setText(0, item[0]);
                            EntryItem->setToolTip(0, tooltip);
                            EntryItem->setText(1, item[1]);
                            EntryItem->setIcon(0,entryicon);

                            // since we know there are entries at this point, NewJournal should be set to false.
                            NewJournal=false;


                            EntryCount++; // auto-increment entry count

                            totalcount++; // update totalcount

                            // append to highlight list;




                            //hilite_list.append();

                        }

                        //if EntryCount==0, we have a new journal. Show a message if this happens.
                        if(EntryCount==0){
                            QMessageBox a;
                            a.information(this,"RoboJournal","Welcome to your new journal! Click <b>Write in Journal</b> (or hit F4) to get started.");
                        }

                        EntryList.clear();

                    }
                    DayList.clear();

                    month->setText(0, longmonth);
                    QString monthcount=QString::number(EntryCount);

                    if(EntryCount==1){
                        month->setToolTip(0, longmonth + " " + nextyear +
                                          " (" + monthcount + " entry)");
                    }
                    else{
                        month->setToolTip(0, longmonth + " " + nextyear +
                                          " (" + monthcount + " entries)");
                    }


                    EntryCount=0;
                }
                MonthList.clear();
            }

            //if NewJournal is still true, that means we have a new journal. Show a message if this happens.
            if(NewJournal){
                root->setHidden(true);
                QMessageBox a;
                a.information(this,"RoboJournal","Welcome to your new journal! Click <b>Write in Journal</b> (or hit F4) to get started.");
            }

            YearList.clear();

            TotalEntryCount(totalcount);


        }

        //#################################################
        //          Sort entries by month only
        //#################################################

        else{
            cout << "OUTPUT: Creating entry list from contents of MySQL database " << Buffer::database_name.toStdString() << " ...";
            MySQLCore my;

            bool NewJournal=true;

            QList<QString> YearList;
            QList<QString> MonthList;
            QList<QString> EntryList;

            YearList=my.getYear();


            QListIterator<QString> IteratorYear(YearList);


            ui->EntryList->setHeaderLabel("Database contents (chronological)");
            ui->EntryList->setToolTip(NULL);
            ui->EntryList->setHeaderHidden(false);

            QString db=Buffer::database_name;

            QTreeWidgetItem *root = new QTreeWidgetItem(ui->EntryList);


            root->setText(0, db);
            root->setIcon(0,rooticon);
            root->setToolTip(0, "<b>" + Buffer::database_name + "</b>@<b>" + Buffer::host + "</b>");
            //root->setFont(0,heavy);



            for(int y=0; y<YearList.count(); y++){
                QTreeWidgetItem *year = new QTreeWidgetItem(root);
                //year->setIcon(0,yearicon);

                QString nextyear=IteratorYear.next();
                year->setText(0, nextyear);
                //year->setFont(0,heavy);

                //cout << "Current Year: " << nextyear.toStdString() << endl;

                MonthList=my.getMonth(nextyear);
                QListIterator<QString> IteratorMonth(MonthList);


                for(int m=0; m < MonthList.count(); m++){
                    QTreeWidgetItem *month = new QTreeWidgetItem(year);
                    //month->setFont(0,heavy);

                    //month->setIcon(0,monthicon);

                    QString itemmonth=IteratorMonth.next();

                    //cout << "Item month:" << itemmonth.toStdString() <<endl;
                    int switchmonth=itemmonth.toInt();

                    QString longmonth;


                    switch(switchmonth){
                        case 1:
                        longmonth="January";
                        break;

                        case 2:
                        longmonth="February";
                        break;

                        case 3:
                        longmonth="March";
                        break;

                        case 4:
                        longmonth="April";
                        break;

                        case 5:
                        longmonth="May";
                        break;

                        case 6:
                        longmonth="June";
                        break;

                        case 7:
                        longmonth="July";
                        break;

                        case 8:
                        longmonth="August";
                        break;

                        case 9:
                        longmonth="September";
                        break;

                        case 10:
                        longmonth="October";
                        break;

                        case 11:
                        longmonth="November";
                        break;

                        case 12:
                            longmonth="December";
                        break;
                    }

                    EntryList=my.getEntriesMonth(itemmonth,nextyear);
                    //cout << "Daylist: " << DayList.length();
                    QListIterator<QString> IteratorEntry(EntryList);

                    int EntryCount=0; // keep track of the number of entries for each month

                    for(int e=0; e<EntryList.length(); e++){
                        QString raw_entry=IteratorEntry.next();
                        //cout << entry.toStdString() << endl;
                        QTreeWidgetItem *EntryItem = new QTreeWidgetItem(month);

                        QStringList item=raw_entry.split(",");

                        // Organize Date depending on Buffer data

                        int dateformat=Buffer::date_format.toInt();
                        QString entry;
                        switch(dateformat){

                        case 0: // international
                            entry=item[2] + " " + longmonth + ": " + item [1];
                            break;

                        case 1:  // usa
                            entry=longmonth + " " + item[2] + ": " + item [1];
                            break;

                        case 2: // japan
                            entry=longmonth + " " + item[2] + ": " + item [1];
                            break;
                        }





                        EntryItem->setText(0,entry);
                        EntryItem->setToolTip(0,entry);

                        EntryItem->setText(1,item[0]);
                        EntryItem->setIcon(0, entryicon);

                        // since we know there are entries at this point, NewJournal should be set to false.
                        NewJournal=false;

                        EntryCount++; // auto-increment entry count

                        totalcount++; // update totalcount
                    }



                    EntryList.clear();



                    QString monthcount=QString::number(EntryCount);

                    month->setText(0, longmonth);

                    // do month post count
                    if(EntryCount==1){
                        month->setToolTip(0, longmonth + " " + nextyear +
                                          " (" + monthcount + " entry)");
                    }
                    else{
                        month->setToolTip(0, longmonth + " " + nextyear +
                                          " (" + monthcount + " entries)");
                    }

                    EntryCount=0;
                }
                MonthList.clear();
            }
            YearList.clear();

            TotalEntryCount(totalcount);

            //if NewJournal is still true, that means we have a new journal. Show a message if this happens.
            if(NewJournal){
                root->setHidden(true);
                QMessageBox a;
                a.information(this,"RoboJournal","Welcome to your new journal! Click <b>Write in Journal</b> (or hit F4) to get started.");
            }

        }
    }
    ui->EntryList->expandAll();
    cout << "Done!" << endl;

    // re-enable a few buttons just in case they were disabled.
    // (e.g. we log into a new journal and then switch to an existing one)

    if(num_records > 0){
        ui->TodayButton->setEnabled(true);
        ui->NextEntry->setEnabled(true);
        ui->LastEntry->setEnabled(true);
        ui->DeleteEntry->setEnabled(true);
        ui->EditEntryButton->setEnabled(true);
        ui->actionLatest_Entry->setDisabled(false);
        ui->actionNext_Entry->setDisabled(false);
        ui->actionPrevious_Entry->setDisabled(false);
        ui->actionDelete_Current_Entry->setDisabled(false);
        ui->actionEdit_Selected_Entry->setDisabled(false);
    }
    else{
        ui->TodayButton->setEnabled(false);
        ui->NextEntry->setEnabled(false);
        ui->LastEntry->setEnabled(false);
        ui->DeleteEntry->setEnabled(false);
        ui->EditEntryButton->setEnabled(false);
        ui->actionLatest_Entry->setDisabled(true);
        ui->actionNext_Entry->setDisabled(true);
        ui->actionPrevious_Entry->setDisabled(true);
        ui->actionDelete_Current_Entry->setDisabled(true);
        ui->actionEdit_Selected_Entry->setDisabled(true);

    }
}

//################################################################################################
// Update status bar label with total entry count
void MainWindow::TotalEntryCount(int totalcount){

    QString count=QString::number(totalcount);

    // update num_count. If num_count==0, the new journal notification will show and we will not
    //jump to first entry at login
    num_records=totalcount;

    if(totalcount==1){
        ui->TotalCount->setText(count  + " entry ");
    }
    else{
        ui->TotalCount->setText(count  + " entries ");
    }

    QString indicator;

    // create status bar messages
    if(Buffer::allentries){
        indicator="<nobr>Total number of entries to date in <b>" + Buffer::database_name + "</b></nobr>";
        ui->TotalCount->setToolTip(indicator);
        ui->statusBar->showMessage("Displaying all entries in the database...",1500);
    }
    else{
        int range=Buffer::entryrange.toInt();

        if(range==1){

            indicator="<nobr>Total number of entries for current range (" +
                    Buffer::entryrange + " year) in <b>" + Buffer::database_name + "</b></nobr>";
             ui->TotalCount->setToolTip(indicator);
             ui->statusBar->showMessage("Displaying all entries from this year...",1500);
        }
        else{
            indicator="<nobr>Total number of entries for current range (" +
                    Buffer::entryrange + " years) in <b>" + Buffer::database_name + "</b></nobr>";
            ui->TotalCount->setToolTip(indicator);
            ui->statusBar->showMessage("Displaying all entries from the last " + Buffer::entryrange
                                       + " years...",1500);
        }
    }
}

//################################################################################################
// Get the most recent entry from the db
void MainWindow::MostRecent(){
    if(Buffer::backend=="MySQL"){
        MySQLCore c;
        QString body=c.Recent();



        // convert plain text linebreaks to html
        body.replace("\n","<br>");

        // find and use system colors
        QPalette pal;
        QBrush bg=pal.highlight();
        QBrush fg=pal.highlightedText();

        QColor bgcolor=bg.color();
        QColor fgcolor=fg.color();

        QString color1=bgcolor.name();
        QString color2=fgcolor.name();

        //QString timestamp=c.TimeStamp(id);

        // display title above entry
        if(Buffer::show_title){

            QString id=c.recordnum;
            QString title=c.GetTitle(id);
            title="<h2>" + title + "</h2>";

            if(Buffer::use_custom_theme){
                QString invertcolor=InvertColor(Buffer::text_hexcolor);
                ui->Output->setText(title + "<div style=\"background-color: " + Buffer::text_hexcolor +
                                    "; color: " + invertcolor + "; width=100%; \"><small>&nbsp;&nbsp; " +
                                    Buffer::username + " wrote:</small></div><div><br>" + body  + "</div>");
            }
            else{
                ui->Output->setText(title + "<div style=\"background-color: " + color1 +
                                    "; color: " + color2 + "; width=100%; \"><small>&nbsp;&nbsp; " +
                                    Buffer::username + " wrote:</small></div><div><br>" + body  + "</div>");
            }

        }
        // don't display title
        else{
            if(Buffer::use_custom_theme){
                QString invertcolor=InvertColor(Buffer::text_hexcolor);
                ui->Output->setText("<div style=\"background-color: " + Buffer::text_hexcolor +
                                    "; color: " + invertcolor + "; width=100%; \"><small>&nbsp;&nbsp; " +
                                    Buffer::username + " wrote:</small></div><div><br>" + body  + "</div>");
            }
            else{
                ui->Output->setText("<div style=\"background-color: " + color1 +
                                    "; color: " + color2 + "; width=100%; \"><small>&nbsp;&nbsp; " +
                                    Buffer::username + " wrote:</small></div><div><br>" + body  + "</div>");
            }

        }



        ui->NextEntry->setDisabled(true);

        CurrentID=QString::number(c.ID);
        HighlightCurrentSelection(CurrentID);
    }
}
//################################################################################################
void MainWindow::GetPrevious(){
    if(Buffer::backend=="MySQL"){
        MySQLCore c;
        QString body=c.GetPrevious();
        ui->Output->setText(body);
    }
}
//################################################################################################
MainWindow::~MainWindow()
{
    delete ui;
}
//################################################################################################
void MainWindow::on_WriteButton_clicked()
{
    Write();
}

//################################################################################################

void MainWindow::on_SearchButton_clicked()
{
    using namespace std;
    EntrySearch es;
    es.exec();

}
//################################################################################################


void MainWindow::on_ConnectButton_clicked()
{
    Connect();
}
//################################################################################################
void MainWindow::on_DisconnectButton_clicked()
{
    Disconnect();

}

//################################################################################################

void MainWindow::on_ConfigButton_clicked()
{
    Preferences();
}
//################################################################################################

void MainWindow::on_actionPreferences_2_triggered()
{
    Preferences();
}
//################################################################################################
void MainWindow::on_actionConnect_triggered()
{
    Connect();
}
//################################################################################################
void MainWindow::on_actionToolbar_triggered(bool checked)
{
    if(!checked){
        ui->mainToolBar->hide();
    }
    else{
        ui->mainToolBar->show();
    }

}

//################################################################################################

void MainWindow::on_actionCopy_triggered()
{
    ui->Output->copy();
}
//################################################################################################
void MainWindow::on_actionSelect_all_triggered()
{
    ui->Output->selectAll();
}
//################################################################################################
void MainWindow::on_actionMain_toolbar_triggered(bool checked)
{
    if(!checked){
        ui->mainToolBar->hide();
    }
    else{
        ui->mainToolBar->show();
    }
}
//################################################################################################
void MainWindow::on_actionLeft_default_triggered()
{
    this->addToolBar(Qt::LeftToolBarArea,ui->mainToolBar);
}
//################################################################################################
void MainWindow::on_actionTop_triggered()
{
    this->addToolBar(Qt::TopToolBarArea,ui->mainToolBar);
}
//################################################################################################
void MainWindow::on_actionRight_triggered()
{
    this->addToolBar(Qt::RightToolBarArea,ui->mainToolBar);
}
//################################################################################################
void MainWindow::on_actionQuit_triggered()
{

    this->close();
}
//################################################################################################
void MainWindow::on_actionAbout_RoboJournal_triggered()
{
    ShowCredits();
}
//################################################################################################
void MainWindow::on_TodayButton_clicked()
{
    MostRecent();
}
//################################################################################################
void MainWindow::on_LastEntry_clicked()
{
    //GetPrevious();
    int direction=0;
    GetAdjacent(direction);
}

//################################################################################################

void MainWindow::on_actionLatest_Entry_triggered()
{
    MostRecent();
}
//################################################################################################
void MainWindow::on_actionDisconnect_triggered()
{
    Disconnect();
}

//################################################################################################
// Function that retrieves an entry when its entry is clicked on
void MainWindow::on_EntryList_itemClicked(QTreeWidgetItem *item)
{

    using namespace std;

    // make sure selected item is an entry and not just another subnode
    if(item->child(0)==NULL){
        QString id=item->text(1);

        GetEntry(id);

        // update selected entry
        UpdateSelectedEntry(id);

        //cout << id.toStdString() << endl;
    }
    else{
        //cout << "This is **NOT** an entry!" << endl;
        CurrentID="-1";
    }
}



//################################################################################################
// Function that moves to the next adjacent entry based on row ID.
// direction 1=forward; direction 0 =backward;
void MainWindow::GetAdjacent(int direction){

    using namespace std;
    if(Record!=NULL){
        QListIterator<QString> browser(IDList);
        using namespace std;

        //go backward one entry
        if(direction==0){

                if(!ui->NextEntry->isEnabled()){
                    ui->NextEntry->setEnabled(true);
                    ui->actionNext_Entry->setEnabled(true);
                }

                int position=IDList.indexOf(Record);
                position--;
                //cout << position << endl;

                QString lastindex=IDList.value(position);
                QString id=lastindex;
                //cout << lastindex.toStdString() << endl;

                if(position<0){
                    ui->LastEntry->setDisabled(true);
                    ui->actionPrevious_Entry->setDisabled(true);
                    if(!ui->NextEntry->isEnabled()){
                        ui->NextEntry->setEnabled(true);
                        ui->actionNext_Entry->setEnabled(true);
                    }
                }
                else{

                    if(CurrentID != "-1"){
                        GetEntry(id);
                        Record=lastindex;
                    }
                }


        }
        // go forward one entry
        if(direction==1){

            if(browser.next()==NULL){
                ui->NextEntry->setDisabled(true);
                ui->actionNext_Entry->setDisabled(true);
            }
            else{
                if(!ui->LastEntry->isEnabled()){
                    ui->LastEntry->setEnabled(true);
                    ui->actionPrevious_Entry->setEnabled(true);
                }



                int position=IDList.indexOf(Record);
                position++;

                QString nextindex=IDList.value(position);
                QString id=nextindex;
                //cout << nextindex.toStdString() << endl;

                if(position==IDList.size()){
                    ui->NextEntry->setDisabled(true);
                    ui->actionNext_Entry->setDisabled(true);
                    if(!ui->LastEntry->isEnabled()){
                        ui->LastEntry->setEnabled(true);
                        ui->actionPrevious_Entry->setEnabled(true);
                    }
                }
                else{

                    if(CurrentID != "-1"){
                        GetEntry(id);
                        Record=nextindex;
                    }


                }

            }
        }
    }

    if(CurrentID != "-1"){
       HighlightCurrentSelection(Record);
    }


}

//################################################################################################

void MainWindow::on_NextEntry_clicked()
{
    int direction=1;
    GetAdjacent(direction);
}
//################################################################################################
void MainWindow::on_actionPrevious_Entry_triggered()
{
    int direction=0;
    GetAdjacent(direction);
}
//################################################################################################
void MainWindow::on_actionNext_Entry_triggered()
{
    int direction=1;
    GetAdjacent(direction);
}

//################################################################################################

void MainWindow::on_actionEdit_Selected_Entry_triggered()
{
    Modify();
}

//################################################################################################

void MainWindow::on_actionSetup_Wizard_2_triggered()
{

    FirstRun f;


    if(Buffer::showwarnings){



        QMessageBox m;
        int reconfigure=m.question(this,"Reconfigure RoboJournal?",
        "Running the RoboJournal Setup Wizard again will purge all existing configuration settings. "
        "(however, any journal databases you have already created will be preserved) "
        " Are you sure you want to do this?",QMessageBox::Cancel,QMessageBox::Ok);

        switch(reconfigure){
            case QMessageBox::Ok:
            f.exec();

            break;

            case QMessageBox::Cancel:
            // do nothing
            break;
        }
    }
    else{
        f.exec();
    }
}

//################################################################################################
void MainWindow::on_actionPrint_triggered()
{
    if(ui->Output->toPlainText()!=NULL){
//        QMessageBox m;
//        m.critical(this,"Error","Printing is not implemented yet!");

        Print();
        //ui->Output->print();
    }
}

//################################################################################################

void MainWindow::on_DeleteEntry_clicked()
{
    DeleteSelectedEntry();
}

//################################################################################################
void MainWindow::on_actionDelete_Current_Entry_triggered()
{
    DeleteSelectedEntry();
}

//################################################################################################
void MainWindow::on_actionWrite_triggered()
{
    Write();
}
//################################################################################################
void MainWindow::on_actionHelp_file_triggered()
{
    ShowHelp();
}
//################################################################################################
void MainWindow::on_EditEntryButton_clicked()
{
Modify();
}