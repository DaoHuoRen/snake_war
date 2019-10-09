/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_client
{
public:

    void setupUi(QDialog *client)
    {
        if (client->objectName().isEmpty())
            client->setObjectName(QString::fromUtf8("client"));
        client->resize(800, 600);

        retranslateUi(client);

        QMetaObject::connectSlotsByName(client);
    } // setupUi

    void retranslateUi(QDialog *client)
    {
        client->setWindowTitle(QCoreApplication::translate("client", "\350\264\252\345\220\203\350\233\207\345\244\247\346\210\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class client: public Ui_client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
