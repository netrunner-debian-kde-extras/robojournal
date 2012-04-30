#include "journaleditor.h"
#include "ui_journaleditor.h"

JournalEditor::JournalEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JournalEditor)
{
    ui->setupUi(this);
    this->setWindowTitle("New Journal Entry");
}

JournalEditor::~JournalEditor()
{
    delete ui;
}
