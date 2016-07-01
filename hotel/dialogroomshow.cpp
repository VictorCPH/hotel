#include "dialogroomshow.h"
#include "ui_dialogroomshow.h"
#include "dialoggoods_amount.h"
#include "hotel.h"

extern Roomlink hotel[LINKNUM];
extern int roomnumberforshow;
extern int amount;//物品数量
extern int flagforbuygoods;//额外消费中使用
int sum = 0;//记录消费金融，作用域本ui

Dialogroomshow::Dialogroomshow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogroomshow)
{
    ui->setupUi(this);

    ui->tableWidget_pricetab->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_accounttab->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget_pricetab->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    ui->tableWidget_accounttab->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式

    ui->tableWidget_pricetab->verticalHeader()->setVisible(false);   //隐藏列表头
    ui->tableWidget_accounttab->verticalHeader()->setVisible(false);   //隐藏列表头

    showinformatin();
}

Dialogroomshow::~Dialogroomshow()
{
    delete ui;
}

void Dialogroomshow::showinformatin()
{
    int roomnumber = roomnumberforshow;
    Roomlink   searchptr=NULL;

    searchptr = hotel[roomnumber/100-1]->next;
    while(searchptr && searchptr->room.roomnumber != roomnumber)
    {
         searchptr = searchptr->next;
    }

    if (searchptr == NULL)//该房间不在空闲链
    {
        searchptr = hotel[roomnumber/100+4]->next;
        while(searchptr && searchptr->room.roomnumber != roomnumber)
        {
             searchptr = searchptr->next;
        }
    }
//房间号1	客户名1	房间类型1	房间价格1	房间状态1	入住时间	已住天数1	计划离店时间	额外消费1
    ui->textBrowser_roomnumber->setText(QString::number(searchptr->room.roomnumber,10));
    ui->textBrowser_roomprice->setText(QString::number(searchptr->room.roomprice,10));
    ui->textBrowser_day->setText(QString::number(searchptr->room.day, 'f', 1));
    ui->textBrowser_consumption->setText(QString::number(searchptr->room.consumption,10));
    ui->textBrowser_clientname->setText(QString::fromStdString(searchptr->room.clientname));
    ui->textBrowser_timein->setText(QString::number(searchptr->room.stayin,10));
    ui->textBrowser_leavetime->setText(QString::number(searchptr->room.leavetime,10));

    switch(searchptr->room.roomtype)
    {//单人间，标准间，商务间，豪华套房，总统套房
         case single:
            ui->textBrowser_roomtype->setText("单人间");
            break;
        case standard:
            ui->textBrowser_roomtype->setText("标准间");
            break;
        case business:
            ui->textBrowser_roomtype->setText("商务间");
            break;
        case deluxe:
            ui->textBrowser_roomtype->setText("豪华套房");
            break;
        case president:
            ui->textBrowser_roomtype->setText("总统套房");
            break;
        default:
            break;
   }
   switch(searchptr->room.sign)
   {
        case Empty:
            ui->textBrowser_roomsign->setText("空闲");
            break;
        case Reserve:
            ui->textBrowser_roomsign->setText("预定");
            break;
        case Occupy:
            ui->textBrowser_roomsign->setText("占据");
            break;
        case Repair:
            ui->textBrowser_roomsign->setText("维修");
            break;
        default:
            break;
   }
}

void Dialogroomshow::on_pushButton_clicked()
{
    close();
}

void Dialogroomshow::on_tableWidget_pricetab_itemDoubleClicked(QTableWidgetItem *item)
{
    Dialoggoods_amount a;
    a.exec();

    if (flagforbuygoods == 0)
    {
        int rowIndex = ui->tableWidget_accounttab->rowCount();

        ui->tableWidget_accounttab->setRowCount(rowIndex + 1);//总行数增加1

        int rownumber = item->row();
        ui->tableWidget_accounttab->setItem(rowIndex, 0, new QTableWidgetItem(ui->tableWidget_pricetab->item(rownumber, 0)->text()));
        ui->tableWidget_accounttab->setItem(rowIndex, 1, new QTableWidgetItem(ui->tableWidget_pricetab->item(rownumber, 1)->text()));

        ui->tableWidget_accounttab->setItem(rowIndex, 2, new QTableWidgetItem(QString::number(amount, 10)));

        int pricetotal = ui->tableWidget_pricetab->item(rownumber, 1)->text().toInt() * amount;

        ui->tableWidget_accounttab->setItem(rowIndex, 3 , new QTableWidgetItem(QString::number(pricetotal, 10)));

        sum = sum + pricetotal;

        ui->textBrowser_total->setText(QString::number(sum, 10));

        flagforbuygoods = 1;
    }
}

void Dialogroomshow::on_pushButton_yesforbuy_clicked()
{
    int roomnumber = roomnumberforshow;
    Roomlink   searchptr=NULL;

    searchptr = hotel[roomnumber/100-1]->next;
    while(searchptr && searchptr->room.roomnumber != roomnumber)
    {
         searchptr = searchptr->next;
    }

    if (searchptr == NULL)//该房间不在空闲链
    {
        searchptr = hotel[roomnumber/100+4]->next;
        while(searchptr && searchptr->room.roomnumber != roomnumber)
        {
             searchptr = searchptr->next;
        }
    }

    searchptr->room.consumption = searchptr->room.consumption + sum;

    flagforbuygoods = 0;

    close();
}
