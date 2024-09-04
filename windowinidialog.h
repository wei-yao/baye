#ifndef WINDOWINIDIALOG_H
#define WINDOWINIDIALOG_H

#include <QDialog>

namespace Ui {
class WindowIniDialog;
}

class WindowIniDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WindowIniDialog(QWidget *parent = 0);
    ~WindowIniDialog();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_buttonBox_accepted();

private:
    Ui::WindowIniDialog *ui;
};

#endif // WINDOWINIDIALOG_H
