#include "dialogcheck_out.h"
#include"mainwindow.h"
#include<log_manage.h>

extern Roomlink hotel[LINKNUM];
extern Clientlink clients;
extern int PRICESINGLE;
extern int PRICESTANDAR;
extern int PRICEBUSINESS;
extern int PRICEDELUXE;
extern int PRICEPRESIDENT;
int account = 0;
extern int general_income;

extern struct tm *timeNow; //当前时间
extern int  can_occupy_room_num[30][5];

extern int time_difference(int  timein,int nowtime);
extern int backupflag;

extern Income income[66];
extern Dayincome  dayincome[13][32];

int repeat=0;
Dialogcheck_out::Dialogcheck_out(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogcheck_out)
{
    ui->setupUi(this);


}

Dialogcheck_out::~Dialogcheck_out()
{
    delete ui;
}

void Dialogcheck_out::on_pushButton_account_clicked()
{
    if(repeat==1){
        return;
    }

    QString Day,Price,Total,Extra;         //用于ui输出；
    int roomnum, floor;
    double totalprice, price;
    Roomlink searchptr = NULL;                 //searchptr用于遍历入住链表；
    if (ui->lineEdit_roomnumber->text().isEmpty()){
         QMessageBox::information(this,tr(""),tr("信息填写不全"));
         return;
    }
    else
    {
        roomnum = ui->lineEdit_roomnumber->text().toInt();
        floor = roomnum / 100;
        searchptr = hotel[floor + 4]->next;
        while (searchptr != NULL)
        {
            if (searchptr->room.roomnumber == roomnum)
            {
                repeat=1;
                price = (searchptr->room.roomprice) * (searchptr->room.day);
                totalprice = price + (searchptr->room.consumption);       //总房价；

                Day = QString::number(searchptr->room.day,'f',1);
                Price = QString::number(price,'f',1);
                Total = QString::number(totalprice,'f',1);
                Extra = QString::number(searchptr->room.consumption,10);

                /*****************************************/
                int d=timeNow->tm_mday;
                int mon=timeNow->tm_mon;
                dayincome[mon][d].roomincome+=price;
                dayincome[mon][d].consumption+=totalprice-price;
                dayincome[mon][d].generalincome+=totalprice;
                dayincome[mon][0].roomincome+=price;
                dayincome[mon][0].consumption+=totalprice-price;
                dayincome[mon][0].generalincome+=totalprice;
                dayincome[0][0].roomincome+=price;
                dayincome[0][0].consumption+=totalprice-price;
                dayincome[0][0].generalincome+=totalprice;

                int t=1;
                for(t=1;t<66;t++){
                    if(income[t].roomnumber==0&&income[t].roomincome==0){
                        if(t>1){
                            if(income[t].roomnumber==income[t-1].roomnumber)
                                break;
                        }
                        income[t].roomnumber=searchptr->room.roomnumber;
                        income[t].consumption=searchptr->room.consumption;
                        income[t].roomincome=price;
                        income[0].roomnumber++;
                        income[0].consumption+=searchptr->room.consumption;
                        income[0].roomincome+=price;
                        break;
                    }
                }
                t=0;




                /*******************************/

                general_income=general_income+totalprice;      //总收入

                ui->textBrowser_price->setText(Price);
                ui->textBrowser_extra->setText(Extra);
                ui->textBrowser_total->setText(Total);
                ui->textBrowser_name->setText(QString::fromStdString(searchptr->room.clientname));
                ui->textBrowser_days->setText(Day);

                account = 1;
                break;
             }
             else
                searchptr = searchptr->next;
          }
        if(account == 0)             //输入的房间不在入住链表中；
            QMessageBox::information(this,tr(""),tr("信息填写有误，请重新输入"));
            return;
     }

}


void Dialogcheck_out::on_pushButton_no_clicked()
{
    close();
}


void Dialogcheck_out::on_pushButton_yes_clicked()
{
    if(account == 0)
        QMessageBox::information(this,tr(""),tr("请先结账"));

    else
    {
        int roomnum, floor;
        string name;
        Roomlink preptr = NULL, searchptr = NULL,nowptr = NULL;                 //searchptr定位链表中的位置，preptr为searchptr的前驱，用于修改入住链表
                                                                                // nowptr用于空闲链表的修改

        Clientlink preptr1 = NULL, searchptr1 = NULL;                         //searchptr1,preptr1用于修改客户链表
        preptr1 = clients;
        searchptr1 = preptr1->next;

        if (ui->lineEdit_roomnumber->text().isEmpty())
             QMessageBox::information(this,tr(""),tr("信息填写不全"));
        else
        {
            roomnum = ui->lineEdit_roomnumber->text().toInt();

            floor = roomnum / 100;
            preptr = hotel[floor + 4];
            nowptr = hotel[floor-1];
            searchptr = preptr->next;

            while (searchptr != NULL)
            {
                if (searchptr->room.roomnumber == roomnum)
                {
                    log_manage(searchptr,Check_out);
                    backupflag = 1;

                    name = searchptr->room.clientname;
                    searchptr->room.clientname = "empty";           //重置房间信息按single, standard, business, deluxe, president分类
                    searchptr->room.consumption = 0;
                    searchptr->room.day = 0;
                    searchptr->room.sign = Empty;
                    searchptr->room.leavetime = 0;
                    searchptr->room.stayin = 0;

                    int roomtype_num=0;   //判断是哪类房间
                   switch (searchptr->room.roomtype) {
                   case single:
                       searchptr->room.roomprice = PRICESINGLE;
                       roomtype_num=0;
                       break;
                   case standard:
                       searchptr->room.roomprice = PRICESTANDAR;
                       roomtype_num=1;
                       break;
                   case business:
                       searchptr->room.roomprice = PRICEBUSINESS;
                       roomtype_num=2;
                       break;
                   case deluxe:
                       searchptr->room.roomprice = PRICEDELUXE;
                       roomtype_num=3;
                       break;
                   case president:
                       searchptr->room.roomprice = PRICEPRESIDENT;
                       roomtype_num=4;
                       break;
                   default:
                       break;
                   }

                    preptr->next = searchptr->next;                 //在入住链表中删除

                    while (nowptr->next != NULL)						//在空闲链表中添加
                    {
                        if (searchptr->room.roomnumber < nowptr->next->room.roomnumber)
                        {
                            searchptr->next = nowptr->next;
                            nowptr->next = searchptr;
                            break;
                        }
                        else
                            nowptr = nowptr->next;
                     }

                     if (nowptr->next == NULL)
                     {
                         nowptr->next = searchptr;
                         searchptr->next = NULL;
                     }

                     hotel[floor + 4]->room.roomnumber--;          //计数
                     hotel[floor]->room.roomnumber++;


                     while (searchptr1 != NULL)                     //在客户链表中删除
                     {
                         if (searchptr1->person.name == name)
                         {
                             /*************************************/       //提前退房，可用房间则加
                             int now_time=timeNow->tm_year*10000+timeNow->tm_mon*100+timeNow->tm_mday;
                             int t=time_difference(searchptr1->person.leavetime,now_time);

                             int stayday=searchptr1->person.staytime;      //**************************
                             searchptr1->person.staytime=searchptr1->person.staytime-t;   //....................记录客户离店信息
                             backup_client_leave(searchptr1);
                             searchptr1->person.staytime=stayday;                         //************************8888


                             int p=0;
                             for(p=0;p<t;p++){
                                 can_occupy_room_num[p][roomtype_num]+=can_occupy_room_num[p][roomtype_num]+searchptr1->person.staytime;
                             }


                            /**************************************/
                             preptr1->next = searchptr1->next;
                             clients->person.roomnumber--;             //入住客户人数减一//fgdhfddddddddddddddddd
                             delete searchptr1;
                             break;
                         }
                         else
                         {
                              preptr1 = searchptr1;
                              searchptr1 = searchptr1->next;
                         }

                     }

                     break;           //打断遍历占据链表，不能删；
                }

                else
                {
                    preptr = searchptr;
                    searchptr = searchptr->next;
                }
            }

            QMessageBox::information(this,tr(""),tr("离店成功"));
            repeat=0;
            close();
        }

    account = 0;


    ui->textBrowser_price->clear();
    ui->textBrowser_extra->clear();
    ui->textBrowser_total->clear();
    ui->textBrowser_name->clear();
    ui->textBrowser_days->clear();
    ui->lineEdit_roomnumber->clear();
    }

}

