#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QDesktopServices>
#include <QUrl>

extern int version_now;
extern QString title;

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    //QString aboutstr;
    //aboutstr.sprintf(" ver%d.%01d.%01d",version_now%1000/100,version_now%100/10,version_now%10);
    //aboutstr = title + aboutstr;
    ui->label->setText(title);
}

AboutDialog::~AboutDialog()
{
    this->close();
    delete ui;
}

void AboutDialog::on_commandLinkButton_2_clicked()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("http://3gby.skysidestudio.com/")));
}

void AboutDialog::on_commandLinkButton_clicked()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("http://www.skysidestudio.com/")));
}
