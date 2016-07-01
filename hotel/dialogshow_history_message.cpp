#include "dialogshow_history_message.h"
#include "ui_dialogshow_history_message.h"
#include<hotel.h>

extern Clientlink show_history;
extern int roomnumber_in[66];
extern int roomtype_in[6];
extern int general_staytime2;


Dialogshow_history_message::Dialogshow_history_message(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogshow_history_message)
{
    ui->setupUi(this);
    ui->id->setText(QString::fromStdString(show_history->person.identy));
    ui->name->setText(QString::fromStdString(show_history->person.name));
    ui->telephone->setText(QString::fromStdString(show_history->person.telephone));
    ui->timein->setText(QString::number(show_history->person.timein));
    ui->leavetime->setText(QString::number(show_history->person.leavetime));
    ui->sex->setText(QString::fromStdString(show_history->person.sex));

    int i=0;
    int time1=0,roomnumber1=0;
    for(i=1;i<66;i++){
        if(roomnumber_in[i]>=time1){
            roomnumber1=i;
            time1=roomnumber_in[i];
        }
    }
    if(roomnumber1>=1&&roomnumber1<=13){
        roomnumber1=roomnumber1+100;
        ui->roomnumber->setText(QString::number(roomnumber1));
    }
    else if(roomnumber1>=14&&roomnumber1<=26){
        roomnumber1=(roomnumber1-13)+200;
        ui->roomnumber->setText(QString::number(roomnumber1));
    }
    else if(roomnumber1>=27&&roomnumber1<=39){
        roomnumber1=roomnumber1-26+300;
        ui->roomnumber->setText(QString::number(roomnumber1));
    }
    else if(roomnumber1>=40&&roomnumber1<=52){
        roomnumber1=roomnumber1-39+400;
        ui->roomnumber->setText(QString::number(roomnumber1));
    }
    else if(roomnumber1>=53&&roomnumber1<=64){
        roomnumber1=roomnumber1%52+500;
        ui->roomnumber->setText(QString::number(roomnumber1));
    }



    float day2=general_staytime2/float(roomtype_in[0]);
    ui->staytime->setText(QString::fromStdString("入住总时间：")+QString::number(general_staytime2)+QString::fromStdString("  平均入住时间：")+QString::number(day2,'f',1));
  \
    ui->time->setText(QString::number(roomtype_in[0]));
    int roomtype1=0,time9=0;
    for(i=1;i<6;i++){
        if( roomtype_in[i]>=time9){
            roomtype1=i;
            time9=roomtype_in[i];
        }
    }
    if (roomtype1 == 1)
        ui->roomtype->setText(QString::fromStdString("单人间"));
    else
        if (roomtype1 == 2)
            ui->roomtype->setText(QString::fromStdString("标准间"));
    else
        if (roomtype1 == 3)
            ui->roomtype->setText(QString::fromStdString("商务间"));
    else
        if (roomtype1 == 4)
            ui->roomtype->setText(QString::fromStdString("豪华套房"));
    else
        if (roomtype1 == 5)
            ui->roomtype->setText(QString::fromStdString("总统套房"));


}

Dialogshow_history_message::~Dialogshow_history_message()
{
    delete ui;
}


void Dialogshow_history_message::on_pushButton_yes_clicked()
{
    close();
}
