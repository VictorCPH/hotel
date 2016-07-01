#include "dialogtimeline.h"
#include "ui_dialogtimeline.h"

extern int ratio;

Dialogtimeline::Dialogtimeline(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogtimeline)
{
    ui->setupUi(this);
}

Dialogtimeline::~Dialogtimeline()
{
    delete ui;
}

void Dialogtimeline::on_pushButton_yes_clicked()
{
    ratio = ui->lineEdit->text().toInt();
    close();
}
