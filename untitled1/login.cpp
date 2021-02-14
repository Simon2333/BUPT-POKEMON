#include "define.h"
#include "authentication.h"
#include "signup.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>
#include <QMainWindow>
#include <QPainter>
#include<QBitmap>
#include<QMouseEvent>
#include <QMessageBox>
#include <QtNetwork/QtNetwork>
#include "login.h"
#include "ui_login.h"
login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() );
    this->setAttribute(Qt::WA_TranslucentBackground);
    client = new QTcpSocket(this);
    InitConnect();
    InitUi();
}
void login::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.drawPixmap(0, 0, QPixmap(":/picture/login.png"));
}
login::~login()
{
    delete ui;
}
void login::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons()&Qt::LeftButton)
    {
        this->move(e->globalPos()-p);
    }
}
void login::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton)
    {
        close();
    }
    else if(e->button()==Qt::LeftButton)
    {
        //求相对坐标
        //当前点击坐标-窗口左上角坐标（矩形区域左上角的坐标）
        p=e->globalPos()-this->frameGeometry().topLeft();//frameGeometry()的说明:geometry of the widget relative to its parent including any window frame
    }

}
void login::InitUi()
{
    ui->passwordText->setEchoMode(QLineEdit::Password);
    QPixmap pixmap(":/picture/btn1.png");
    /*用于隐藏toolbutton的边框*/
    ui->logonBtn->setStyleSheet("QToolButton{border:0px;}");
    /*调整按钮大小以适应图片的尺寸*/
    ui->logonBtn->resize(pixmap.size());
    ui->logonBtn->setIconSize(pixmap.size());
    ui->logonBtn->setIcon(QIcon(pixmap));
    /*通过掩码设置按钮形状以适应图片的形状*/
    ui->logonBtn->setMask(pixmap.mask());

    QPixmap pixmap2(":/picture/btn2.png");
    /*用于隐藏toolbutton的边框*/
    ui->signupBtn->setStyleSheet("QToolButton{border:0px;}");
    /*调整按钮大小以适应图片的尺寸*/
    ui->signupBtn->resize(pixmap2.size());
    ui->signupBtn->setIconSize(pixmap2.size());
    ui->signupBtn->setIcon(QIcon(pixmap2));
    /*通过掩码设置按钮形状以适应图片的形状*/
    ui->signupBtn->setMask(pixmap2.mask());
    ui->passwordText->setStyleSheet("background:transparent");
    ui->accountText->setStyleSheet("background:transparent");
}
void login::InitConnect()
{
    // 触发登陆
    connect(client, &QTcpSocket::readyRead, this, &login::getServerMsg);
    connect(ui->logonBtn, SIGNAL(clicked()), this, SLOT(Login()));
    // 触发注册
    connect(ui->signupBtn, SIGNAL(clicked()), this, SLOT(Signup()));

}
void login::Login()
{
    if (!isValidUsername(ui->accountText->text()))
    {
        QMessageBox::warning(this, tr("错误"), tr("用户名不合法"));
        ui->accountText->clear();
        ui->passwordText->clear();
        return;
    }
    if (!isValidPassword(ui->passwordText->text()))
    {
        QMessageBox::warning(this, tr("错误"), tr("密码不合法"));
        ui->passwordText->clear();
        return;
    }
    client->connectToHost(QHostAddress("127.0.0.1"), 7500);
    QString msg = "login";
    msg += ' ';
    msg += ui->accountText->text();
    msg += ' ';
    msg += ui->passwordText->text();

    if (client->write(msg.toLocal8Bit(), BUF_LENGTH) == -1)
    {
        // error
        QMessageBox::warning(this, tr("错误"), tr("服务器错误"));
        ui->logonBtn->setDisabled(false);
    }
    ui->passwordText->clear();
}
void login::Signup()
{
    signup *up=new signup;
    up->show();

}
int login::GetPort()
{
    return port;
}
void login::getServerMsg()
{

    auto ret = client->read(BUF_LENGTH);
    client->disconnectFromHost();

    QString msg = QString::fromLocal8Bit(ret);
    port = msg.toInt();
    if (port == 0)
    {
        // login failed
        QMessageBox::warning(this, tr("错误"), msg);
    }
    else
    {
        // success

        username = ui->accountText->text();
        /*client->connectToHost(QHostAddress("127.0.0.1"), port);*/
        MainWindow *s=new MainWindow(port);
        s->show();
        this->close();
    }
}
