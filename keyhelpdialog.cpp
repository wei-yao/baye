#include "keyhelpdialog.h"
#include "ui_keyhelpdialog.h"

KeyHelpDialog::KeyHelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyHelpDialog)
{
    ui->setupUi(this);
}

KeyHelpDialog::~KeyHelpDialog()
{
    delete ui;
}
