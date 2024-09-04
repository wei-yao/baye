#include "keyinidialog.h"
#include "ui_keyinidialog.h"
#include <QKeyEvent>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
//QString str[146]={"","","","","","","","","BackSpace","Tab","","","","Return","","","Shift","Ctrl","Alt","Pause","CapsLock","","","","","","","Esc","","","","","Space","PageUp","PageDown","End","Home","LeftArrow","UpArrow","RightArrow","DownArrow","","*","+","'","Insert","Delete","/","0","1","2","3","4","5","6","7","8","9","",";","","=","","","","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","\[","\\","\]","","","~","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","","","","","","","","","","","","","","","","","","","","","NumLock","SckLock"};
int keyascii[10];

KeyIniDialog::KeyIniDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyIniDialog)
{
    ui->setupUi(this);
    //setFocusPolicy(Qt::ClickFocus);

    QSettings *configIniStartUp = new QSettings("startup.ini", QSettings::IniFormat);
    configIniStartUp->setIniCodec("GB2312");

    keyascii[0]=configIniStartUp->value("/Key/Return",13).toInt();
    keyascii[2]=configIniStartUp->value("/Key/Esc",27).toInt();
    keyascii[1]=configIniStartUp->value("/Key/Space",32).toInt();
    keyascii[3]=configIniStartUp->value("/Key/Home",36).toInt();
    keyascii[4]=configIniStartUp->value("/Key/Up",38).toInt();
    keyascii[7]=configIniStartUp->value("/Key/Down",40).toInt();
    keyascii[5]=configIniStartUp->value("/Key/Left",37).toInt();
    keyascii[6]=configIniStartUp->value("/Key/Right",39).toInt();
    keyascii[8]=configIniStartUp->value("/Key/PgUp",73).toInt();
    keyascii[9]=configIniStartUp->value("/Key/PgDn",81).toInt();

    ui->lineEdit->setText(QKeySequence(keyascii[0]).toString(QKeySequence::NativeText) );
    ui->lineEdit_2->setText(QKeySequence(keyascii[1]).toString(QKeySequence::NativeText) );
    ui->lineEdit_3->setText(QKeySequence(keyascii[2]).toString(QKeySequence::NativeText) );
    ui->lineEdit_4->setText(QKeySequence(keyascii[3]).toString(QKeySequence::NativeText) );
    ui->lineEdit_5->setText(QKeySequence(keyascii[4]).toString(QKeySequence::NativeText) );
    ui->lineEdit_6->setText(QKeySequence(keyascii[5]).toString(QKeySequence::NativeText) );
    ui->lineEdit_7->setText(QKeySequence(keyascii[6]).toString(QKeySequence::NativeText) );
    ui->lineEdit_8->setText(QKeySequence(keyascii[7]).toString(QKeySequence::NativeText) );
    ui->lineEdit_9->setText(QKeySequence(keyascii[8]).toString(QKeySequence::NativeText) );
    ui->lineEdit_10->setText(QKeySequence(keyascii[9]).toString(QKeySequence::NativeText) );



    int MouseEnable = configIniStartUp->value("/Startup/UseMouse",1).toInt();

    if(MouseEnable == 0)
    {
        ui->checkBox->setChecked(true);
    }
    else
    {
        ui->checkBox->setChecked(false);
    }


    delete configIniStartUp;
}

KeyIniDialog::~KeyIniDialog()
{
    this->close();
    delete ui;
}

void KeyIniDialog::on_buttonBox_accepted()
{
    QSettings *configIniStartUp = new QSettings("startup.ini", QSettings::IniFormat);
    configIniStartUp->setIniCodec("GB2312");


    if(ui->checkBox->isChecked())
    {
        configIniStartUp->setValue("/Startup/UseMouse",QVariant(1));
    }
    else
    {
        configIniStartUp->setValue("/Startup/UseMouse",QVariant(0));
    }

    configIniStartUp->setValue("/Key/Return",QVariant(keyascii[0]));
    configIniStartUp->setValue("/Key/Space",QVariant(keyascii[1]));
    configIniStartUp->setValue("/Key/Esc",QVariant(keyascii[2]));
    configIniStartUp->setValue("/Key/Home",QVariant(keyascii[3]));
    configIniStartUp->setValue("/Key/Up",QVariant(keyascii[4]));
    configIniStartUp->setValue("/Key/Left",QVariant(keyascii[5]));
    configIniStartUp->setValue("/Key/Right",QVariant(keyascii[6]));
    configIniStartUp->setValue("/Key/Down",QVariant(keyascii[7]));
    configIniStartUp->setValue("/Key/PgUp",QVariant(keyascii[8]));
    configIniStartUp->setValue("/Key/PgDn",QVariant(keyascii[9]));
    delete configIniStartUp;

    QMessageBox::about(this, "提示", "设置成功，请重新启动游戏");
}

void KeyIniDialog::keyPressEvent(QKeyEvent *e)
{
    QWidget *current_focus_widget;
    current_focus_widget = QApplication::focusWidget();
    if (current_focus_widget->inherits("QLineEdit"))
    {
        //int keyCode = e->key();
        QLineEdit *le= qobject_cast<QLineEdit*>(current_focus_widget);
        //le->setText(str[e->key()] );
        le->setText(QKeySequence(e->key()).toString(QKeySequence::NativeText) );
        if(le->objectName()=="lineEdit")
        {
            keyascii[0]= e->key();
        }
        else if(le->objectName()=="lineEdit_2")
        {
            keyascii[1]= e->key();
        }
        else if(le->objectName()=="lineEdit_3")
        {
            keyascii[2]= e->key();
        }
        else if(le->objectName()=="lineEdit_4")
        {
            keyascii[3]= e->key();
        }
        else if(le->objectName()=="lineEdit_5")
        {
            keyascii[4]= e->key();
        }
        else if(le->objectName()=="lineEdit_6")
        {
            keyascii[5]= e->key();
        }
        else if(le->objectName()=="lineEdit_7")
        {
            keyascii[6]= e->key();
        }
        else if(le->objectName()=="lineEdit_8")
        {
            keyascii[7]= e->key();
        }
        else if(le->objectName()=="lineEdit_9")
        {
            keyascii[8]= e->key();
        }
        else if(le->objectName()=="lineEdit_10")
        {
            keyascii[9]= e->key();
        }
        //qDebug() << "press key's content" << QKeySequence(e->modifiers() + e->key());
    }
}


/*bool KeyIniDialog::event(QEvent *event)
 {
     if (event->type() == QEvent::KeyPress)
     {
         QKeyEvent *ke = static_cast<QKeyEvent*>(event);
         if (ke->key() == Qt::Key_Tab||ke->key() == Qt::Key_Left||ke->key() == Qt::Key_Right)
         {
             return true;
         }
     }
     return QWidget::event(event);

 }*/
