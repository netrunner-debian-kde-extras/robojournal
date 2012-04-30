#ifndef JOURNALFINDER_H
#define JOURNALFINDER_H

#include <QDialog>

namespace Ui {
    class JournalFinder;
}

class JournalFinder : public QDialog
{
    Q_OBJECT

public:
    explicit JournalFinder(QWidget *parent = 0);
    ~JournalFinder();

private:
    Ui::JournalFinder *ui;
};

#endif // JOURNALFINDER_H
