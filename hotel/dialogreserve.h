#ifndef DIALOGRESERVE_H
#define DIALOGRESERVE_H
#include <QDialog>

#include<hotel.h>

namespace Ui {
class Dialogreserve;
}

class Dialogreserve : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogreserve(QWidget *parent = 0);
    int roomnum_can_use(int roomtype);
    void show_room_can_use(int i);
    int check_in();
    void show_tabWidget_currentChanged();
    void ChangeRoomlink(int a,int b,Clientlink newptr);
    ~Dialogreserve();

private slots:

    void on_pushButton_yes_clicked();

    void on_pushButton_2_clicked();

    void on_checkin_clicked();

    void on_pushButton_yes_2_clicked();


    void on_radioButton_toggled();

    void on_radioButton_2_toggled();

    void on_radioButton_3_toggled();

    void on_radioButton_4_toggled();

    void on_radioButton_5_toggled();

    void on_cancel_clicked();

    void on_cancel_reserve_clicked();

    void on_yes_clicked();

    void on_esc_clicked();

private:
    Ui::Dialogreserve *ui;
    int room_type;
};

#endif // DIALOGRESERVE_H
