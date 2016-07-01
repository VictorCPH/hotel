#ifndef DIALOGINQURY_CLIENT_RESULT_H
#define DIALOGINQURY_CLIENT_RESULT_H

#include<hotel.h>
#include <QDialog>

namespace Ui {
class Dialoginqury_client_result;
}

class Dialoginqury_client_result : public QDialog
{
    Q_OBJECT

public:
    explicit Dialoginqury_client_result(QWidget *parent = 0);
    void show_client_inqury_result(Clientlink);
    Clientlink client_inqury;
    ~Dialoginqury_client_result();



private slots:
    void on_pushButton_yes_clicked();

private:
    Ui::Dialoginqury_client_result *ui;
};

#endif // DIALOGINQURY_CLIENT_RESULT_H
