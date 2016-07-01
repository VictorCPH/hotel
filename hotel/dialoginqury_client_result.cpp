#include "dialoginqury_client_result.h"
#include "ui_dialoginqury_client_result.h"

Dialoginqury_client_result::Dialoginqury_client_result(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialoginqury_client_result)
{
    ui->setupUi(this);
}

Dialoginqury_client_result::~Dialoginqury_client_result()
{
    delete ui;
}

void Dialoginqury_client_result::show_client_inqury_result(Clientlink client_)
{
       ui->clientname->setText(QString::fromStdString(client_->person.name));
       ui->sex->setText(QString::fromStdString(client_->person.sex));
       ui->identy->setText(QString::fromStdString(client_->person.identy));
       ui->telephone->setText(QString::fromStdString(client_->person.telephone));
       ui->roomnumber->setText(QString::number(client_->person.roomnumber));
       ui->staytime->setText(QString::number(client_->person.staytime));
       ui->timein->setText(QString::number(client_->person.timein));
       ui->leavetime->setText(QString::number(client_->person.leavetime));
       switch(client_->person.roomtype){//?????????????????????
           case single:
               ui->roomtype->setText("单人间");
               break;
           case standard:
               ui->roomtype->setText("标准间");
               break;
           case business:
               ui->roomtype->setText("商务间");    //单人间，标准间，商务间，豪华套房，总统套房
               break;
           case deluxe:
               ui->roomtype->setText("豪华套房");
               break;
           case president:
               ui->roomtype->setText("总统套房");
               break;
           default:
               break;
       }
}



void Dialoginqury_client_result::on_pushButton_yes_clicked()
{
    close();
}
