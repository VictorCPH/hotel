#include "hotel.h"
#include "dialogmanage.h"
#include "ui_dialogmanage.h"
#include <QMessageBox>
#include <qdebug.h>
#include <log_manage.h>


extern Roomlink hotel[LINKNUM];
extern int PRICESINGLE;
extern int PRICESTANDAR;
extern int PRICEBUSINESS;
extern int PRICEDELUXE;
extern int PRICEPRESIDENT;
extern int backupflag;

Dialogmanage::Dialogmanage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogmanage)
{
    ui->setupUi(this);
    showprice();
}

Dialogmanage::~Dialogmanage()
{
    delete ui;
}

void Dialogmanage::showprice()
{
    ui->textBrowser_singleoriginal->setText(QString::number(PRICESINGLE,10));
    ui->textBrowser_standardoriginal->setText(QString::number(PRICESTANDAR,10));
    ui->textBrowser_bussinessoriginal->setText(QString::number(PRICEBUSINESS,10));
    ui->textBrowser_deluxeoriginal->setText(QString::number(PRICEDELUXE,10));
    ui->textBrowser_presidentoriginal->setText(QString::number(PRICEPRESIDENT,10));
}


void Dialogmanage::on_pushButton_yesforprice_clicked()
{
    if (! ui->lineEdit_singlechange->text().isEmpty())
        PRICESINGLE = ui->lineEdit_singlechange->text().toInt();
    if (! ui->lineEdit_standardchange->text().isEmpty())
        PRICESTANDAR = ui->lineEdit_standardchange->text().toInt();
    if (! ui->lineEdit_bussinesschange->text().isEmpty())
        PRICEBUSINESS = ui->lineEdit_bussinesschange->text().toInt();
    if (! ui->lineEdit_deluxechange->text().isEmpty())
        PRICEDELUXE = ui->lineEdit_deluxechange->text().toInt();
    if (! ui->lineEdit_presidentchange->text().isEmpty())
        PRICEPRESIDENT = ui->lineEdit_presidentchange->text().toInt();

    int i;

    for (i=0; i<=4; i++)
    {
        Roomlink t = hotel[i]->next;

        while(t)
        {
            switch(i)
            {
                case 0:
                    t->room.roomprice = PRICESINGLE;
                    break;
                case 1:
                    t->room.roomprice = PRICESTANDAR;
                    break;
                case 2:
                    t->room.roomprice = PRICEBUSINESS;
                    break;
                case 3:
                    t->room.roomprice = PRICEDELUXE;
                    break;
                case 4:
                    t->room.roomprice = PRICEPRESIDENT;
                    break;
            }
            t=t->next;
        }

    }

    if (! ui->lineEdit_singlechange->text().isEmpty())            // ??////////////////////////////////////////////////////
            {
                log_manage(hotel[0]->next,change_price);
                backupflag = 1;
            }
            if (! ui->lineEdit_standardchange->text().isEmpty())
            {
                log_manage(hotel[1]->next,change_price);
                backupflag = 1;
            }
            if (! ui->lineEdit_bussinesschange->text().isEmpty())
            {
                log_manage(hotel[2]->next,change_price);
                backupflag = 1;
            }
            if (! ui->lineEdit_deluxechange->text().isEmpty())
            {
                log_manage(hotel[3]->next,change_price);
                backupflag = 1;
            }
            if (! ui->lineEdit_presidentchange->text().isEmpty())
            {
                log_manage(hotel[4]->next,change_price);
               backupflag = 1;
            }
        close();
}

void Dialogmanage::on_pushButton_noforprice_clicked()
{
    close();
}

void Dialogmanage::on_pushButton_yesforservice_clicked()
{
    if (ui->lineEdit_serviceroomnumber->text().isEmpty())
    {
        QMessageBox::information(this,tr(""),tr("信息填写不全"));
        return;
    }

    int roomnumber = ui->lineEdit_serviceroomnumber->text().toInt();

    if ((roomnumber >= 101 && roomnumber <= 113) ||
        (roomnumber >= 201 && roomnumber <= 213) ||
        (roomnumber >= 301 && roomnumber <= 313) ||
        (roomnumber >= 401 && roomnumber <= 413) ||
        (roomnumber >= 501 && roomnumber <= 513))
        ;//房间号非法输入判断
    else
    {
        QMessageBox::information(this,tr(""),tr("无此房间,请重新输入"));
        return;
    }

    Roomlink t = hotel[roomnumber/100-1]->next;//找到房间号对应的空闲链表节点
    while(t && t->room.roomnumber != roomnumber)
    {
        t = t->next;
    }
    if (t == NULL)//该房间为占据
    {
        t = hotel[roomnumber/100+4]->next;//找到房间号对应的占据链表节点
        while(t && t->room.roomnumber != roomnumber)
        {
            t = t->next;
        }
        if (t->room.sign == Occupy)
            QMessageBox::information(this,tr(""),tr("该房间住有客户，请先退房，再转成维修状态"));

        if (t->room.sign == Repair)
            QMessageBox::information(this,tr(""),tr("该房为维修房，不需要转换"));

        return;
    }
    else
    {
        t->room.sign = Repair;
        hotel[roomnumber/100+4]->room.roomprice++;//维修房数目+1
        switch(roomnumber/100)
        {
            case 1:
                changeRoomlinkuseroom(0,5,t);
                break;
            case 2:
                changeRoomlinkuseroom(1,6,t);
                break;
            case 3:
                changeRoomlinkuseroom(2,7,t);
                break;
            case 4:
                changeRoomlinkuseroom(3,8,t);
                break;
            case 5:
                changeRoomlinkuseroom(4,9,t);
                break;
            default:
                break;
        }

    }

    close();
}

void changeRoomlinkuseroom(int a,int b,Roomlink newptr)//从a链剔除，加入b链，均为有序排列
{
    Roomlink 	preptr=NULL,searchptr=NULL,roompreptr=hotel[b],roomsearchptr=hotel[b]->next;
    searchptr=hotel[a]->next;
    preptr=hotel[a];

    while(searchptr&&searchptr->room.roomnumber!=newptr->room.roomnumber){
        preptr=searchptr;
       searchptr=searchptr->next;
    }

    if(searchptr!=NULL)
    {
       preptr->next=searchptr->next;
       hotel[a]->room.roomnumber--;          //jhvjhvbh
       searchptr->next=NULL;
       while(roomsearchptr&&roomsearchptr->room.roomnumber<searchptr->room.roomnumber)
       {
           roompreptr=roomsearchptr;
           roomsearchptr=roomsearchptr->next;
       }

       if(roomsearchptr==NULL)
       {
           roompreptr->next=searchptr;
       }
       else
       {
           searchptr->next=roomsearchptr;
           roompreptr->next=searchptr;
       }
       hotel[b]->room.roomnumber++;
   }

    if(b > 4)
    {
        log_manage(searchptr,repair);
        backupflag = 1;
    }
    else
    {
        log_manage(searchptr,repair_to_empty);
        backupflag = 1;
    }
}


void Dialogmanage::on_pushButton_noforservice_clicked()
{
    close();
}

void Dialogmanage::on_pushButton_yesforfree_clicked()
{
    if (ui->lineEdit_changefreenumber->text().isEmpty())
    {
        QMessageBox::information(this,tr(""),tr("信息填写不全"));
        return;
    }

    int roomnumber = ui->lineEdit_changefreenumber->text().toInt();

    if ((roomnumber >= 101 && roomnumber <= 113) ||
        (roomnumber >= 201 && roomnumber <= 213) ||
        (roomnumber >= 301 && roomnumber <= 313) ||
        (roomnumber >= 401 && roomnumber <= 413) ||
        (roomnumber >= 501 && roomnumber <= 513))
        ;//房间号非法输入判断
    else
    {
        QMessageBox::information(this,tr(""),tr("无此房间,请重新输入"));
        return;
    }

    Roomlink t = hotel[roomnumber/100+4]->next;//找到房间号对应的占据链表
    while(t && t->room.roomnumber != roomnumber)
    {
        t = t->next;
    }
    if (t == NULL)//该房间为占据
    {
        QMessageBox::information(this,tr(""),tr("该房间不是维修房，无法转换"));//该房为空闲房
        return;
    }
    else
    {
        if(t->room.sign != Repair)
        {
            QMessageBox::information(this,tr(""),tr("该房间不是维修房，无法转换"));//该房为占据房
            return;
        }
        else
        {
            t->room.sign = Empty;
            hotel[roomnumber/100+4]->room.roomprice--;//维修房数目减1
            switch(roomnumber/100)
            {
            case 1:
                changeRoomlinkuseroom(5,0,t);
                break;
            case 2:
                changeRoomlinkuseroom(6,1,t);
                break;
            case 3:
                changeRoomlinkuseroom(7,2,t);
                break;
            case 4:
                changeRoomlinkuseroom(8,3,t);
                break;
            case 5:
                changeRoomlinkuseroom(9,4,t);
                break;
            default:
                break;
            }
        }
    }

    close();
}

void Dialogmanage::on_pushButton_noforfree_clicked()
{
    close();
}
