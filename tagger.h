#ifndef TAGGER_H
#define TAGGER_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class Tagger;
}

class Tagger : public QDialog
{
    Q_OBJECT
    
public:
    explicit Tagger(QWidget *parent = 0);
    ~Tagger();

    static QString id_num;
    static QString title;
    


private slots:
    void on_RemoveTag_clicked();

    void on_TagList_itemSelectionChanged();

    void on_AddTag_clicked();

    void on_TagChooser_editTextChanged(const QString &arg1);

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_NewTag_clicked();

    void on_TagChooser_currentIndexChanged();

private:
    Ui::Tagger *ui;

    void AddTag(QString newtag);
    void DeleteTag();
    void TagAggregator();
    void SaveTags();
    void LoadTags();
    void AddTagToList();
    QString ExportTagList();


};

#endif // TAGGER_H
