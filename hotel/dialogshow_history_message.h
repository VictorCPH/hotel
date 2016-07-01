#ifndef DIALOGSHOW_HISTORY_MESSAGE_H
#define DIALOGSHOW_HISTORY_MESSAGE_H

#include <QDialog>

namespace Ui {
class Dialogshow_history_message;
}

class Dialogshow_history_message : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogshow_history_message(QWidget *parent = 0);
    ~Dialogshow_history_message();

private slots:
    void on_pushButton_yes_clicked();

private:
    Ui::Dialogshow_history_message *ui;
};

#endif // DIALOGSHOW_HISTORY_MESSAGE_H
