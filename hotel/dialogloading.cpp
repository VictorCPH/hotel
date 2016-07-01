#include "dialogloading.h"
#include "ui_dialogloading.h"
#include "hotel.h"
#include <QMessageBox>

extern int loadsuccess;

Dialogloading::Dialogloading(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogloading)
{
    ui->setupUi(this);
    ui->pushButton_load->setShortcut(QKeySequence::InsertParagraphSeparator);
    ui->pushButton_load_register->setShortcut(QKeySequence::InsertParagraphSeparator);
    ui->lineEdit_password_load->setEchoMode(QLineEdit::Password);
    ui->lineEdit_password_register_1->setEchoMode(QLineEdit::Password);
    ui->lineEdit_password_register_2->setEchoMode(QLineEdit::Password);

}

Dialogloading::~Dialogloading()
{
    delete ui;
}

void Dialogloading::on_pushButton_load_register_clicked()
{
    string username;
    string password;
    if (ui->lineEdit_username_register->text().isEmpty())
    {
        QMessageBox::information(this,tr(""),tr("用户名不能为空"));
        return;
    }
    if (ui->lineEdit_password_register_1->text().isEmpty() || ui->lineEdit_password_register_2->text().isEmpty())
    {
        QMessageBox::information(this,tr(""),tr("密码不能为空"));
        return;
    }

    if (ui->lineEdit_password_register_1->text() != ui->lineEdit_password_register_2->text())
    {
        QMessageBox::information(this,tr(""),tr("两次密码输入不一致，请重新输入"));
        return;
    }

    ifstream in("administrator.txt");
    if (! in.is_open())
    {
        qDebug()<< "Error opening administrator.txt";
        exit (1);
    }


   //判断是否重名
    while (in.peek() != EOF)
    {
        in>>username;
        in>>password;

        if (username == ui->lineEdit_username_register->text().toStdString())
        {
            QMessageBox::information(this,tr(""),tr("该用户名已被使用，请重新输入"));
            return;
        }
    }
    in.close();
    username = ui->lineEdit_username_register->text().toStdString();
    password = ui->lineEdit_password_register_1->text().toStdString();

    ofstream out("administrator.txt",ios::app);
    if (! out.is_open())
    {
       qDebug()<< "Error opening administrator.txt";
       exit (1);
    }

    out<<endl;
    out<<username<<endl;
    out<<password<<endl;
    out.close();
    QMessageBox::information(this,tr(""),tr("注册成功"));

}

void Dialogloading::on_pushButton_load_clicked()
{
    string username;
    string password;
    if (ui->lineEdit_username_load->text().isEmpty())
    {
        QMessageBox::information(this,tr(""),tr("用户名不能为空"));
        return;
    }
    if (ui->lineEdit_password_load->text().isEmpty())
    {
        QMessageBox::information(this,tr(""),tr("密码不能为空"));
        return;
    }

    ifstream in("administrator.txt");
    if (! in.is_open())
    {
        qDebug()<< "Error opening administrator.txt";
        exit (1);
    }


   //判断密码是否正确
    while (in.peek() != EOF)
    {
        in>>username;
        in>>password;

        if (username == ui->lineEdit_username_load->text().toStdString() && password == ui->lineEdit_password_load->text().toStdString())
        {
            QMessageBox::information(this,tr(""),tr("登陆成功"));
            loadsuccess = 1;
            close();
            return;
        }
    }

    QMessageBox::information(this,tr(""),tr("密码错误"));

    in.close();
}
