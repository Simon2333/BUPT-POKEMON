#include "signup.h"
#include "ui_signup.h"
#include <QHostAddress>
#include "define.h"
#include "authentication.h"
#include <QTcpSocket>
#include <QtNetwork/QtNetwork>
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
signup::signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signup)
{
    ui->setupUi(this);
    client = new QTcpSocket(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() );
    this->setAttribute(Qt::WA_TranslucentBackground);
    InitConnect();
    InitUi();
}

signup::~signup()
{
    delete ui;
}
void signup::InitUi()
{
    ui->passwordText->setEchoMode(QLineEdit::Password);
    ui->againText->setEchoMode(QLineEdit::Password);
    ui->confirmBtn->setFlat(true);
    ui->backBtn->setFlat(true);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);
}
void signup::InitConnect()
{
     connect(ui->confirmBtn, SIGNAL(clicked()), this, SLOT(signUp()));
     connect(ui->backBtn,SIGNAL(clicked()),this,SLOT(shut()));
     connect(client, &QTcpSocket::readyRead, this, &signup::readServerMsg);
}
void signup::shut()
{
    this->close();
}
void signup::signUp()
{
    if (!isValidUsername(ui->accountText->text()))
    {
        QMessageBox::warning(this, tr("不合法的用户名"), tr("账号必需由字母、数字、下划线组成且长度介于6-30"));
        return;
    }
    else if (!isValidPassword(ui->passwordText->text()))
    {
        QMessageBox::warning(this, tr("不合法的密码"), tr("密码必需由字母、数字、下划线组成且长度介于6-30"));
        return;
    }
    else if (ui->passwordText->text() != ui->againText->text())
    {
        QMessageBox::warning(this, tr("错误"), tr("两次输入密码不一致"));
        return;
    }

    client->connectToHost(QHostAddress("127.0.0.1"), 7500);

    QString msg = "logon";
    msg += ' ';
    msg += ui->accountText->text();
    msg += ' ';
    msg += ui->passwordText->text();

    if (client->write(msg.toLocal8Bit(), BUF_LENGTH) == -1)
    {
        // error occur
        QMessageBox::warning(this, tr("错误"), tr("服务器出错"));
        this->close();
    }
}

void signup::readServerMsg()
{
    auto ret = client->read(BUF_LENGTH);

    client->disconnectFromHost();

    if (QString(ret) == "Accept.\n")
    {
        QMessageBox::information(this, tr("注册成功"), tr("注册成功"));
        this->close();
     // back to mainwindow
    }
    else
    {
        QMessageBox::warning(this, tr("错误"), QString::fromLocal8Bit(ret));
    }
}
