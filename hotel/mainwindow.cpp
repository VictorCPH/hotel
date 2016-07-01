#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogstatistics.h"
#include "dialogtimeline.h"

#include <QCloseEvent>
#include "QTimer"
#include <QDateTime>
#include <QDebug>
#include "hotel.h"

extern struct tm *timeNow;
extern Roomlink hotel[LINKNUM];
extern Hotel myhotel;
extern Clientlink clients;

extern Reserve_clientlink head_Reserve_clientlink;
extern Reserve_client  reserve_tab[7][65];
extern int  can_occupy_room_num[30][5];//5种类型房
extern void Can_use_room_num_tab();
extern int roomnumberforshow;
extern time_t end;

extern int PRICESINGLE;
extern int PRICESTANDAR;
extern int PRICEBUSINESS;
extern int PRICEDELUXE;
extern int PRICEPRESIDENT;
extern int loadsuccess;

extern int ratio;
extern int general_income;
extern float occupancy_rate;
extern int posi;
int first = 0;
int backupflag = 0;
int manageflag = 0;
extern Income income[66];
extern Dayincome dayincome[13][32];

Clientlink head_remind_clients=NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Clientlink p;
    p=new Clientnode;
    p->next=NULL;
    head_remind_clients=p;




    QTimer *timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);

    connect(this,SIGNAL(Nowbackup()),&backup,SLOT(Nowstart()));

    connect(this,SIGNAL(timerstop()),timer,SLOT(stop()));
    connect(this,SIGNAL(timerrestart()),timer,SLOT(start()));
    show_tabWidget_currentChanged();
    change_roomsign();
    color();

    ui->general_income->setText(QString::number(general_income));
    occupancy_rate=float(clients->person.roomnumber)/float(65);
    occupancy_rate=occupancy_rate*100;
    ui->occupancyrate->setText(QString::number(occupancy_rate,'f',1)+"%");

    shownumeber();

}

void MainWindow::closeEvent(QCloseEvent *event)        //新加函数
{
    backup_general_income_occupancy_rate();
    backupincome();
    backupclient();
    backuproom();
    backupsoftwaretime();
    backupprice();
    backup_reserve_client();
    emit Nowbackup();
    backup.exec();
    event->accept();

}


void MainWindow::backupincome(){
    ofstream fout("yearincome.txt");
    if (! fout.is_open())
    {
        qDebug()<< "Error opening roomprize.txt";
        exit (1);
    }
    fout<<"		房价收入	额外消费收入	总收入"<<endl;
    //fout.width(16);
    fout<<setiosflags(ios::left)<<"年总收入"<<"\t";
    fout.width(16);
    fout<<setiosflags(ios::left)<<dayincome[0][0].roomincome;
    fout.width(16);
    fout<<setiosflags(ios::left)<<dayincome[0][0].consumption;
    fout.width(16);
    fout<<setiosflags(ios::left)<<dayincome[0][0].generalincome<<endl;
   // fout.width(16);
    fout<<setiosflags(ios::left)<<"每天收入"<<"\t";
    int i=1,j=1;
    for(i=1;i<13;i++){
        for(j=1;j<32;j++){
            if(j!=1||i!=1){
                fout<<"\t\t";

            }
            fout.width(16);
            fout<<setiosflags(ios::left)<<dayincome[i][j].roomincome;
            fout.width(16);
            fout<<setiosflags(ios::left)<<dayincome[i][j].consumption;
            fout.width(16);
            fout<<setiosflags(ios::left)<<dayincome[i][j].generalincome<<endl;
         //   dayincome[i][0].consumption+=

        }
    }
    ofstream fout1("dayincome.txt");
    if (! fout1.is_open())
    {
        qDebug()<< "Error opening roomprize.txt";
        exit (1);
    }
    fout1<<"房间号		房价收入	额外消费收入	总收入"<<endl;
   // fout1.width(18);
    fout1<<setiosflags(ios::left)<<"当天总收入"<<"\t"<<"  ";

    fout1.width(16);
    fout1<<setiosflags(ios::left)<<income[0].roomincome;
    fout1.width(16);
    fout1<<setiosflags(ios::left)<<income[0].consumption;

    fout1.width(16);
    fout1<<setiosflags(ios::left)<<income[0].consumption+income[0].roomincome<<endl;
    int t=1;
    for(t=1;t<66;t++){
        fout1.width(18);
        fout1<<setiosflags(ios::left)<<income[t].roomnumber;
        fout1.width(16);
        fout1<<setiosflags(ios::left)<<income[t].roomincome;
        fout1.width(16);
        fout1<<setiosflags(ios::left)<<income[t].consumption;

        fout1.width(16);
        fout1<<setiosflags(ios::left)<<income[t].consumption+income[t].roomincome<<endl;

    }

}

void MainWindow::backupprice()
{
     ofstream out("roomprize.txt");
     if (! out.is_open())
     {
         cout << "Error opening roomprize.txt";
         exit (1);
     }

     out<<"单人间 "<<PRICESINGLE<<endl;
     out<<"标准间 "<<PRICESTANDAR<<endl;
     out<<"商务间 "<<PRICEBUSINESS<<endl;
     out<<"豪华套房 "<<PRICEDELUXE<<endl;
     out<<"总统套房 "<<PRICEPRESIDENT<<endl;

     out.close();
}

void MainWindow::backupsoftwaretime()
{
    ofstream out("softwaretime.txt");
    if (! out.is_open())
    {
        cout << "Error opening softwaretime.txt";
        exit (1);
    }

    out << timeNow->tm_year<<' '<<timeNow->tm_mon<<' '<<timeNow->tm_mday<<' '
        << timeNow->tm_hour<<' '<<timeNow->tm_min<<' '<<timeNow->tm_sec;

    out.close();
}

void MainWindow::backup_general_income_occupancy_rate(){
    ofstream out("general_income_occupancy_rate.txt");
    if (! out.is_open())
    {
        qDebug()<< "Error opening roomprize.txt";
        exit (1);
    }
    out<<"总收入"<<"\t"<<general_income<<endl;
    out<<"入住率"<<"\t"<<occupancy_rate<<endl;

    out.close();
}

void MainWindow::backup_reserve_client(){
    ofstream out("reserve.txt");

    if (! out.is_open())
    {
        qDebug()<< "Error opening reserve.txt";
        exit (1);
    }
    out<<"客户姓名	电话号码	预约时间	预约入住时间	预约天数	房间类型	房间间数"<<endl;

    Reserve_clientlink ptr=head_Reserve_clientlink->next;
    while(ptr){
        out<<ptr->reserve_client.name<<'\t'<<ptr->reserve_client.telephone<<'\t';
        out<<ptr->reserve_client.reserve_time<<'\t'<<ptr->reserve_client.timein;
        out<<'\t'<<ptr->reserve_client.reserve_stay<<'\t';
        if (ptr->reserve_client.roomtype == single)
           out<<"单人间";
        else
            if (ptr->reserve_client.roomtype == standard)
                out<<"标准间";
        else
            if (ptr->reserve_client.roomtype == business)
                out<<"商务间";
        else
            if (ptr->reserve_client.roomtype == deluxe)
                out<<"豪华套房";
        else
            if (ptr->reserve_client.roomtype == president)
                out<<"总统套房";
        out<<'\t'<<ptr->reserve_client.room_num<<endl;
        ptr=ptr->next;
    }

}
void MainWindow::backupclient()                  //新加的备份
{
    Clientlink p;
    ofstream out("client.txt");
    if (! out.is_open())
    {
        cout << "Error opening client.txt";
        exit (1);
    }

    out<<"客户名字\t性别\t身份证\t电话\t入住房号\t入住时间\t住房时间\t计划离店时间\t房间类型\t\n";

    for (p=clients->next; p!=NULL; p=p->next)
    {
        out<<p->person.name<<'\t'<<p->person.sex <<'\t'<<p->person.identy <<'\t'<<p->person.telephone
            <<'\t'<<p->person.roomnumber<<'\t'<<p->person.timein<<'\t'<<p->person.staytime<<'\t'<<p->person.leavetime;

        if (p->person.roomtype == single)
            out<<"单人间\t\n";
        else
            if (p->person.roomtype == standard)
                out<<"标准间\t\n" ;
        else
            if (p->person.roomtype == business)
                out<<"商务间\t\n" ;
        else
            if (p->person.roomtype == deluxe)
                out<<"豪华套房\t\n" ;
        else
            if (p->person.roomtype == president)
                out<<"总统套房\t\n" ;
    }
    out.close();
}

void MainWindow::backuproom()         //新加的备份
{
    int emptylist, occupylist;
    Roomlink p1, p2;

    ofstream out("room.txt");
    if (! out.is_open())
    {
        cout << "Error opening room.txt";
        exit (1);
    }

    out<<"房间号\t客户名\t房间类型\t房间价格\t房间状态\t入住时间\t已住天数\t计划离店时间\t额外消费\t\n";
    for (emptylist=0; emptylist<=4; emptylist++)
    {
        occupylist = emptylist+5;

        p1 = hotel[emptylist]->next;
        p2 = hotel[occupylist]->next;

        while (p1&&p2)
        {
            if (p1->room.roomnumber < p2->room.roomnumber)
            {
                out<<p1->room.roomnumber<<'\t'<<p1->room.clientname<<'\t';

                if (p1->room.roomtype == single)
                    out<<"单人间\t";
                else
                    if (p1->room.roomtype == standard)
                        out<<"标准间\t" ;
                else
                    if (p1->room.roomtype == business)
                        out<<"商务间\t" ;
                else
                    if (p1->room.roomtype == deluxe)
                        out<<"豪华套房\t" ;
                else
                    if (p1->room.roomtype == president)
                        out<<"总统套房\t" ;

                out<<p1->room.roomprice<<'\t';

                switch(p1->room.sign)
                {
                    case Empty:
                        out<<"空闲\t";//空闲，预定，占据，维修
                        break;
                    case Reserve:
                        out<<"预定\t";
                        break;
                    case Occupy:
                        out<<"占据\t";
                        break;
                    case Repair:
                        out<<"维修\t";
                        break;
                    default:
                        break;
                }
                out<<p1->room.stayin<<'\t'<<p1->room.day<<'\t'<<p1->room.leavetime<<'\t';
                out<<p1->room.consumption<<'\t'<<'\n';
                p1 = p1->next;
            }
            else
            {
                out<<p2->room.roomnumber<<'\t'<<p2->room.clientname<<'\t';

                if (p2->room.roomtype == single)
                    out<<"单人间\t";
                else
                    if (p2->room.roomtype == standard)
                        out<<"标准间\t" ;
                else
                    if (p2->room.roomtype == business)
                        out<<"商务间\t" ;
                else
                    if (p2->room.roomtype == deluxe)
                        out<<"豪华套房\t" ;
                else
                    if (p2->room.roomtype == president)
                        out<<"总统套房\t" ;

                out<<p2->room.roomprice<<'\t';

                switch(p2->room.sign)
                {
                    case Empty:
                        out<<"空闲\t";//空闲，预定，占据，维修
                        break;
                    case Reserve:
                        out<<"预定\t";
                        break;
                    case Occupy:
                        out<<"占据\t";
                        break;
                    case Repair:
                        out<<"维修\t";
                        break;
                    default:
                        break;
                }
                out<<p2->room.stayin<<'\t'<<p2->room.day<<'\t'<<p2->room.leavetime<<'\t';
                out<<p2->room.consumption<<'\t'<<'\n';
                p2 = p2->next;
            }
        }

        if (p1 == NULL)
            p1 = p2;
        for (; p1!=NULL; p1=p1->next)
        {
            out<<p1->room.roomnumber<<'\t'<<p1->room.clientname<<'\t';

            if (p1->room.roomtype == single)
                out<<"单人间\t";
            else
                if (p1->room.roomtype == standard)
                    out<<"标准间\t" ;
            else
                if (p1->room.roomtype == business)
                    out<<"商务间\t" ;
            else
                if (p1->room.roomtype == deluxe)
                    out<<"豪华套房\t" ;
            else
                if (p1->room.roomtype == president)
                    out<<"总统套房\t" ;

            out<<p1->room.roomprice<<'\t';

            switch(p1->room.sign)
            {
                case Empty:
                    out<<"空闲\t";//空闲，预定，占据，维修
                    break;
                case Reserve:
                    out<<"预定\t";
                    break;
                case Occupy:
                    out<<"占据\t";
                    break;
                case Repair:
                    out<<"维修\t";
                    break;
                default:
                    break;
            }
            out<<p1->room.stayin<<'\t'<<p1->room.day<<'\t'<<p1->room.leavetime<<'\t';
            out<<p1->room.consumption<<'\t'<<'\n';
        }
    }
    out.close();
}
void MainWindow::roomnumber_match(Roomlink room){  //Empty, Reserve, Occupy,background-color: rgb(0, 135, 0);
    switch(room->room.roomnumber){
    case 101:
        if(room->room.sign == Reserve)       ui->pushButton_101->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign == Occupy)   ui->pushButton_101->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign == Repair)   ui->pushButton_101->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign == Empty)    ui->pushButton_101->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 102:
         if(room->room.sign==Reserve)       ui->pushButton_102->setStyleSheet("background-color: rgb(0, 135, 0);");
         else if(room->room.sign==Occupy)   ui->pushButton_102->setStyleSheet("background-color: rgb(255, 85, 0);");
         else if(room->room.sign==Repair)   ui->pushButton_102->setStyleSheet("background-color: rgb(0, 255, 255);");
         else if(room->room.sign==Empty)    ui->pushButton_102->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 103:
        if(room->room.sign==Reserve)       ui->pushButton_103->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_103->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_103->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_103->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 104:
        if(room->room.sign==Reserve)       ui->pushButton_104->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_104->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_104->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_104->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 105:
        if(room->room.sign==Reserve)       ui->pushButton_105->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_105->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_105->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_105->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 106:
        if(room->room.sign==Reserve)        ui->pushButton_106->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_106->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_106->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_106->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 107:
        if(room->room.sign==Reserve)        ui->pushButton_107->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_107->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_107->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_107->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 108:
        if(room->room.sign==Reserve)        ui->pushButton_108->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_108->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_108->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_108->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 109:
        if(room->room.sign==Reserve)        ui->pushButton_109->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_109->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_109->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_109->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 110:
        if(room->room.sign==Reserve)        ui->pushButton_110->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_110->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_110->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_110->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 111:
        if(room->room.sign==Reserve)        ui->pushButton_111->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_111->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_111->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_111->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 112:
        if(room->room.sign==Reserve)        ui->pushButton_112->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_112->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_112->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_112->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 113:
        if(room->room.sign==Reserve)        ui->pushButton_113->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_113->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_113->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_113->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 201:
        if(room->room.sign==Reserve)        ui->pushButton_201->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_201->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_201->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_201->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 202:
        if(room->room.sign==Reserve)        ui->pushButton_202->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_202->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_202->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_202->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 203:
        if(room->room.sign==Reserve)        ui->pushButton_203->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_203->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_203->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_203->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 204:
        if(room->room.sign==Reserve)        ui->pushButton_204->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_204->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_204->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_204->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 205:
        if(room->room.sign==Reserve)        ui->pushButton_205->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_205->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_205->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_205->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 206:
        if(room->room.sign==Reserve)        ui->pushButton_206->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_206->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_206->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_206->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 207:
        if(room->room.sign==Reserve)        ui->pushButton_207->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_207->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_207->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_207->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 208:
        if(room->room.sign==Reserve)        ui->pushButton_208->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_208->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_208->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_208->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 209:
        if(room->room.sign==Reserve)        ui->pushButton_209->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_209->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_209->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_209->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 210:
        if(room->room.sign==Reserve)        ui->pushButton_210->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_210->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_210->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_210->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 211:
        if(room->room.sign==Reserve)        ui->pushButton_211->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_211->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_211->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_211->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 212:
        if(room->room.sign==Reserve)        ui->pushButton_212->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_212->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_212->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_212->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 213:
        if(room->room.sign==Reserve)        ui->pushButton_213->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_213->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_213->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_213->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 301:
        if(room->room.sign==Reserve)        ui->pushButton_301->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_301->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_301->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_301->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 302:
        if(room->room.sign==Reserve)        ui->pushButton_302->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_302->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_302->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_302->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 303:
        if(room->room.sign==Reserve)        ui->pushButton_303->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_303->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_303->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_303->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 304:
        if(room->room.sign==Reserve)        ui->pushButton_304->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_304->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_304->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_304->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 305:
        if(room->room.sign==Reserve)        ui->pushButton_305->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_305->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_305->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_305->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 306:
        if(room->room.sign==Reserve)        ui->pushButton_306->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_306->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_306->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_306->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 307:
        if(room->room.sign==Reserve)        ui->pushButton_307->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_307->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_307->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_307->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 308:
        if(room->room.sign==Reserve)        ui->pushButton_308->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_308->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_308->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_308->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 309:
        if(room->room.sign==Reserve)        ui->pushButton_309->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_309->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_309->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_309->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 310:
        if(room->room.sign==Reserve)        ui->pushButton_310->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_310->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_310->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_310->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 311:
        if(room->room.sign==Reserve)        ui->pushButton_311->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_311->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_311->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_311->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 312:
        if(room->room.sign==Reserve)        ui->pushButton_312->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_312->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_312->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_312->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 313:
        if(room->room.sign==Reserve)        ui->pushButton_313->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_313->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_313->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_313->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 401:
        if(room->room.sign==Reserve)        ui->pushButton_401->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_401->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_401->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_401->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 402:
        if(room->room.sign==Reserve)        ui->pushButton_402->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_402->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_402->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_402->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 403:
        if(room->room.sign==Reserve)        ui->pushButton_403->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_403->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_403->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_403->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 404:
        if(room->room.sign==Reserve)        ui->pushButton_404->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_404->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_404->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_404->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 405:
        if(room->room.sign==Reserve)        ui->pushButton_405->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_405->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_405->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_405->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 406:
        if(room->room.sign==Reserve)        ui->pushButton_406->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_406->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_406->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_406->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 407:
        if(room->room.sign==Reserve)        ui->pushButton_407->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_407->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_407->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_407->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 408:
        if(room->room.sign==Reserve)        ui->pushButton_408->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_408->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_408->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_408->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 409:
        if(room->room.sign==Reserve)        ui->pushButton_409->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_409->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_409->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_409->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 410:
        if(room->room.sign==Reserve)        ui->pushButton_410->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_410->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_410->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_410->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 411:
        if(room->room.sign==Reserve)        ui->pushButton_411->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_411->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_411->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_411->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 412:
        if(room->room.sign==Reserve)        ui->pushButton_412->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_412->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_412->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_412->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 413:
        if(room->room.sign==Reserve)        ui->pushButton_413->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_413->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_413->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_413->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 501:
        if(room->room.sign==Reserve)        ui->pushButton_501->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_501->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_501->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_501->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 502:
        if(room->room.sign==Reserve)        ui->pushButton_502->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_502->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_502->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_502->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 503:
        if(room->room.sign==Reserve)        ui->pushButton_503->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_503->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_503->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_503->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 504:
        if(room->room.sign==Reserve)        ui->pushButton_504->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_504->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_504->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_504->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 505:
        if(room->room.sign==Reserve)        ui->pushButton_505->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_505->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_505->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_505->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 506:
        if(room->room.sign==Reserve)        ui->pushButton_506->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_506->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_506->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_506->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 507:
        if(room->room.sign==Reserve)        ui->pushButton_507->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_507->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_507->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_507->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 508:
        if(room->room.sign==Reserve)        ui->pushButton_508->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_508->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_508->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_508->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 509:
        if(room->room.sign==Reserve)        ui->pushButton_509->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_509->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_509->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_509->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 510:
        if(room->room.sign==Reserve)        ui->pushButton_510->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_510->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_510->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_510->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 511:
        if(room->room.sign==Reserve)        ui->pushButton_511->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_511->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_511->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_511->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 512:
        if(room->room.sign==Reserve)       ui->pushButton_512->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_512->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_512->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_512->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    case 513:
        if(room->room.sign==Reserve)        ui->pushButton_513->setStyleSheet("background-color: rgb(0, 135, 0);");
        else if(room->room.sign==Occupy)   ui->pushButton_513->setStyleSheet("background-color: rgb(255, 85, 0);");
        else if(room->room.sign==Repair)   ui->pushButton_513->setStyleSheet("background-color: rgb(0, 255, 255);");
        else if(room->room.sign==Empty)    ui->pushButton_513->setStyleSheet("background-color: rgb(162, 162, 162);");
        break;
    }
}

void MainWindow::color()
{

     Roomlink ptr;
     int i=0;
   //  ui->pushButton_101->setStyleSheet("background-color: rgb(0, 135, 0);");
     for(i=0;i<10;i++){
         ptr=hotel[i]->next;

         while(ptr){
             MainWindow::roomnumber_match(ptr);
             ptr=ptr->next;
         }
     }
}

void MainWindow::shownumeber()
{

    myhotel.Occupy = (hotel[5]->room.roomnumber - hotel[5]->room.roomprice)+
                     (hotel[6]->room.roomnumber - hotel[6]->room.roomprice)+
                     (hotel[7]->room.roomnumber - hotel[7]->room.roomprice)+
                     (hotel[8]->room.roomnumber - hotel[8]->room.roomprice)+
                     (hotel[9]->room.roomnumber - hotel[9]->room.roomprice);
    myhotel.Repair = hotel[5]->room.roomprice + hotel[6]->room.roomprice + hotel[7]->room.roomprice + hotel[8]->room.roomprice + hotel[9]->room.roomprice;

    myhotel.Reserve =hotel[0]->room.roomprice + hotel[1]->room.roomprice+
                     hotel[2]->room.roomprice+ hotel[3]->room.roomprice+
                      hotel[4]->room.roomprice;
        //预约房总数还未完善，暂时用总数减其他得到，在此标记一下;
    myhotel.Empty = hotel[0]->room.roomnumber + hotel[1]->room.roomnumber + hotel[2]->room.roomnumber + hotel[3]->room.roomnumber + hotel[4]->room.roomnumber-myhotel.Reserve;

    ui->label_free->setText(QString::number(myhotel.Empty, 10));
    ui->label_occupy->setText(QString::number(myhotel.Occupy, 10));
    ui->label_repair->setText(QString::number(myhotel.Repair, 10));
    ui->label_reserve->setText(QString::number(myhotel.Reserve, 10));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_inquiry_clicked()
{
    emit timerstop();
    Dialoginquiry b;
    b.exec();
    emit timerrestart();
}

void MainWindow::cancel_reserve(string name,string telephone){
    Reserve_clientlink preptr=head_Reserve_clientlink;
    Reserve_clientlink newptr=preptr->next;

    while(newptr){
        if(newptr->reserve_client.name==name&&newptr->reserve_client.telephone==telephone){
            if(preptr==head_Reserve_clientlink){
                head_Reserve_clientlink->next=newptr->next;
                break;
            }
            preptr->next=newptr->next;
            head_Reserve_clientlink->reserve_client.room_num--;

         //   qDebug()<<head_Reserve_clientlink->reserve_client.room_num;
            break;
        }
        preptr=newptr;
        newptr=newptr->next;
    }

       int i=0;
       for(i=0;i<65;i++){
           if(reserve_tab[0][i].name==name&&reserve_tab[0][i].telephone==telephone){
               reserve_tab[0][i].name="";
               reserve_tab[0][i].room_num=0;
               reserve_tab[0][i].telephone="";
               break;
           }
       }

   Can_use_room_num_tab();
}

void MainWindow::creat_remind()
{
    Clientlink newptr=clients->next;
    Clientlink p=NULL;

    head_remind_clients->person.roomnumber=0;
    Clientlink lastptr=head_remind_clients;

    timeNow = localtime(&end);
    timeNow->tm_year = timeNow->tm_year+1900;
    timeNow->tm_mon = timeNow->tm_mon+1;
    int nowtime=timeNow->tm_year*10000+timeNow->tm_mon*100+timeNow->tm_mday;
  //  qDebug()<<nowtime;
    while(newptr){
        if(newptr->person.leavetime==nowtime){
            p=new Clientnode;
            p->next=NULL;
            p->person=newptr->person;
            while(lastptr&&lastptr->next){
                lastptr=lastptr->next;
            }
            if(lastptr==head_remind_clients){
                head_remind_clients->next=p;
            }
            else{
                if(lastptr)
                    lastptr->next=p;
            }

            head_remind_clients->person.roomnumber++;
        }
        newptr=newptr->next;
    }
}

void MainWindow::timerUpdate()
{
   int prevhour = timeNow->tm_hour; //更新前的小时数
   int prevyear=timeNow->tm_year;
   end = end + 1*ratio*60;//1s推进ratio分钟
   timeNow = localtime(&end);
   timeNow->tm_year = timeNow->tm_year+1900;
   timeNow->tm_mon = timeNow->tm_mon+1;
   if (timeNow != NULL)
        ui->label_time->setText(QString::number(timeNow->tm_year)+"/"+QString::number(timeNow->tm_mon)+"/"+QString::number(timeNow->tm_mday)
                            +" "+QString::number(timeNow->tm_hour)+":"+QString::number(timeNow->tm_min)+":"+QString::number(timeNow->tm_sec)); //显示时间格式为例如 12:24:33 2008/8/28

   if ((prevhour < 12 && timeNow->tm_hour >= 12) ||
       (prevhour < 18 && timeNow->tm_hour >= 18))
   {

       for (int i=5; i<=9; i++)
       {
           Roomlink searchptr = hotel[i]->next;
           while(searchptr)
           {
               if (searchptr->room.sign == Occupy && searchptr->room.stayin != timeNow->tm_year*10000 + timeNow->tm_mon*100 + timeNow->tm_mday)
                   searchptr->room.day = searchptr->room.day + 0.5;

               searchptr = searchptr->next;
           }
       }
   }



   if(prevhour < 22 && timeNow->tm_hour >= 22){       //取消已经过期的预约信息

       int nowtime=timeNow->tm_year*10000+timeNow->tm_mon*100+timeNow->tm_mday;
       Reserve_clientlink preptr=head_Reserve_clientlink;
       Reserve_clientlink newptr=preptr->next;
      // string name="",telephone="";
       while(newptr){
           if(newptr->reserve_client.reserve_time==nowtime){
               cancel_reserve(newptr->reserve_client.name,newptr->reserve_client.telephone);
               QMessageBox::information(this,tr(""),tr("客户预约信息过期，已自动取消预约"));
           }
           newptr=newptr->next;
       }
       change_roomsign();
       color();
       show_tabWidget_currentChanged();

   }


   if(prevhour < 10 && timeNow->tm_hour >= 10){        //提醒客户离店
       creat_remind();
       if(head_remind_clients->person.roomnumber!=0){
               QMessageBox::information(this,tr(""),tr("提醒客户离店"));
               show_remind_tab();                                 //入住和离开都要再刷新表格和创建提醒链表
       }
   }
   /*****************************************************/
   if(prevhour < 1 && timeNow->tm_hour >= 1){

       int i=0;
       for(i=0;i<66;i++){
           income[i].consumption=0;
           income[i].roomincome=0;
           income[i].roomnumber=0;
       }
   }
   if(prevyear!=timeNow->tm_year){
       int j=0,i=0;
       for(i=0;i<13;i++){
           for(j=0;j<31;j++){
               dayincome[i][j].consumption=0;
               dayincome[i][j].roomincome=0;
               dayincome[i][j].generalincome=0;
           }
       }

   }





   /*********************************************/
}



void MainWindow::on_pushButton_clientin_clicked()
{
    emit timerstop();

    Dialogclientin a;
    a.exec();
    if(backupflag == 1)
            {
                receive_pushbutton_log();
                backupflag = 0;
            }
    emit timerrestart();
    change_roomsign();
    color();


    occupancy_rate=float(clients->person.roomnumber)/float(65);
    occupancy_rate=occupancy_rate*100;
    ui->occupancyrate->setText(QString::number(occupancy_rate, 'f', 1)+"%");

    creat_remind();
    show_remind_tab();
    shownumeber();
}

void MainWindow::on_pushButton_101_clicked()
{
    emit timerstop();
    roomnumberforshow = 101;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();

}

void MainWindow::on_pushButton_102_clicked()
{
    emit timerstop();
    roomnumberforshow = 102;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();

}

void MainWindow::on_pushButton_103_clicked()
{
    emit timerstop();
    roomnumberforshow = 103;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}


void MainWindow::on_pushButton_104_clicked()
{
    emit timerstop();
    roomnumberforshow = 104;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}


void MainWindow::on_pushButton_105_clicked()
{
    emit timerstop();
    roomnumberforshow = 105;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_106_clicked()
{
    emit timerstop();
    roomnumberforshow = 106;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_107_clicked()
{
    emit timerstop();
    roomnumberforshow = 107;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_108_clicked()
{
    emit timerstop();
    roomnumberforshow = 108;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_109_clicked()
{
    emit timerstop();
    roomnumberforshow = 109;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_110_clicked()
{
    emit timerstop();
    roomnumberforshow = 110;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_111_clicked()
{
    emit timerstop();
    roomnumberforshow = 111;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_112_clicked()
{
    emit timerstop();
    roomnumberforshow = 112;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_113_clicked()
{
    emit timerstop();
    roomnumberforshow = 113;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_201_clicked()
{
    emit timerstop();
    roomnumberforshow = 201;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_202_clicked()
{
    emit timerstop();
    roomnumberforshow = 202;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_203_clicked()
{
    emit timerstop();
    roomnumberforshow = 203;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_204_clicked()
{
    emit timerstop();
    roomnumberforshow = 204;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_205_clicked()
{
    emit timerstop();
    roomnumberforshow = 205;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_206_clicked()
{
    emit timerstop();
    roomnumberforshow = 206;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_207_clicked()
{
    emit timerstop();
    roomnumberforshow = 207;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_208_clicked()
{
    emit timerstop();
    roomnumberforshow = 208;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_209_clicked()
{
    emit timerstop();
    roomnumberforshow = 209;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_210_clicked()
{
    emit timerstop();
    roomnumberforshow = 210;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_211_clicked()
{
    emit timerstop();
    roomnumberforshow = 211;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_212_clicked()
{
    emit timerstop();
    roomnumberforshow = 212;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_213_clicked()
{
    emit timerstop();
    roomnumberforshow = 213;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_301_clicked()
{
    emit timerstop();
    roomnumberforshow = 301;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_302_clicked()
{
    emit timerstop();
    roomnumberforshow = 302;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_303_clicked()
{
    emit timerstop();
    roomnumberforshow = 303;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_304_clicked()
{
    emit timerstop();
    roomnumberforshow = 304;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_305_clicked()
{
    emit timerstop();
    roomnumberforshow = 305;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_306_clicked()
{
    emit timerstop();
    roomnumberforshow = 306;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_307_clicked()
{
    emit timerstop();
    roomnumberforshow = 307;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_308_clicked()
{
    emit timerstop();
    roomnumberforshow = 308;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_309_clicked()
{
    emit timerstop();
    roomnumberforshow = 309;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_310_clicked()
{
    emit timerstop();
    roomnumberforshow = 310;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_311_clicked()
{
    emit timerstop();
    roomnumberforshow = 311;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_312_clicked()
{
    emit timerstop();
    roomnumberforshow = 312;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_313_clicked()
{
    emit timerstop();
    roomnumberforshow = 313;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_401_clicked()
{
    emit timerstop();
    roomnumberforshow = 401;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_402_clicked()
{
    emit timerstop();
    roomnumberforshow = 402;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_403_clicked()
{
    emit timerstop();
    roomnumberforshow = 403;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_404_clicked()
{
    emit timerstop();
    roomnumberforshow = 404;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_405_clicked()
{
    emit timerstop();
    roomnumberforshow = 405;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_406_clicked()
{
    emit timerstop();
    roomnumberforshow = 406;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_407_clicked()
{
    emit timerstop();
    roomnumberforshow = 407;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_408_clicked()
{
    emit timerstop();
    roomnumberforshow = 408;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_409_clicked()
{
    emit timerstop();
    roomnumberforshow = 409;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_410_clicked()
{
    emit timerstop();
    roomnumberforshow = 410;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_411_clicked()
{
    emit timerstop();
    roomnumberforshow = 411;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_412_clicked()
{
    emit timerstop();
    roomnumberforshow = 412;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_413_clicked()
{
    emit timerstop();
    roomnumberforshow = 413;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_501_clicked()
{
    emit timerstop();
    roomnumberforshow = 501;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_502_clicked()
{
    emit timerstop();
    roomnumberforshow = 502;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_503_clicked()
{
    emit timerstop();
    roomnumberforshow = 503;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_504_clicked()
{
    emit timerstop();
    roomnumberforshow = 504;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_505_clicked()
{
    emit timerstop();
    roomnumberforshow = 505;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_506_clicked()
{
    emit timerstop();
    roomnumberforshow = 506;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_507_clicked()
{
    emit timerstop();
    roomnumberforshow = 507;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_508_clicked()
{
    emit timerstop();
    roomnumberforshow = 508;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_509_clicked()
{
    emit timerstop();
    roomnumberforshow = 509;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_510_clicked()
{
    emit timerstop();
    roomnumberforshow = 510;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_511_clicked()
{
    emit timerstop();
    roomnumberforshow = 511;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_512_clicked()
{
    emit timerstop();
    roomnumberforshow = 512;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}

void MainWindow::on_pushButton_513_clicked()
{
    emit timerstop();
    roomnumberforshow = 513;
    Dialogroomshow a;
    a.exec();
    emit timerrestart();
}


void MainWindow::on_pushButton_manage_clicked()
{
    emit timerstop();

    manageflag = 1;
    Dialogmanage a;
    a.exec();
    if(backupflag == 1)
    {
        receive_pushbutton_log();
        backupflag = 0;
    }
    manageflag = 0;

    emit timerrestart();
    color();
    shownumeber();
}

void MainWindow::on_pushButton_checkout_clicked()
{
    emit timerstop();
    Dialogcheck_out a;
     a.exec();
    if(backupflag == 1)
    {
        receive_pushbutton_log();
        backupflag = 0;
    }

    emit timerrestart();
    color();
    ui->general_income->setText(QString::number(general_income));
    occupancy_rate=float(clients->person.roomnumber)/float(65);
    occupancy_rate=occupancy_rate*100;
    ui->occupancyrate->setText(QString::number(occupancy_rate,'f',1)+"%");

    creat_remind();
    show_remind_tab();
    shownumeber();
}

void MainWindow::receive_pushbutton_log()         //uiμ?±?·Yoˉêy
{
    string str;
    ifstream log("log_manage.txt");
    if(first == 0)
    {
        getline(log,str);
        ui->textBrowser_log->append(QString::fromStdString(str));
        first = 1;
    }
    log.seekg(posi,ios::beg);
    while(log.peek() != EOF)
    {
        getline(log,str);
        ui->textBrowser_log->append(QString::fromStdString(str));
    }
    posi = log.tellg();

    log.close();
}

void MainWindow::change_roomsign()
{
    int i=0;
    //根据分配表载入预约房间数
    for(i=0;i<5;i++){
        hotel[i]->room.roomprice=0;
    }
    for(i=0;i<65;i++){
        if(reserve_tab[0][i].name!=""&&reserve_tab[0][i].room_num!=0){
            if (reserve_tab[0][i].roomtype == single){
                 hotel[0]->room.roomprice+=reserve_tab[0][i].room_num;
              //   qDebug()<<hotel[0]->room.roomprice;//..............................
            }
            else
                if (reserve_tab[0][i].roomtype == standard){
                    hotel[1]->room.roomprice+=reserve_tab[0][i].room_num;
                }
            else
                if (reserve_tab[0][i].roomtype == business){
                    hotel[2]->room.roomprice+=reserve_tab[0][i].room_num;
                }
            else
                if (reserve_tab[0][i].roomtype == deluxe){
                    hotel[3]->room.roomprice+=reserve_tab[0][i].room_num;
                }
            else
                if (reserve_tab[0][i].roomtype == president){
                    hotel[4]->room.roomprice+=reserve_tab[0][i].room_num;
                }
        }
    }
    for(i=0;i<5;i++){
      Roomlink newptr=NULL;
      newptr=hotel[i]->next;
      int num=0;
      while(newptr&&num<hotel[i]->room.roomprice){         //改变房间状态
          if(newptr->room.sign==Reserve){
              newptr=newptr->next;
              num++;
          }
          else{

              newptr->room.sign=Reserve;
              num=num+1;
              newptr=newptr->next;
          }

      }
      while(newptr&&newptr->room.sign!=Empty){
          if(newptr->room.sign==Reserve)
               newptr->room.sign=Empty;
          newptr=newptr->next;
      }

    }
}

void MainWindow::on_pushButton_reserve_clicked()
{
    emit timerstop();
    manageflag = 1;
    Dialogreserve a;
    a.exec();
    if(backupflag == 1)
    {
        receive_pushbutton_log();
        backupflag = 0;
    }
    manageflag = 0;
    emit timerrestart();

    change_roomsign();
    color();
    show_tabWidget_currentChanged();
    shownumeber();

    ui->general_income->setText(QString::number(general_income));
    occupancy_rate=float(clients->person.roomnumber)/float(65);
    occupancy_rate=occupancy_rate*100;
    ui->occupancyrate->setText(QString::number(occupancy_rate,'f',1)+"%");

}

void MainWindow::show_tabWidget_currentChanged()
{
    ui->tableWidget->clear();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList header;
    header<<"客户姓名"<<"电话号码"<<"预约时间"<<"预约入住时间"<<"预约天数"<<"房间类型"<<"房间间数";
    ui->tableWidget->setHorizontalHeaderLabels(header);

    Reserve_clientlink newptr=head_Reserve_clientlink->next;

    int j=0;
    while(newptr){
        if(newptr->reserve_client.room_num!=0){
            QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(newptr->reserve_client.name));
            ui->tableWidget->setItem(j, 0, newItem);
            newItem = new QTableWidgetItem(QString::fromStdString(newptr->reserve_client.telephone));
            ui->tableWidget->setItem(j, 1, newItem);
            newItem = new QTableWidgetItem(QString::number(newptr->reserve_client.reserve_time));
            ui->tableWidget->setItem(j, 2, newItem);
            newItem = new QTableWidgetItem(QString::number(newptr->reserve_client.timein));
            ui->tableWidget->setItem(j, 3, newItem);
            newItem = new QTableWidgetItem(QString::number(newptr->reserve_client.reserve_stay));
            ui->tableWidget->setItem(j, 4, newItem);

            if (newptr->reserve_client.roomtype == single)
               newItem = new QTableWidgetItem(QString::fromStdString("单人间"));
            else
                if (newptr->reserve_client.roomtype == standard)
                    newItem = new QTableWidgetItem(QString::fromStdString("标准间"));
            else
                if (newptr->reserve_client.roomtype == business)
                    newItem = new QTableWidgetItem(QString::fromStdString("商务间"));
            else
                if (newptr->reserve_client.roomtype == deluxe)
                    newItem = new QTableWidgetItem(QString::fromStdString("豪华套房"));
            else
                if (newptr->reserve_client.roomtype == president)
                    newItem = new QTableWidgetItem(QString::fromStdString("总统套房"));
            ui->tableWidget->setItem(j, 5, newItem);

            newItem = new QTableWidgetItem(QString::number(newptr->reserve_client.room_num));
            ui->tableWidget->setItem(j, 6, newItem);
            j++;
        }
        newptr=newptr->next;
      }
}

void MainWindow::show_remind_tab()
{
    ui->tableWidget_2->clear();
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);



    Clientlink newptr=head_remind_clients->next;
    int j=0;
    while(newptr){
       // if(newptr->reserve_client.room_num!=0){
            QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(newptr->person.name));
            ui->tableWidget_2->setItem(j, 0, newItem);
            newItem = new QTableWidgetItem(QString::fromStdString(newptr->person.sex));
            ui->tableWidget_2->setItem(j, 1, newItem);
            newItem = new QTableWidgetItem(QString::fromStdString(newptr->person.identy));
            ui->tableWidget_2->setItem(j, 2, newItem);
            newItem = new QTableWidgetItem(QString::fromStdString(newptr->person.telephone));
            ui->tableWidget_2->setItem(j, 3, newItem);
            newItem = new QTableWidgetItem(QString::number(newptr->person.roomnumber));
            ui->tableWidget_2->setItem(j, 4, newItem);
            newItem = new QTableWidgetItem(QString::number(newptr->person.timein));
            ui->tableWidget_2->setItem(j, 5, newItem);
            newItem = new QTableWidgetItem(QString::number(newptr->person.staytime));
            ui->tableWidget_2->setItem(j, 6, newItem);
            newItem = new QTableWidgetItem(QString::number(newptr->person.leavetime));
            ui->tableWidget_2->setItem(j, 7, newItem);

            if (newptr->person.roomtype == single)
               newItem = new QTableWidgetItem(QString::fromStdString("单人间"));
            else
                if (newptr->person.roomtype == standard)
                    newItem = new QTableWidgetItem(QString::fromStdString("标准间"));
            else
                if (newptr->person.roomtype == business)
                    newItem = new QTableWidgetItem(QString::fromStdString("商务间"));
            else
                if (newptr->person.roomtype == deluxe)
                    newItem = new QTableWidgetItem(QString::fromStdString("豪华套房"));
            else
                if (newptr->person.roomtype == president)
                    newItem = new QTableWidgetItem(QString::fromStdString("总统套房"));
            ui->tableWidget_2->setItem(j, 8, newItem);

            j++;

        newptr=newptr->next;
      }

      Clientlink preptr=head_remind_clients->next;
      head_remind_clients->next=NULL;
      if(preptr==NULL)
                return;
      else{
                Clientlink newptr=preptr->next;
                while(preptr){
                    delete  preptr;
                    preptr=newptr;
                    if(newptr)
                        newptr=newptr->next;
                }
       }
     head_remind_clients->person.roomnumber=0;
}


void MainWindow::on_statistics_clicked()
{
    emit timerstop();
    Dialogstatistics a;
    a.exec();
    emit timerrestart();
}


void MainWindow::on_pushButton_timer_clicked()
{
    emit timerstop();
    Dialogtimeline a;
    a.exec();
    emit timerrestart();
}
