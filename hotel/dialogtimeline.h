#ifndef DIALOGTIMELINE_H
#define DIALOGTIMELINE_H

#include <QDialog>

namespace Ui {
class Dialogtimeline;
}

class Dialogtimeline : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogtimeline(QWidget *parent = 0);
    ~Dialogtimeline();

private slots:

    void on_pushButton_yes_clicked();

private:
    Ui::Dialogtimeline *ui;
};

#endif // DIALOGTIMELINE_H
