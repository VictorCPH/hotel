#include "dialogstatistics.h"
#include "ui_dialogstatistics.h"
#include "hotel.h"

extern struct tm *timeNow; //当前时间
extern Income income[66];
extern Dayincome  dayincome[13][32];

Dialogstatistics::Dialogstatistics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogstatistics)
{
    ui->setupUi(this);
    show_nowday_income();
    show_month_income();
    show_year_income();

}

Dialogstatistics::~Dialogstatistics()
{
    delete ui;
}

void Dialogstatistics::show_nowday_income(){
    ui->tableWidget_2->clear();
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList header;
    header<<"房间号"<<"房价收入"<<"额外消费收入"<<"总收入";
    ui->tableWidget_2->setHorizontalHeaderLabels(header);

    QStringList header1;
    header1<<"当天统计收入";
    ui->tableWidget_2->setVerticalHeaderLabels(header1);

    int j=0,i=1;
//    QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(income[i].roomnumber));
   // ui->tableWidget_2->setItem(0, 0, newItem);
    QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(income[0].roomincome));
    ui->tableWidget_2->setItem(0, 1, newItem);
    newItem = new QTableWidgetItem(QString::number(income[0].consumption));
    ui->tableWidget_2->setItem(0, 2, newItem);
    newItem = new QTableWidgetItem(QString::number(income[0].consumption+income[0].roomincome));
    ui->tableWidget_2->setItem(0, 3, newItem);
    j=1;
    while(income[i].roomnumber){
       // if(newptr->reserve_client.room_num!=0){
            QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(income[i].roomnumber));
            ui->tableWidget_2->setItem(j, 0, newItem);
            newItem = new QTableWidgetItem(QString::number(income[i].roomincome));
            ui->tableWidget_2->setItem(j, 1, newItem);
            newItem = new QTableWidgetItem(QString::number(income[i].consumption));
            ui->tableWidget_2->setItem(j, 2, newItem);
            newItem = new QTableWidgetItem(QString::number(income[i].consumption+income[i].roomincome));
            ui->tableWidget_2->setItem(j, 3, newItem);

            j++;
            i++;
      }
}
void Dialogstatistics::show_month_income()
{
    ui->tableWidget->clear();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList header;
    header<<"房价收入"<<"额外消费收入"<<"总收入";
    ui->tableWidget->setHorizontalHeaderLabels(header);

    QStringList header1;
    header1<<"当月统计收入"<<"1号"<<"2号"<<"3号"<<"4号"<<"5号"<<"6号"<<"7号"<<"8号"<<"9号"<<"10号"<<"11号"<<"12号"
             <<"13号"<<"14号"<<"15号"<<"16号"<<"17号"<<"18号"<<"19号"<<"20号"<<"21号"<<"22号"<<"23号"<<"24号"
    <<"25号"<<"26号"<<"27号"<<"28号"<<"29号"<<"30号"<<"31号";
    ui->tableWidget->setVerticalHeaderLabels(header1);

    int month=timeNow->tm_mon;
    QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(dayincome[month][0].roomincome));
    ui->tableWidget->setItem(0, 0, newItem);
    newItem = new QTableWidgetItem(QString::number(dayincome[month][0].consumption));
    ui->tableWidget->setItem(0, 1, newItem);
    newItem = new QTableWidgetItem(QString::number(dayincome[month][0].generalincome));
    ui->tableWidget->setItem(0, 2, newItem);

    int j=1,i=1;
    for(i=1;i<32;i++){
            QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(dayincome[month][i].roomincome));
            ui->tableWidget->setItem(j, 0, newItem);
            newItem = new QTableWidgetItem(QString::number(dayincome[month][i].consumption));
            ui->tableWidget->setItem(j, 1, newItem);
            newItem = new QTableWidgetItem(QString::number(dayincome[month][i].generalincome));
            ui->tableWidget->setItem(j, 2, newItem);
            j++;

      }
}

void Dialogstatistics::show_year_income()
{
    ui->tableWidget_3->clear();
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList header;
    header<<"房价收入"<<"额外消费收入"<<"总收入";
    ui->tableWidget_3->setHorizontalHeaderLabels(header);
    QStringList header1;
    header1<<"当年统计收入"<<"1月"<<"2月"<<"3月"<<"4月"<<"5月"<<"6月"<<"7月"<<"8月"<<"9月"<<"10月"<<"11月"<<"12月";
    ui->tableWidget_3->setVerticalHeaderLabels(header1);
    //int month=timeNow->tm_mon;
    QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(dayincome[0][0].roomincome));
    ui->tableWidget_3->setItem(0, 0, newItem);
    newItem = new QTableWidgetItem(QString::number(dayincome[0][0].consumption));
    ui->tableWidget_3->setItem(0, 1, newItem);
    newItem = new QTableWidgetItem(QString::number(dayincome[0][0].generalincome));
    ui->tableWidget_3->setItem(0, 2, newItem);

    int j=1,i=1;
    for(i=1;i<13;i++){
       // if(newptr->reserve_client.room_num!=0){
            QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(dayincome[i][0].roomincome));
            ui->tableWidget_3->setItem(j, 0, newItem);
            newItem = new QTableWidgetItem(QString::number(dayincome[i][0].consumption));
            ui->tableWidget_3->setItem(j, 1, newItem);
            newItem = new QTableWidgetItem(QString::number(dayincome[i][0].generalincome));
            ui->tableWidget_3->setItem(j, 2, newItem);
            j++;


      }
}
