/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QCommandLinkButton *commandLinkButton;
    QCommandLinkButton *commandLinkButton_2;
    QLabel *label_5;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->resize(513, 276);
        buttonBox = new QDialogButtonBox(AboutDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(70, 220, 401, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(AboutDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(170, 50, 301, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font.setPointSize(12);
        label->setFont(font);
        label_2 = new QLabel(AboutDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(170, 80, 301, 31));
        label_2->setFont(font);
        label_3 = new QLabel(AboutDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 140, 161, 31));
        label_3->setFont(font);
        label_4 = new QLabel(AboutDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 180, 151, 21));
        label_4->setFont(font);
        commandLinkButton = new QCommandLinkButton(AboutDialog);
        commandLinkButton->setObjectName(QString::fromUtf8("commandLinkButton"));
        commandLinkButton->setGeometry(QRect(180, 130, 321, 41));
        commandLinkButton_2 = new QCommandLinkButton(AboutDialog);
        commandLinkButton_2->setObjectName(QString::fromUtf8("commandLinkButton_2"));
        commandLinkButton_2->setGeometry(QRect(180, 170, 321, 41));
        label_5 = new QLabel(AboutDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(50, 50, 71, 71));
        label_5->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));

        retranslateUi(AboutDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AboutDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AboutDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QCoreApplication::translate("AboutDialog", "\345\205\263\344\272\216", nullptr));
        label->setText(QCoreApplication::translate("AboutDialog", "\345\244\251\351\231\205\350\276\271\344\270\211\345\233\275\351\234\270\344\270\232 ver3.0.0", nullptr));
        label_2->setText(QCoreApplication::translate("AboutDialog", "\345\244\251\351\231\205\350\276\271\345\267\245\344\275\234\345\256\244 \350\215\243\350\252\211\345\207\272\345\223\201", nullptr));
        label_3->setText(QCoreApplication::translate("AboutDialog", "\345\244\251\351\231\205\350\276\271\345\267\245\344\275\234\345\256\244", nullptr));
        label_4->setText(QCoreApplication::translate("AboutDialog", "\345\256\230\346\226\271\347\275\221\347\253\231", nullptr));
        commandLinkButton->setText(QCoreApplication::translate("AboutDialog", "http://www.skysidestudio.com", nullptr));
        commandLinkButton_2->setText(QCoreApplication::translate("AboutDialog", "http://3gby.skysidestudio.com", nullptr));
        label_5->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
