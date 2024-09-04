#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

#include "tcpdatastruct.h"

extern int version_now;
extern Player g_player;

//extern UT ShareHand();

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);

}

LoginDialog::~LoginDialog()
{
    this->close();
    delete ui;
}

void LoginDialog::on_buttonBox_accepted()
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    QByteArray ba = username.toLocal8Bit();
    memset(g_player.playerName,0,30);
    memcpy(g_player.playerName,ba.data(),30);

    ba = password.toLocal8Bit();
    memset(g_player.password,0,64);

    char sha256[100];
    StrSHA256(ba.data(),strlen(ba.data()),sha256);

    memcpy(g_player.password,sha256,64);

    strcpy((char *)g_player.UUID,"CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");//未登录前为32位C

    if(TCPisConnected()==OK)
    {

    }
    else
    {
        NewTCPClient();
        TCPconnectToServer();
    }

    ShareHand();//ClientSndMsg(TRANS_SHAREHAND,data,0);
    C data[65535];
    if(ClientRcvMsg(TRANS_SHAREHAND,data,1)>0)
    {
        if(data[0] == 0x00)
        {
            char UUID[32+1];
            memcpy(UUID,data+1,32);
            memcpy(g_player.UUID,data+1,32);
            g_player.logined = 1;
            QMessageBox::about(NULL,"提示","登录成功");
        }
        else
        {
            QMessageBox::about(NULL,"提示","登录失败");
        }
    }


}
