#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QTcpSocket>
#include <QtNetwork/QtNetwork>
#include "mainwindow.h"
namespace Ui {
class signup;
}

class signup : public QWidget
{
    Q_OBJECT

public:
    explicit signup(QWidget *parent = nullptr);
    ~signup();
public slots:
    void signUp();
    void shut();
private:
    Ui::signup *ui;
    void InitUi();
    void InitConnect();
    QTcpSocket *client;
private slots:
    void readServerMsg();


};

#endif // SIGNUP_H
