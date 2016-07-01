#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <hotel.h>
#include "dialogbackup.h"
#include "dialogclientin.h"
#include "dialoginquiry.h"
#include "dialogroomshow.h"
#include "dialogcheck_out.h"
#include "dialogmanage.h"
#include "dialogreserve.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
     void color();
    void roomnumber_match(Roomlink);
    void shownumeber();
    void backupincome();
    void backup_general_income_occupancy_rate();
    void backupclient(); //备份客户和房间信息；
    void backuproom();
    void backup_reserve_client();
    void backupsoftwaretime();
    void backupprice();
    void show_tabWidget_currentChanged();
    void change_roomsign();
    void show_remind_tab();
    void creat_remind();
    void cancel_reserve(string name,string telephone);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_inquiry_clicked();

    void on_pushButton_101_clicked();

    void timerUpdate();

    void on_pushButton_clientin_clicked();

    void on_pushButton_102_clicked();

    void on_pushButton_104_clicked();

    void on_pushButton_103_clicked();

    void on_pushButton_105_clicked();

    void on_pushButton_106_clicked();

    void on_pushButton_107_clicked();

    void on_pushButton_108_clicked();

    void on_pushButton_109_clicked();

    void on_pushButton_110_clicked();

    void on_pushButton_111_clicked();

    void on_pushButton_112_clicked();

    void on_pushButton_113_clicked();

    void on_pushButton_201_clicked();

    void on_pushButton_202_clicked();

    void on_pushButton_203_clicked();

    void on_pushButton_204_clicked();

    void on_pushButton_205_clicked();

    void on_pushButton_206_clicked();

    void on_pushButton_207_clicked();

    void on_pushButton_208_clicked();

    void on_pushButton_209_clicked();

    void on_pushButton_210_clicked();

    void on_pushButton_211_clicked();

    void on_pushButton_212_clicked();

    void on_pushButton_213_clicked();

    void on_pushButton_301_clicked();

    void on_pushButton_302_clicked();

    void on_pushButton_303_clicked();

    void on_pushButton_304_clicked();

    void on_pushButton_305_clicked();

    void on_pushButton_306_clicked();

    void on_pushButton_307_clicked();

    void on_pushButton_308_clicked();

    void on_pushButton_309_clicked();

    void on_pushButton_310_clicked();

    void on_pushButton_311_clicked();

    void on_pushButton_312_clicked();

    void on_pushButton_313_clicked();

    void on_pushButton_401_clicked();

    void on_pushButton_402_clicked();

    void on_pushButton_403_clicked();

    void on_pushButton_404_clicked();

    void on_pushButton_405_clicked();

    void on_pushButton_406_clicked();

    void on_pushButton_407_clicked();

    void on_pushButton_408_clicked();

    void on_pushButton_409_clicked();

    void on_pushButton_410_clicked();

    void on_pushButton_411_clicked();

    void on_pushButton_412_clicked();

    void on_pushButton_413_clicked();

    void on_pushButton_501_clicked();

    void on_pushButton_502_clicked();

    void on_pushButton_503_clicked();

    void on_pushButton_504_clicked();

    void on_pushButton_505_clicked();

    void on_pushButton_506_clicked();

    void on_pushButton_507_clicked();

    void on_pushButton_508_clicked();

    void on_pushButton_509_clicked();

    void on_pushButton_510_clicked();

    void on_pushButton_511_clicked();

    void on_pushButton_512_clicked();

    void on_pushButton_513_clicked();

    void on_pushButton_manage_clicked();

    void on_pushButton_checkout_clicked();

    void on_pushButton_reserve_clicked();

    void receive_pushbutton_log();

    void on_statistics_clicked();

    void on_pushButton_timer_clicked();

signals:
    void timerstop();
    void timerrestart();
    void Nowbackup();

private:
    Ui::MainWindow *ui;
    Dialogbackup backup;
};

#endif // MAINWINDOW_H
