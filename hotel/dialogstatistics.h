#ifndef DIALOGSTATISTICS_H
#define DIALOGSTATISTICS_H

#include <QDialog>

namespace Ui {
class Dialogstatistics;
}

class Dialogstatistics : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogstatistics(QWidget *parent = 0);
    void show_nowday_income();
    void show_month_income();
    void show_year_income();
    ~Dialogstatistics();

private:
    Ui::Dialogstatistics *ui;
};

#endif // DIALOGSTATISTICS_H
