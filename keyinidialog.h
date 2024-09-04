#ifndef KEYINIDIALOG_H
#define KEYINIDIALOG_H

#include <QDialog>

namespace Ui {
class KeyIniDialog;
}

class KeyIniDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeyIniDialog(QWidget *parent = 0);
    ~KeyIniDialog();

protected:
    void keyPressEvent(QKeyEvent *e);
    //bool event(QEvent *event);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::KeyIniDialog *ui;
};

#endif // KEYINIDIALOG_H
