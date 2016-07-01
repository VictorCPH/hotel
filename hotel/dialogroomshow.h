#ifndef DIALOGROOMSHOW_H
#define DIALOGROOMSHOW_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class Dialogroomshow;
}

class Dialogroomshow : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogroomshow(QWidget *parent = 0);
    void showinformatin();
    ~Dialogroomshow();

private slots:
    void on_pushButton_clicked();

    void on_tableWidget_pricetab_itemDoubleClicked(QTableWidgetItem *item);

    void on_pushButton_yesforbuy_clicked();

private:
    Ui::Dialogroomshow *ui;
};

#endif // DIALOGROOMSHOW_H
