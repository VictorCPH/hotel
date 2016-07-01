#include "dialogbackup.h"
#include "ui_dialogbackup.h"
#include "QTimer"
#include "qdebug.h"
#include "mainwindow.h"

Dialogbackup::Dialogbackup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogbackup)
{
    ui->setupUi(this);
    this-> setWindowFlags(Qt::FramelessWindowHint);
}

Dialogbackup::~Dialogbackup()
{
    delete ui;
}
int progress = 0;

void Dialogbackup::Nowstart()
{
    QTimer *timer1 = new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(change()));
    timer1->start(10);

}

void Dialogbackup::change()
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(progress);
    progress++;
    if(progress == 101)
        close();

}
