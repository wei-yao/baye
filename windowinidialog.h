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

#ifndef NEW_MENU_WINDOW_H
#define NEW_MENU_WINDOW_H

#include <QMainWindow>
#include <QListWidget>

class NewMenuWindow : public QMainWindow {
    Q_OBJECT

public:
    NewMenuWindow(QWidget *parent = nullptr);
    ~NewMenuWindow();

private slots:
    void onItemClicked(QListWidgetItem *item);

private:
    QListWidget *listWidget;
};

#endif // NEW_MENU_WINDOW_H

