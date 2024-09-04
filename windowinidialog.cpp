#include "windowinidialog.h"
#include "ui_windowinidialog.h"
#include <QSettings>
#include <QMessageBox>

WindowIniDialog::WindowIniDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowIniDialog)
{
    ui->setupUi(this);
}

WindowIniDialog::~WindowIniDialog()
{
    this->close();
    delete ui;
}

void WindowIniDialog::on_lineEdit_textChanged(const QString &arg1)
{
    QString s_width = ui->lineEdit->text();
    bool ok;
    int height = s_width.toInt(&ok,10)*3/4;
    ui->lineEdit_2->setText(QString::number(height, 10));
}

void WindowIniDialog::on_buttonBox_accepted()
{
    QSettings *configIniStartUp = new QSettings("startup.ini", QSettings::IniFormat);
    configIniStartUp->setIniCodec("GB2312");

    QString s_width = ui->lineEdit->text();
    QString s_height = ui->lineEdit_2->text();
    if(ui->radioButton->isChecked())
    {
        configIniStartUp->setValue("/Startup/Screen",QVariant(0));
    }
    else
    {
        configIniStartUp->setValue("/Startup/Screen",QVariant(1));
    }

    configIniStartUp->setValue("/Startup/Width",QVariant(s_width));
    configIniStartUp->setValue("/Startup/Height",QVariant(s_height));
    delete configIniStartUp;

    QMessageBox::about(this, "提示", "设置成功，请重新启动游戏");
}
