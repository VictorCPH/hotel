#ifndef DIALOGBACKUP_H
#define DIALOGBACKUP_H

#include <QDialog>

namespace Ui {
class Dialogbackup;
}

class Dialogbackup : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogbackup(QWidget *parent = 0);
    ~Dialogbackup();
private slots:
    void change();
    void Nowstart();


private:
    Ui::Dialogbackup *ui;

};

#endif // DIALOGBACKUP_H
