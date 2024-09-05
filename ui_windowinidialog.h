/********************************************************************************
** Form generated from reading UI file 'windowinidialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOWINIDIALOG_H
#define UI_WINDOWINIDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_WindowIniDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label_2;
    QGroupBox *groupBox;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;

    void setupUi(QDialog *WindowIniDialog)
    {
        if (WindowIniDialog->objectName().isEmpty())
            WindowIniDialog->setObjectName(QString::fromUtf8("WindowIniDialog"));
        WindowIniDialog->resize(332, 317);
        buttonBox = new QDialogButtonBox(WindowIniDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(50, 270, 211, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(WindowIniDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 20, 91, 31));
        lineEdit = new QLineEdit(WindowIniDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(150, 30, 113, 20));
        lineEdit_2 = new QLineEdit(WindowIniDialog);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(150, 90, 113, 20));
        lineEdit_2->setReadOnly(true);
        label_2 = new QLabel(WindowIniDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 80, 101, 41));
        groupBox = new QGroupBox(WindowIniDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(50, 150, 241, 81));
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(30, 30, 89, 31));
        radioButton->setChecked(true);
        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(140, 30, 89, 31));

        retranslateUi(WindowIniDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), WindowIniDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), WindowIniDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(WindowIniDialog);
    } // setupUi

    void retranslateUi(QDialog *WindowIniDialog)
    {
        WindowIniDialog->setWindowTitle(QCoreApplication::translate("WindowIniDialog", "\347\252\227\345\217\243\351\205\215\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("WindowIniDialog", "\347\252\227\345\217\243\345\256\275\345\272\246", nullptr));
        lineEdit->setText(QCoreApplication::translate("WindowIniDialog", "800", nullptr));
        lineEdit_2->setText(QCoreApplication::translate("WindowIniDialog", "600", nullptr));
        label_2->setText(QCoreApplication::translate("WindowIniDialog", "\347\252\227\345\217\243\351\253\230\345\272\246", nullptr));
        groupBox->setTitle(QCoreApplication::translate("WindowIniDialog", "\346\230\276\347\244\272\346\250\241\345\274\217", nullptr));
        radioButton->setText(QCoreApplication::translate("WindowIniDialog", "\347\252\227\345\217\243", nullptr));
        radioButton_2->setText(QCoreApplication::translate("WindowIniDialog", "\345\205\250\345\261\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WindowIniDialog: public Ui_WindowIniDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWINIDIALOG_H
