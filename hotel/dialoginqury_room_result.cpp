#include "dialoginqury_room_result.h"
#include "ui_dialoginqury_room_result.h"

Dialoginqury_room_result::Dialoginqury_room_result(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialoginqury_room_result)
{
    ui->setupUi(this);
}

Dialoginqury_room_result::~Dialoginqury_room_result()
{
    delete ui;
}

void Dialoginqury_room_result::show_room_inqury_result(Roomlink room_)
{
    ui->name->setText(QString::fromStdString(room_->room.clientname));
    ui->roomnumber->setText(QString::number(room_->room.roomnumber));
    ui->roomprice->setText(QString::number(room_->room.roomprice));
    ui->day->setText(QString::number(room_->room.day,'f',1));
    ui->consumption->setText(QString::number(room_->room.consumption));
    ui->timein->setText(QString::number(room_->room.stayin));
    ui->leavetime->setText(QString::number(room_->room.leavetime));
    switch(room_->room.roomtype){//?????????????????????
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
    switch(room_->room.sign){
        case Empty:
            ui->sign->setText("空闲");
            break;
        case Reserve:
            ui->sign->setText("预定");
            break;
        case Occupy:
            ui->sign->setText("占据");
            break;
        case Repair:
            ui->sign->setText("维修");
            break;
        default:
            break;
    }
}

void Dialoginqury_room_result::on_pushButton_yes_clicked()
{
    close();
}
