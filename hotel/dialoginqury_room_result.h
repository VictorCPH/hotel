#ifndef DIALOGINQURY_ROOM_RESULT_H
#define DIALOGINQURY_ROOM_RESULT_H

#include <QDialog>
#include<hotel.h>

namespace Ui {
class Dialoginqury_room_result;
}

class Dialoginqury_room_result : public QDialog
{
    Q_OBJECT

public:
    explicit Dialoginqury_room_result(QWidget *parent = 0);
     void show_room_inqury_result(Roomlink);
     Roomlink   room_inqury;
    ~Dialoginqury_room_result();

private slots:
     void on_pushButton_yes_clicked();

private:
    Ui::Dialoginqury_room_result *ui;
};

#endif // DIALOGINQURY_ROOM_RESULT_H
