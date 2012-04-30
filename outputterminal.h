#ifndef OUTPUTTERMINAL_H
#define OUTPUTTERMINAL_H

#include <QDialog>

namespace Ui {
    class OutputTerminal;
}

class OutputTerminal : public QDialog
{
    Q_OBJECT

public:
    explicit OutputTerminal(QWidget *parent = 0);
    ~OutputTerminal();

private:
    Ui::OutputTerminal *ui;
};

#endif // OUTPUTTERMINAL_H
