#include "hotel.h"
#include "dialogclientin.h"
#include "ui_dialogclientin.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <log_manage.h>
#include<dialogshow_history_message.h>

extern Roomlink hotel[LINKNUM];
extern Clientlink clients;
extern time_t end;
extern struct tm *timeNow; //????
extern int  can_occupy_room_num[30][5];//..............可入住房间数目
extern int backupflag;

extern Clientlink show_history;
extern int roomnumber_in[66];
extern int roomtype_in[6];
extern int general_staytime2;


Dialogclientin::Dialogclientin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogclientin)
{
    ui->setupUi(this);

    timeNow = localtime(&end);
    timeNow->tm_year = timeNow->tm_year+1900;
    timeNow->tm_mon = timeNow->tm_mon+1;
    int timein = timeNow->tm_year*10000 + timeNow->tm_mon*100 + timeNow->tm_mday;
    ui->textBrowser_stayin->setText(QString::number(timein, 10));//打印当天入住时间


    time_t  temp;

    temp= end + ui->spinBox_day->value()*24*3600;

    struct tm *leavetime = localtime(&temp);
    leavetime->tm_year = leavetime->tm_year+1900;
    leavetime->tm_mon = leavetime->tm_mon+1;

    int leave = leavetime->tm_year*10000 + leavetime->tm_mon*100 + leavetime->tm_mday;

    ui->textBrowser_leavetime->setText(QString::number(leave, 10));//打印默认离店时间

    //一旦居住天数改变，离店时间相应改变，并打印出来
    connect(ui->spinBox_day, SIGNAL(valueChanged(int)),this,SLOT(changeValue()));
}

Dialogclientin::~Dialogclientin()
{
    delete ui;
}


void Dialogclientin::on_pushButton_yes_clicked()
{
    Clientlink newptr=NULL,preptr=clients,searchptr=clients->next;
    Roomlink   searchptr1=NULL;

       preptr=clients;
       searchptr=clients->next;


       if (ui->lineEdit_name->text().isEmpty() || ui->lineEdit_id->text().isEmpty() ||
           ui->lineEdit_telephone->text().isEmpty() || ui->lineEdit_roomnumber->text().isEmpty())
       {
            QMessageBox::information(this,tr(""),tr("信息填写不全"));
       }
       else
       {
           newptr = new Clientnode;
           newptr->next=NULL;
           newptr->person.name = ui->lineEdit_name->text().toStdString();

           newptr->person.sex = ui->comboBox_sex->currentText().toStdString();
           newptr->person.identy = ui->lineEdit_id->text().toStdString();
           newptr->person.telephone = ui->lineEdit_telephone->text().toStdString();
           newptr->person.roomnumber = ui->lineEdit_roomnumber->text().toInt();
           if ((newptr->person.roomnumber >= 101 && newptr->person.roomnumber <= 113) ||
               (newptr->person.roomnumber >= 201 && newptr->person.roomnumber <= 213) ||
               (newptr->person.roomnumber >= 301 && newptr->person.roomnumber <= 313) ||
               (newptr->person.roomnumber >= 401 && newptr->person.roomnumber <= 413) ||
               (newptr->person.roomnumber >= 501 && newptr->person.roomnumber <= 513))
               ;//房间号非法输入判断
           else
           {
               QMessageBox::information(this,tr(""),tr("无此房间,请重新输入"));
               return;
           }

           timeNow = localtime(&end);
           timeNow->tm_year = timeNow->tm_year+1900;
           timeNow->tm_mon = timeNow->tm_mon+1;
           newptr->person.timein=timeNow->tm_year*10000 + timeNow->tm_mon*100 + timeNow->tm_mday;

          // qDebug() <<newptr->person.timein;

           newptr->person.staytime = ui->spinBox_day->value();

          // qDebug()<<newptr->person.staytime;
           /*******由当天时间，即入住时间 + 要住天数 ，自动获得离店时间****/
           time_t  temp;

           temp= end + ui->spinBox_day->value()*24*3600;
           struct tm *leavetime = localtime(&temp);
           leavetime->tm_year = leavetime->tm_year+1900;
           leavetime->tm_mon = leavetime->tm_mon+1;

           newptr->person.leavetime = leavetime->tm_year*10000 + leavetime->tm_mon*100 + leavetime->tm_mday;
           /******************************************************/

           searchptr1=hotel[newptr->person.roomnumber/100-1]->next;
           while(searchptr1&&searchptr1->room.roomnumber!=newptr->person.roomnumber){
                searchptr1=searchptr1->next;
           }
           if(searchptr1!=NULL){
               QMessageBox::information(this,tr(""),tr("入住成功"));

               switch(newptr->person.roomnumber/100){
                   case 1:
                       newptr->person.roomtype=single;
                       changeRoomlink(0,5,newptr);
                       break;
                   case 2:
                       newptr->person.roomtype=standard;
                       changeRoomlink(1,6,newptr);
                       break;
                   case 3:
                       newptr->person.roomtype=business;
                       changeRoomlink(2,7,newptr);
                       break;
                   case 4:
                       newptr->person.roomtype=deluxe;
                       changeRoomlink(3,8,newptr);
                   break;
                   case 5:
                       newptr->person.roomtype=president;
                       changeRoomlink(4,9,newptr);
                       break;
                   default:
                       break;
               }

       //	newptr->next=NULL;
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
               close();
           }
           else
               QMessageBox::information(this,tr(""),tr("该房间无法使用，请重新输入"));
       }
}

void  changeRoomlink(int a,int b,Clientlink newptr)
{
    Roomlink 	preptr=NULL,searchptr=NULL,roompreptr=hotel[b],roomsearchptr=hotel[b]->next;
    searchptr=hotel[a]->next;
    preptr=hotel[a];
   // roomlastptr=hotel[b];
    while(searchptr&&searchptr->room.roomnumber!=newptr->person.roomnumber){
        preptr=searchptr;
       searchptr=searchptr->next;
    }
    if(searchptr!=NULL){

       searchptr->room.clientname=newptr->person.name;//???
       searchptr->room.sign=Occupy;//???
       searchptr->room.day = 1;//刚入住住房时间记为一天
       searchptr->room.stayin = newptr->person.timein;
       searchptr->room.leavetime = newptr->person.leavetime;
       preptr->next=searchptr->next;

       log_manage(searchptr,check_in);      //更改日志；
       backupflag = 1;

       hotel[a]->room.roomnumber--;          //jhvjhvbh
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

}


void Dialogclientin::on_radioButton_business_clicked()
{
    /*****************************************/                    //判断是否有房间可住
    if(can_occupy_room_num[0][2]<1){
        QMessageBox::information(this,tr(""),tr("此类型房间已满"));
        return;
    }
    else{
        can_occupy_room_num[0][2]--;
    }
    /*************************************************************/
    string str, str1;
    stringstream ss;

    Roomlink t=hotel[2]->next;

    while(t)
    {
        ss<<t->room.roomnumber;
        ss>>str1;
        str = str + str1 + ' ';
        ss.clear();
        t=t->next;
    }
    ui->textBrowser->setText(QString::fromStdString(str));
}

void Dialogclientin::on_radioButton_single_clicked()
{
    /*****************************************/                    //判断是否有房间可住
    if(can_occupy_room_num[0][0]<1){
        QMessageBox::information(this,tr(""),tr("此类型房间已满"));
        return;
    }
    else{
        can_occupy_room_num[0][0]--;
    }
    /*************************************************************/
    string str, str1;
    stringstream ss;

    Roomlink t=hotel[0]->next;

    while(t)
    {
        ss<<t->room.roomnumber;
        ss>>str1;
        str = str + str1 + ' ';
        ss.clear();
        t=t->next;
    }
    ui->textBrowser->setText(QString::fromStdString(str));
}

void Dialogclientin::on_radioButton_standard_clicked()
{
    /*****************************************/                    //判断是否有房间可住
    if(can_occupy_room_num[0][1]<1){
        QMessageBox::information(this,tr(""),tr("此类型房间已满"));
        return;
    }
    else{
        can_occupy_room_num[0][1]--;
    }
    /*************************************************************/
    string str, str1;
    stringstream ss;

    Roomlink t=hotel[1]->next;

    while(t)
    {
        ss<<t->room.roomnumber;
        ss>>str1;
        str = str + str1 + ' ';
        ss.clear();
        t=t->next;
    }
    ui->textBrowser->setText(QString::fromStdString(str));
}

void Dialogclientin::on_radioButton_deluxe_clicked()
{
    /*****************************************/                    //判断是否有房间可住
    if(can_occupy_room_num[0][3]<1){
        QMessageBox::information(this,tr(""),tr("此类型房间已满"));
        return;
    }
    else{
        can_occupy_room_num[0][3]--;
    }
    /*************************************************************/
    string str, str1;
    stringstream ss;

    Roomlink t=hotel[3]->next;

    while(t)
    {
        ss<<t->room.roomnumber;
        ss>>str1;
        str = str + str1 + ' ';
        ss.clear();
        t=t->next;
    }
    ui->textBrowser->setText(QString::fromStdString(str));
}

void Dialogclientin::on_radioButton_president_clicked()
{
    /*****************************************/                    //判断是否有房间可住
    if(can_occupy_room_num[0][4]<1){
        QMessageBox::information(this,tr(""),tr("此类型房间已满"));
        return;
    }
    else{
        can_occupy_room_num[0][4]--;
    }
    /*************************************************************/
    string str, str1;
    stringstream ss;

    Roomlink t=hotel[4]->next;

    while(t)
    {
        ss<<t->room.roomnumber;
        ss>>str1;
        str = str + str1 + ' ';
        ss.clear();
        t=t->next;
    }
    ui->textBrowser->setText(QString::fromStdString(str));
}

void Dialogclientin::on_pushButton_no_clicked()
{
    close();
}

void Dialogclientin::changeValue()
{
    time_t  temp;

    temp= end + ui->spinBox_day->value()*24*3600;
    struct tm *leavetime = localtime(&temp);
    leavetime->tm_year = leavetime->tm_year+1900;
    leavetime->tm_mon = leavetime->tm_mon+1;

    int leave = leavetime->tm_year*10000 + leavetime->tm_mon*100 + leavetime->tm_mday;

    ui->textBrowser_leavetime->setText(QString::number(leave, 10));
}

void Dialogclientin::on_history_inqury_clicked()
{
    if(ui->lineEdit_name->text().toStdString()==""){
        QMessageBox::information(this,tr(""),tr("信息填写不全"));
        return;
    }
    string  clientname="",clientidenty="",str;
    string name="",//??
           sex="",//??
           identy="",//???
           telephone;//??
    int roomnumber=0,//????

        timein=0,//????
        leavetime=0;
    string roomtype;//????
    float staytime=0;
    general_staytime2=0;

    int i=0;
    for(i=0;i<66;i++){
        roomnumber_in[i]=0;
    }

    for(i=0;i<6;i++){
        roomtype_in[i]=0;
    }
    clientname=ui->lineEdit_name->text().toStdString();
    clientidenty=ui->lineEdit_id->text().toStdString();
    ifstream in("history_client.txt");

    if (!in.is_open())
    {
        cout << "Error opening log_manage.txt" << endl;
        exit(1);
    }
    else{
        getline(in,str);
        while(!in.peek()!=EOF){
            roomnumber=0;
            in>>name>>sex>>identy>>telephone>>roomnumber>>timein>>staytime>>leavetime>>roomtype;
            if(roomnumber==0)
                break;
            if(clientname==name){
                if(show_history){
                show_history->person.name=name;
                show_history->person.identy=identy;
                show_history->person.sex=sex;
                show_history->person.telephone=telephone;
                }
              //  show_history->person.roomnumber=roomnumber;
              //  roomnumber1=roomnumber;  //建立数组,按房间号对应数组下标，65维数组
                roomnumber_in[roomnumber%100+13*(roomnumber/100-1)]++;
                roomnumber_in[0]++;
           //     staytime,staytime1;     //建立数组,按住的天数对应数组下标，30维数组
                general_staytime2+=staytime;

                if(show_history){
                show_history->person.timein=timein;
                show_history->person.leavetime=leavetime;
                }
          //      roomtype,roomtype1;  //建立数组,按房间类型对应数组下标，5维数组
                if (roomtype == "单人间")
                    roomtype_in[1]++;
                else
                    if (roomtype == "标准间")
                        roomtype_in[2]++;
                else
                    if (roomtype == "商务间")
                        roomtype_in[3]++;
                else
                    if (roomtype == "豪华套房")
                        roomtype_in[4]++;
                else
                    if (roomtype == "总统套房")
                        roomtype_in[5]++;
                roomtype_in[0]++;
            }

        }
        in.close();
        if(roomtype_in[0]==0){
            QMessageBox::information(this,tr(""),tr("此客户为新客户"));
            return;
        }
        else{
            ui->lineEdit_telephone->setText(QString::fromStdString(show_history->person.telephone));
            ui->lineEdit_id->setText(QString::fromStdString(show_history->person.identy));
            ui->label_sex->setText(QString::fromStdString(show_history->person.sex));
            Dialogshow_history_message a;
            a.exec();

        }

    }
}
