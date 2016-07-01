#ifndef DIALOGINQUIRY_H
#define DIALOGINQUIRY_H

#include<hotel.h>
#include <QDialog>
#include<dialoginqury_client_result.h>

namespace Ui {
class Dialoginquiry;
}

class Dialoginquiry : public QDialog
{
    Q_OBJECT

public:
    explicit Dialoginquiry(QWidget *parent = 0);
    Roomlink Roominquiry(int roomnumber);

    Clientlink Clientinquiry(string name);
    ~Dialoginquiry();

private slots:
    void on_radioButton_toggled();

    void on_radioButton_2_toggled();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Dialoginquiry *ui;
    int i;
};

#endif // DIALOGINQUIRY_H
