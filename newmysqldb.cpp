#include "newmysqldb.h"
#include <QWizard>
#include <QWizardPage>
#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QGridLayout>
#include <QLineEdit>

NewMySQLDB::NewMySQLDB()
{
}

QWizardPage *startPage(){

    QWizardPage *page=new QWizardPage();
    page->setTitle("Welcome");

    QLabel *label1=new QLabel("Welcome to the new journal wizard. This tool"
    " will help you set up a new RoboJournal database based on MySQL or SQLite."
    " Click <b>Next</b> to get started.");

    label1->setWordWrap(true);

    QVBoxLayout *layout=new QVBoxLayout();
    layout->addWidget(label1);
    page->setLayout(layout);

    return page;

}

QWizardPage *DBSettings(){

    QWizardPage *page=new QWizardPage();
    page->setTitle("Define Settings");

    QLabel *label1=new QLabel("Provide the name of the database, the default user, and host. If you "
    "want to run the database on this computer, use <em>localhost</em> for the host name. If you want "
    "to run the database on a remote server, provide the domain (example.com) or the IP address. (192.168.x.x) "
    "Lastly, the default user's account must have read and write priviliges on the host.<br>");
    label1->setWordWrap(true);

    QLabel *l_dbname=new QLabel("Database name");
    QLabel *l_dbhost=new QLabel("Database host");
    QLabel *l_dbuser=new QLabel("Default user (root not recommended)");

    QLineEdit *dbname=new QLineEdit();
    QLineEdit *dbhost=new QLineEdit();
    QLineEdit *dbuser=new QLineEdit();

    //QGridLayout *layout1=new QGridLayout();
    QVBoxLayout *layout1=new QVBoxLayout();

    layout1->addWidget(label1);
    layout1->addWidget(l_dbname);
    layout1->addWidget(dbname);
    layout1->addWidget(l_dbhost);
    layout1->addWidget(dbhost);
    layout1->addWidget(l_dbuser);
    layout1->addWidget(dbuser);

    page->setLayout(layout1);

    return page;
}

QWizardPage *DBRoot(){

    QWizardPage *page=new QWizardPage();
    page->setTitle("Root access required");
    QString host="host";

    QLabel *label1=new QLabel("In order to create the new database, RoboJournal needs root-level access "
    "to the MySQL server process running on " + host + ". If host is a remote server, be sure that it is configured "
    "to accept remote connections or the database creation attempt will fail.<br>");
    label1->setWordWrap(true);

    QLabel *l_root = new QLabel("Root password");
    QLineEdit *root = new QLineEdit();
    root->setEchoMode(QLineEdit::Password);

    QVBoxLayout *layout1=new QVBoxLayout();
    layout1->addWidget(label1);
    layout1->addWidget(l_root);
    layout1->addWidget(root);

    page->setLayout(layout1);
    return page;
}

void NewMySQLDB::StartWizard(){

    QWizard w;
    w.addPage(startPage());
    w.addPage(DBSettings());
    w.addPage(DBRoot());
    w.setWindowTitle("New Journal Wizard (MySQL)");
    w.exec();
}
