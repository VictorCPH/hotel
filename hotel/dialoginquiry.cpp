#include "dialoginquiry.h"
#include "ui_dialoginquiry.h"


#include<hotel.h>
#include<dialoginqury_room_result.h>
#include<dialoginqury_client_result.h>
#include <QMessageBox>

extern Roomlink hotel[LINKNUM];
extern Clientlink clients;

Dialoginquiry::Dialoginquiry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialoginquiry)
{
    ui->setupUi(this);
    i=2;
}

Dialoginquiry::~Dialoginquiry()
{
    delete ui;
}

Roomlink Dialoginquiry::Roominquiry(int roomnumber){
   int r;
   string clientname;//???


   Roomlink searchptr=NULL;
   r=roomnumber/100;
   if(r>=1&&r<=5){
        searchptr=hotel[r-1]->next;
        while(searchptr&&searchptr!=NULL&&searchptr->room.roomnumber!=roomnumber){
              searchptr=searchptr->next;
        }

        if(searchptr==NULL){
              searchptr=hotel[r-1+5]->next;
              while(searchptr!=NULL&&searchptr->room.roomnumber!=roomnumber){
                    searchptr=searchptr->next;
              }
              if(searchptr==NULL){
                    QMessageBox::information(this,tr(""),tr("输入错误房间号"));
                    return NULL;
              }
        }
        return searchptr;
   }
   else   {
       QMessageBox::information(this,tr(""),tr("输入错误房间号"));
       return NULL;
   }
}



Clientlink Dialoginquiry::Clientinquiry(string name){

   Clientlink  searchptr=NULL;
   searchptr=clients->next;
   while(searchptr!=NULL){
       if(searchptr->person.name==name)
         return searchptr;
       searchptr=searchptr->next;
   }
   QMessageBox::information(this,tr(""),tr("不存在此客户"));
   return NULL;
}

void Dialoginquiry::on_radioButton_toggled()
{
     i=0;
}

void Dialoginquiry::on_radioButton_2_toggled()
{
    i=1;
}

void Dialoginquiry::on_pushButton_clicked()
{
    if(ui->lineEdit->text().toStdString()==""){
        QMessageBox::information(this,tr(""),tr("信息填写不全"));
        return;
    }
    if(i==0){

       Dialoginqury_room_result a;
       int roomnumber=ui->lineEdit->text().toInt();
       if(Roominquiry(roomnumber)){
          a.room_inqury=Roominquiry(roomnumber);
          a.show_room_inqury_result(a.room_inqury);
          a.exec();
       }

    }

    else if(i==1){
        Dialoginqury_client_result a;
        string name=ui->lineEdit->text().toStdString();
        if(Clientinquiry(name)){
           a.client_inqury=Clientinquiry(name);
           a.show_client_inqury_result(a.client_inqury);
           a.exec();
        }
    }
    else {
        QMessageBox::information(this,tr(""),tr("请选择查询方式"));
        return;
    }

}

void Dialoginquiry::on_pushButton_2_clicked()
{
    close();
}
