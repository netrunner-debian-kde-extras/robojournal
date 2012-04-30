#include "journalfinder.h"
#include "ui_journalfinder.h"

JournalFinder::JournalFinder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JournalFinder)
{
    ui->setupUi(this);
}

JournalFinder::~JournalFinder()
{
    delete ui;
}
