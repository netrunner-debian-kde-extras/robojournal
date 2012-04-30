#include "setupwizard.h"
#include "buffer.h"
#include <QWizard>
#include <QWizardPage>
#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QTextBrowser>

QWizardPage *startPage(){
    QWizardPage *page = new QWizardPage;
     page->setTitle("Welcome to RoboJournal");


     QLabel *label = new QLabel("This setup Wizard is running because this is the first time you have used Robojournal"
     " <i>or</i> because the configuration file is missing and needs to be re-built. "
     "During this process, you will be able to create a new database from scratch "
     "or tell RoboJournal to use an existing one. <br><br>Click Next to begin.");
     label->setWordWrap(true);

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(label);
     page->setLayout(layout);
     page->setFinalPage(false);

     return page;

}

QWizardPage *DBTypePage(){
    QWizardPage *page = new QWizardPage;
    page->setTitle("Choose Journal Database Type");
    QLabel *label = new QLabel("Robojournal stores all its journal entry data in a database."
    " Several types of databases are available for use, each with different advantages."

    " Choose the type you want to create from the list below.<br><br>If you want to use an"
    " existing journal database, choose the option corresponding to its type.");
    label->setWordWrap(true);

    QRadioButton *sqlite=new QRadioButton("SQLite (recommended)");
    QRadioButton *mysql=new QRadioButton("MySQL");
    QRadioButton *postgres=new QRadioButton("PostgreSQL");

    QTextBrowser *description=new QTextBrowser();


    // disable postgres for now
    postgres->setDisabled(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(sqlite);
    layout->addWidget(mysql);
    layout->addWidget(postgres);
    //layout->addWidget(description);

    sqlite->setToolTip("<p>SQLite is a simple built-in database that runs on the local system instead of a remote server."
                  " Databases are stored as a simple file in the <i>.robojournal</i> directory."
                  " Use SQLite if you don't need a lot of security or are fairly new to the concept of administrating databases.</p>");

    mysql->setToolTip("<p>MySQL is a very popular database solution that uses a client-server design. The database is stored on"
                      " a remote server and a client terminal (your computer) is used to interface with it. MySQL is more powerful than"
                      " SQLite and offers greater security, but can be harder to administrate/configure if you don't know what you are doing."
                      " Note: Although you can easily run a MYSQL server instance on the same computer as Robojournal, you need to know how to manually configure MySQL"
                      " to be able to access remote databases on remote servers with RoboJournal.</p>");

    sqlite->setChecked(true);

    page->setLayout(layout);
    page->setFinalPage(false);


    return page;


}


// Page that lets you decide what to do with DB
QWizardPage *DB_Setup_1(){
    QWizardPage *page = new QWizardPage;
    page->setTitle("Journal Configuration: Step 1");

    QLabel *intro = new QLabel("What do you want to do?<br>");
    QRadioButton *create_new = new QRadioButton("Create a new empty journal");
    QRadioButton *use_existing=new QRadioButton("Configure RoboJournal to use an existing journal");

    QVBoxLayout *layout2 = new QVBoxLayout;
    create_new->setChecked(true);

    layout2->addWidget(intro);
    layout2->addWidget(create_new);
    layout2->addWidget(use_existing);

    page->setLayout(layout2);
    return page;

}

QWizardPage *SQLLiteSetup(){

}

QWizardPage *MYSQLSetup(){

}

QWizardPage *MYSQL_CreateDB(){

}

QWizardPage *UseExisting(){

}

void SetupWizard::StartWizard(){

    QWizard setup;
    setup.addPage(startPage());
    setup.addPage(DBTypePage());
    setup.addPage(DB_Setup_1());
    setup.setWindowTitle("RoboJournal Setup Wizard");
    setup.exec();



}

SetupWizard::SetupWizard()
{
}
