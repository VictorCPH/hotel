#include "dialogreserve.h"
#include "ui_dialogreserve.h"
#include <QMessageBox>
#include<dialogclientin.h>
#include<hotel.h>
#include<dialoginquiry.h>
#include<QTableWidgetItem>
#include <QDebug>
#include "log_manage.h"

extern Roomlink hotel[LINKNUM];
extern Clientlink clients;
extern Reserve_clientlink head_Reserve_clientlink;
extern Reserve_client  reserve_tab[7][65];
extern int  can_occupy_room_num[30][5];
extern time_t end;
extern struct tm *timeNow; //当前时间
extern int time_difference(int  timein,int nowtime);
extern void Load_reserve_tab();
extern void Can_use_room_num_tab();
enum type roomtype;
extern int backupflag;
string receiveman;
int sign1=0;
int reserve_room=0;

Dialogreserve::Dialogreserve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogreserve)
{
    ui->setupUi(this);
    show_tabWidget_currentChanged();

}



int Dialogreserve::roomnum_can_use(int roomtype){
    int timein=ui->timein->text().toInt();
    int timestay=ui->timestay->text().toInt();
    int nowtime=timeNow->tm_year*10000+timeNow->tm_mon*100+timeNow->tm_mday;

    int i=time_difference(timein,nowtime);
    int num=0;
    int j=0;
    if(i+timestay+1>30||timein<nowtime){
        QMessageBox::information(this,tr(""),tr("不在可预约时间内"));
        reserve_room=2;
        return -1;
    }
    num=can_occupy_room_num[j][roomtype];
    for(j=i;j<i+timestay-1;j++){
        if(can_occupy_room_num[j][roomtype]<num)
             num=can_occupy_room_num[j][roomtype];
    }

    return num;

}

void Dialogreserve::on_radioButton_toggled()
{
    room_type=0;
    roomtype=single;
    int i=0;
    int num;
    int roomnum=ui->roomnum->text().toInt();
    num=roomnum_can_use(i);
    ui->show_room_num->setText(QString::number(num));

    if(num<roomnum&&num!=-1){
        QMessageBox::information(this,tr(""),tr("可预约房间数不足"));
        reserve_room=1;

    }

}


void Dialogreserve::on_radioButton_2_toggled()
{
    room_type=1;
    roomtype=standard;
    int i=1;
    int num;
    int roomnum=ui->roomnum->text().toInt();
    num=roomnum_can_use(i);
    ui->show_room_num->setText(QString::number(num));
    if(num<roomnum&&num!=-1){
        QMessageBox::information(this,tr(""),tr("可预约房间数不足"));
        reserve_room=1;
    }

}

void Dialogreserve::on_radioButton_3_toggled()
{
    room_type=2;
    roomtype=business;
    int i=2;
    int num;
    int roomnum=ui->roomnum->text().toInt();
    num=roomnum_can_use(i);
    ui->show_room_num->setText(QString::number(num));
    if(num<roomnum&&num!=-1){
        QMessageBox::information(this,tr(""),tr("可预约房间数不足"));
        reserve_room=1;
    }

}

void Dialogreserve::on_radioButton_4_toggled()
{
    room_type=3;
    roomtype=deluxe;
    int i=3;
    int num;
    int roomnum=ui->roomnum->text().toInt();
    num=roomnum_can_use(i);
    ui->show_room_num->setText(QString::number(num));
    if(num<roomnum&&num!=-1){
        reserve_room=1;
        QMessageBox::information(this,tr(""),tr("可预约房间数不足"));
    }

}

void Dialogreserve::on_radioButton_5_toggled()
{
    room_type=4;
    roomtype=president;
    int i=4;
    int num;
    int roomnum=ui->roomnum->text().toInt();

    num=roomnum_can_use(i);
    ui->show_room_num->setText(QString::number(num));
    if(num<roomnum&&num!=-1){
        QMessageBox::information(this,tr(""),tr("可预约房间数不足"));
        reserve_room=1;
    }

}


Dialogreserve::~Dialogreserve()
{
    delete ui;
}

void Dialogreserve::on_pushButton_yes_clicked()
{
       // if(ui->telephone->text().toStdString()==""||ui->timein->text().toInt()==""||ui->roomnum->text().toInt()||ui->roomnum->text().toInt())
        if(reserve_room==1){
            QMessageBox::information(this,tr(""),tr("可预约房间数不足"));
            return;
        }
        if(reserve_room==2){
            QMessageBox::information(this,tr(""),tr("不在可预约时间内"));
            return;
        }
        Reserve_clientlink newptr;

        string name="empty",
               telephone="empty";
        int reserve_time,timein,room_num,staytime;

        name=ui->name->text().toStdString();

        telephone=ui->telephone->text().toStdString();
        timein=ui->timein->text().toInt();
        room_num=ui->roomnum->text().toInt();
        staytime=ui->timestay->text().toInt();
        if(name==""||telephone==""||timein==0||room_num==0||staytime==0){
            QMessageBox::information(this,tr(""),tr("信息填写不全"));
            return;
        }
        else{
            QMessageBox::information(this,tr(""),tr("预约成功"));
            reserve_room=0;

            int timestay=ui->timestay->text().toInt();
            int nowtime=timeNow->tm_year*10000+timeNow->tm_mon*100+timeNow->tm_mday;
            int i=time_difference(timein,nowtime),j=0;
            for(j=i;j<i+timestay;j++){
                     can_occupy_room_num[j][room_type]=can_occupy_room_num[j][room_type]-room_num;
            }
        }

        reserve_time=timeNow->tm_year*10000+timeNow->tm_mon*100+timeNow->tm_mday;
        newptr=new Reserve_clientnode;
        newptr->next=NULL;
        newptr->reserve_client.name=name;
        newptr->reserve_client.reserve_stay=staytime;
        newptr->reserve_client.reserve_time=reserve_time;
        newptr->reserve_client.roomtype=roomtype;
        newptr->reserve_client.room_num=room_num;
        newptr->reserve_client.telephone=telephone;
        newptr->reserve_client.timein=timein;

        receiveman = name;                      //更新日志；
        log_manage(hotel[room_type]->next,reserve);
        backupflag = 1;

        Reserve_clientlink lastptr;
        lastptr=head_Reserve_clientlink->next;
        Reserve_clientlink preptr;
        preptr=head_Reserve_clientlink;
        while(lastptr){
            if(lastptr->reserve_client.timein>newptr->reserve_client.timein)
                break;
            else{
                preptr=lastptr;
                lastptr=lastptr->next;
            }
        }
        newptr->next=lastptr;
        preptr->next=newptr;
        head_Reserve_clientlink->reserve_client.room_num++;

        int i=0;
        if(time_difference(timein,reserve_time)<7){
            while(reserve_tab[time_difference(timein,reserve_time)][i].room_num!=0)
                i++;
            reserve_tab[time_difference(timein,reserve_time)][i]=newptr->reserve_client;
        }
        ui->name->setText(QString::fromStdString(""));

        ui->telephone->setText(QString::fromStdString(""));
        ui->timein->setText(QString::fromStdString(""));
        ui->roomnum->setText(QString::fromStdString(""));
        ui->timestay->setText(QString::fromStdString(""));
        ui->show_room_num->setText(QString::fromStdString(""));
        show_tabWidget_currentChanged();
        close();
}

void Dialogreserve::on_pushButton_2_clicked()
{
    close();
}
void Dialogreserve::show_room_can_use(int i)
{
    string str, str1;
    stringstream ss;

    Roomlink t=hotel[i]->next;

    while(t)
    {
        ss<<t->room.roomnumber;
        ss>>str1;
        str = str + str1 + ' ';
        ss.clear();
        t=t->next;
    }
    ui->room_can_use->setText(QString::fromStdString(str));
}
void Dialogreserve::on_checkin_clicked()
{
    Dialogclientin a;
    int i=0;
    for(i=0;i<65;i++){
          if(reserve_tab[0][i].name==ui->name_1->text().toStdString()){
              ui->telephone_1->setText(QString::fromStdString(reserve_tab[0][i].telephone));
              ui->timenin_1->setText(QString::number(reserve_tab[0][i].timein));
              ui->reserve_time_1->setText(QString::number(reserve_tab[0][i].reserve_time));
              ui->timestay_1->setText(QString::number(reserve_tab[0][i].reserve_stay));
              ui->roomnum_1->setText(QString::number(reserve_tab[0][i].room_num));
              if (reserve_tab[0][i].roomtype == single){
                   ui->roomtype_1->setText(QString::fromStdString("单人间"));
                   show_room_can_use(0);
                   }
              else
                  if (reserve_tab[0][i].roomtype == standard){
                      ui->roomtype_1->setText(QString::fromStdString("标准间"));
                      show_room_can_use(1);
                  }
              else
                  if (reserve_tab[0][i].roomtype == business){
                      ui->roomtype_1->setText(QString::fromStdString("商务间"));
                      show_room_can_use(2);
                  }
              else
                  if (reserve_tab[0][i].roomtype == deluxe){
                      ui->roomtype_1->setText(QString::fromStdString("豪华套房"));
                      show_room_can_use(3);
                  }
              else
                  if (reserve_tab[0][i].roomtype == president){
                      ui->roomtype_1->setText(QString::fromStdString("总统套房"));
                      show_room_can_use(4);
                  }
             break;
             return;
          }

    }
    if(i>=65)
        QMessageBox::information(this,tr(""),tr("无此客户预约信息"));
}
void  Dialogreserve::ChangeRoomlink(int a,int b,Clientlink newptr){
    Roomlink 	preptr=NULL,searchptr=NULL,roompreptr=hotel[b],roomsearchptr=hotel[b]->next;
    searchptr=hotel[a]->next;
    preptr=hotel[a];

    while(searchptr&&searchptr->room.roomnumber!=newptr->person.roomnumber){
        preptr=searchptr;
       searchptr=searchptr->next;
    }
    if(searchptr!=NULL){

       searchptr->room.clientname=newptr->person.name;//???
       searchptr->room.sign=Occupy;//???
       searchptr->room.day=1;
       searchptr->room.leavetime = newptr->person.leavetime;

       searchptr->room.stayin=newptr->person.timein;

       preptr->next=searchptr->next;

       hotel[a]->room.roomnumber--;

       searchptr->next=NULL;
       while(roomsearchptr&&roomsearchptr->room.roomnumber<newptr->person.roomnumber){
           roompreptr=roomsearchptr;
           roomsearchptr=roomsearchptr->next;
       }
       if(roomsearchptr==NULL){
           roompreptr->next=searchptr;
       }
       else{
           searchptr->next=roomsearchptr;
           roompreptr->next=searchptr;
       }
       hotel[b]->room.roomnumber++;
    }
    log_manage(searchptr,reserve_to_in);
    backupflag = 1;
}
int Dialogreserve::check_in()
{
   Clientlink newptr=NULL,preptr=clients,searchptr=clients->next;
   Roomlink   searchptr1=NULL;

   preptr=clients;
   searchptr=clients->next;

   newptr = new Clientnode;
   newptr->next=NULL;
   if (ui->name_2->text().isEmpty() || ui->identy_1->text().isEmpty() ||
       ui->telephone_2->text().isEmpty() || ui->roomnumber->text().isEmpty())
   {
        QMessageBox::information(this,tr(""),tr("信息填写不全"));
        return 0;
   }
   else
   {
       newptr->person.name = ui->name_2->text().toStdString();

       newptr->person.sex = ui->sex_1->currentText().toStdString();
       newptr->person.identy = ui->identy_1->text().toStdString();
       newptr->person.telephone = ui->telephone_2->text().toStdString();
       newptr->person.roomnumber = ui->roomnumber->text().toInt();
       newptr->person.staytime = ui->timestay_1->text().toInt();
       if ((newptr->person.roomnumber >= 101 && newptr->person.roomnumber <= 113) ||
           (newptr->person.roomnumber >= 201 && newptr->person.roomnumber <= 213) ||
           (newptr->person.roomnumber >= 301 && newptr->person.roomnumber <= 313) ||
           (newptr->person.roomnumber >= 401 && newptr->person.roomnumber <= 413) ||
           (newptr->person.roomnumber >= 501 && newptr->person.roomnumber <= 513))
           ;//房间号非法输入判断
       else
       {
           QMessageBox::information(this,tr(""),tr("无此房间,请重新输入"));
           return 0;
       }

       newptr->person.timein=ui->timenin_1->text().toInt();//........................
        /****************************************************/
       time_t  temp;

       temp= end + newptr->person.staytime*24*3600;
       struct tm *leavetime = localtime(&temp);
       leavetime->tm_year = leavetime->tm_year+1900;
       leavetime->tm_mon = leavetime->tm_mon+1;

       newptr->person.leavetime = leavetime->tm_year*10000 + leavetime->tm_mon*100 + leavetime->tm_mday;

       /***************************************************/

       searchptr1=hotel[newptr->person.roomnumber/100-1]->next;
       while(searchptr1&&searchptr1->room.roomnumber!=newptr->person.roomnumber){
            searchptr1=searchptr1->next;
       }
       if(searchptr1!=NULL){
           QMessageBox::information(this,tr(""),tr("入住成功"));
           ui->name_2->setText(QString::fromStdString(""));

           ui->identy_1->setText(QString::fromStdString(""));
           ui->telephone_2->setText(QString::fromStdString(""));
           ui->roomnumber->setText(QString::fromStdString(""));

           switch(newptr->person.roomnumber/100){
               case 1:
                   newptr->person.roomtype=single;
                   ChangeRoomlink(0,5,newptr);
                   break;
               case 2:
                   newptr->person.roomtype=standard;
                   ChangeRoomlink(1,6,newptr);
                   break;
               case 3:
                   newptr->person.roomtype=business;
                   ChangeRoomlink(2,7,newptr);
                   break;
               case 4:
                   newptr->person.roomtype=deluxe;
                   ChangeRoomlink(3,8,newptr);
               break;
               case 5:
                   newptr->person.roomtype=president;
                   ChangeRoomlink(4,9,newptr);
                   break;
               default:
                   break;
           }

           while(searchptr&&searchptr->person.roomnumber<newptr->person.roomnumber){
               preptr=searchptr;
               searchptr=searchptr->next;
           }
           if(searchptr==NULL){
               preptr->next=newptr;
           }
           else{
               newptr->next=searchptr;
               preptr->next=newptr;
           }

           clients->person.roomnumber++;//???? //
       }
       else{
           QMessageBox::information(this,tr(""),tr("该房间无法使用，请重新输入"));
           return 0;
       }
   }
   return 1;
}

void Dialogreserve::on_pushButton_yes_2_clicked()
{
     if(check_in()==0){
            return;
     }

    string name=ui->name_1->text().toStdString();

    Reserve_clientlink preptr=head_Reserve_clientlink;
    Reserve_clientlink newptr=preptr->next;
    while(newptr){
        if(newptr->reserve_client.name==name){
            newptr->reserve_client.room_num--;     //,........................
            if(newptr->reserve_client.room_num==0){
                if(preptr==head_Reserve_clientlink){
                    head_Reserve_clientlink->next=NULL;
                }
                else
                    preptr->next=newptr->next;
                close();
            }
            else{
                ui->roomnum_1->setText(QString::number(newptr->reserve_client.room_num));
            }
            break;
        }

        newptr=newptr->next;
    }
    int i=0;
    for(i=0;i<65;i++){
       if(reserve_tab[0][i].room_num!=0&&reserve_tab[0][i].name==name){
           reserve_tab[0][i].room_num--;
           if(reserve_tab[0][i].room_num==0)
              reserve_tab[0][i].name="";
           break;
       }
    }
    show_tabWidget_currentChanged();
}

void Dialogreserve::show_tabWidget_currentChanged()
{
    ui->tableWidget->clear();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList header;
    header<<"客户姓名"<<"电话号码"<<"预约时间"<<"预约入住时间"<<"预约天数"<<"房间类型"<<"房间间数";
    ui->tableWidget->setHorizontalHeaderLabels(header);

    int i=0,j=0;
    for(i=0;i<65;i++){
        if(reserve_tab[0][i].name!=""&&reserve_tab[0][i].room_num!=0){
            QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(reserve_tab[0][i].name));
            ui->tableWidget->setItem(j, 0, newItem);
            newItem = new QTableWidgetItem(QString::fromStdString(reserve_tab[0][i].telephone));
            ui->tableWidget->setItem(j, 1, newItem);
            newItem = new QTableWidgetItem(QString::number(reserve_tab[0][i].reserve_time));
            ui->tableWidget->setItem(j, 2, newItem);
            newItem = new QTableWidgetItem(QString::number(reserve_tab[0][i].timein));
            ui->tableWidget->setItem(j, 3, newItem);
            newItem = new QTableWidgetItem(QString::number(reserve_tab[0][i].reserve_stay));
            ui->tableWidget->setItem(j, 4, newItem);

            if (reserve_tab[0][i].roomtype == single)
               newItem = new QTableWidgetItem(QString::fromStdString("单人间"));
            else
                if (reserve_tab[0][i].roomtype == standard)
                    newItem = new QTableWidgetItem(QString::fromStdString("标准间"));
            else
                if (reserve_tab[0][i].roomtype == business)
                    newItem = new QTableWidgetItem(QString::fromStdString("商务间"));
            else
                if (reserve_tab[0][i].roomtype == deluxe)
                    newItem = new QTableWidgetItem(QString::fromStdString("豪华套房"));
            else
                if (reserve_tab[0][i].roomtype == president)
                    newItem = new QTableWidgetItem(QString::fromStdString("总统套房"));
            ui->tableWidget->setItem(j, 5, newItem);

            newItem = new QTableWidgetItem(QString::number(reserve_tab[0][i].room_num));
            ui->tableWidget->setItem(j, 6, newItem);
            j++;
        }
    }


}



void Dialogreserve::on_cancel_clicked()
{
    close();
}

void Dialogreserve::on_cancel_reserve_clicked()
{
    Reserve_clientlink newptr=head_Reserve_clientlink->next;
    if(ui->name_6->text().toStdString()==""||ui->telephone_6->text().toStdString()==""){
        QMessageBox::information(this,tr(""),tr("信息填写不全"));
        return;
    }
    while(newptr){
        if(newptr->reserve_client.name==ui->name_6->text().toStdString())
        {
            if(newptr->reserve_client.telephone==ui->telephone_6->text().toStdString()){
                 ui->reservestay_4->setText(QString::number(newptr->reserve_client.reserve_stay));
                 ui->reserve_time_4->setText(QString::number(newptr->reserve_client.reserve_time));
                 ui->roomnum_6->setText(QString::number(newptr->reserve_client.room_num));
                 ui->timein_3->setText(QString::number(newptr->reserve_client.timein));
                 return;
            }
        }
        newptr=newptr->next;
    }
    if(newptr==NULL){
        sign1=1;
        QMessageBox::information(this,tr(""),tr("没找到此客户预约信息,信息填写有误"));
    }

}

void Dialogreserve::on_yes_clicked()
{
    if(ui->name_6->text().toStdString()==""||ui->telephone_6->text().toStdString()==""){
        QMessageBox::information(this,tr(""),tr("信息填写不全"));
        return;
    }
    if(ui->reservestay_4->text().toStdString()=="" || ui->roomnum_6->text().toStdString()=="" || ui->name_6->text().toStdString()==""||ui->telephone_6->text().toStdString()==""){
        QMessageBox::information(this,tr(""),tr("请先确认预约信息"));
        return;
    }
    else{
        QMessageBox::information(this,tr(""),tr("取消预约成功"));
    }
    Reserve_clientlink preptr=head_Reserve_clientlink;
    Reserve_clientlink newptr=preptr->next;

    string name=ui->name_6->text().toStdString();
    string telephone=ui->telephone_6->text().toStdString();

    while(newptr){
        if(newptr->reserve_client.name==name&&newptr->reserve_client.telephone==telephone){
            if(preptr==head_Reserve_clientlink){
                head_Reserve_clientlink->next=newptr->next;
                break;
            }
            preptr->next=newptr->next;
            head_Reserve_clientlink->reserve_client.room_num--;

            break;
        }
        preptr=newptr;
        newptr=newptr->next;
    }

   int nowtime=timeNow->tm_year*10000+timeNow->tm_mon*100+timeNow->tm_mday;
   if(newptr&&time_difference(newptr->reserve_client.timein,nowtime)<7){
       int i=0,j=time_difference(newptr->reserve_client.timein,nowtime);
       for(i=0;i<65;i++){
           if(reserve_tab[j][i].name==name&&reserve_tab[j][i].telephone==telephone){
               reserve_tab[j][i].name="";
               reserve_tab[j][i].room_num=0;
               reserve_tab[j][i].telephone="";
               break;
           }
       }
   }
   Can_use_room_num_tab();

   show_tabWidget_currentChanged();

   close();
}

void Dialogreserve::on_esc_clicked()
{
    close();
}
