#ifndef DIALOGLOADING_H
#define DIALOGLOADING_H

#include <QDialog>

namespace Ui {
class Dialogloading;
}

class Dialogloading : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogloading(QWidget *parent = 0);
    ~Dialogloading();

private slots:

    void on_pushButton_load_register_clicked();

    void on_pushButton_load_clicked();

private:
    Ui::Dialogloading *ui;
};

#endif // DIALOGLOADING_H
