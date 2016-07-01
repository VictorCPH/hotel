#ifndef DIALOGCLIENTIN_H
#define DIALOGCLIENTIN_H
#include <hotel.h>
#include <QDialog>

namespace Ui {
class Dialogclientin;
}

class Dialogclientin : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogclientin(QWidget *parent = 0);
    ~Dialogclientin();

private slots:

    void on_pushButton_yes_clicked();

    void changeValue();

    void on_radioButton_business_clicked();

    void on_radioButton_single_clicked();

    void on_radioButton_standard_clicked();

    void on_radioButton_deluxe_clicked();

    void on_radioButton_president_clicked();

    void on_pushButton_no_clicked();

    void on_history_inqury_clicked();

private:
    Ui::Dialogclientin *ui;
};

void changeRoomlink(int a,int b, Clientlink newptr);

#endif // DIALOGCLIENTIN_H
