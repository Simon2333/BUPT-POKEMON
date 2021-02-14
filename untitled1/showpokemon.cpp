#include "showpokemon.h"
#include "ui_showpokemon.h"
#include <QtNetwork/QtNetwork>
#include "login.h"
#include "define.h"
#include "mainwindow.h"
#include <QMessageBox>
showpokemon::showpokemon(QTcpSocket *clients,int port,QString msg,bool self,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showpokemon)
{
    this->msg=msg;
    this->port=port;
    this->clients=clients;
    wheherself=self;
    client=new QTcpSocket(this);
    ui->setupUi(this);
    InitUi(msg);
    connect(client, &QTcpSocket::readyRead, this, &showpokemon::getServerMsg);
    connect(ui->changeBtn,&QPushButton::clicked,this, [this] {
        ui->label->show();
        ui->nameEdit->show();
        ui->confirmBtn->show();
    });
    connect(ui->confirmBtn,SIGNAL(clicked()),this, SLOT(ChangeName()));
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background,QColor(255,255,255));
    this->setPalette(palette);
    ui->pictureLabel->setStyleSheet("border:2px groove #242424;");
}

showpokemon::~showpokemon()
{
    delete ui;
}
void showpokemon::ChangeName()
{

    clients->disconnectFromHost();
    client->connectToHost(QHostAddress("127.0.0.1"),port);
    ui->label->hide();
    ui->nameEdit->hide();
    ui->confirmBtn->hide();
    auto detail=msg.split(' ');
    QString str = "pokemonChangeName ";
    str+=detail[0];
    str+=" ";
    str+=ui->nameEdit->text();
    client->write(str.toLocal8Bit(), BUF_LENGTH);
}
QString showpokemon::changerace(int race)
{
    switch(race)
    {
        case 0:
        return "Charmander";break;
        case 1:
        return "Bullbasaur";break;
        case 2:
        return "Squirtle";break;
        default:break;
    }
    return 0;
}
QString showpokemon::changetype(int type)
{
    switch(type)
    {
        case 0:
        return "ATK";break;
        case 1:
        return "HP";break;
        case 2:
        return "DEF";break;
        case 3:
        return "SPE";break;
        default:break;
    }
    return 0;
}
QString showpokemon::changeele(int element)
{
    switch(element)
    {
        case 0:
        return "Fire";break;
        case 1:
        return "Ice";break;
        case 2:
        return "Grass";break;
        case 3:
        return "Water";break;
        case 4:
        return "Electron";break;
        default:break;
    }
    return 0;
}
void showpokemon::InitUi(QString msg)
{
    if(!wheherself)
    {
        ui->changeBtn->hide();
    }
    auto detail=msg.split(' ');
    QPixmap pic(":/picture/"+detail[2]+".png");
    ui->pictureLabel->setPixmap(pic.scaled(151, 151, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setWindowIcon(QIcon(":/picture/"+detail[2]+".png"));
    setWindowTitle("详细属性");
    ui->pictureLabel->show();
    ui->label->hide();
    ui->nameEdit->hide();
    ui->confirmBtn->hide();
    ui->nameLabe->setText(detail[1]);
    ui->idLable->setText(detail[0]);
    ui->raceLable->setText(changerace(detail[2].toInt()));
    ui->attackLable->setText(detail[3]);
    ui->defenceLable->setText(detail[4]);
    ui->hpLable->setText(detail[5]);
    ui->speedLable->setText(detail[6]);
    ui->levelLable->setText(detail[7]);
    ui->typeLable->setText(changetype(detail[9].toInt()));
    ui->elementLable->setText(changeele(detail[10].toInt()));


}
void showpokemon::getServerMsg()
{
    client->disconnectFromHost();
    clients->connectToHost(QHostAddress("127.0.0.1"),port);
    auto ret = client->read(BUF_LENGTH);

    QString msg = QString::fromLocal8Bit(ret);
    if (msg != "Accept.\n")
    {
        QMessageBox::warning(this, tr("错误"), tr("修改精灵名字失败"));
    }
    else
    {
        QMessageBox::information(this, tr("精灵修改名字"), tr("精灵名字已更新"));
        ui->nameLabe->setText(ui->nameEdit->text());
    }
}
