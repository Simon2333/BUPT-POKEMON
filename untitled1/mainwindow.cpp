#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "define.h"
#include "authentication.h"
#include "login.h"
#include "signup.h"
#include "showpokemon.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QLCDNumber>
#include <QTableWidget>
#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QtNetwork/QtNetwork>
#include <QMovie>

MainWindow::MainWindow(int port,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setFixedSize(1280,720);
    this->port=port;
    client = new QTcpSocket(this);
    client->connectToHost(QHostAddress("127.0.0.1"),port);
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);  //新建QNetworkAccessManager对象
    InitUi();

    InitConnect();
    timer=new QTimer(this);
    timer->start();
    /*timer->setInterval(1000);*/
    connect(timer,SIGNAL(timeout()),this,SLOT(OnTimeOut()));
    ChangeToMain();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitUi()
{
    ui->oldPasswordText->setEchoMode(QLineEdit::Password);
    ui->newPasswordText->setEchoMode(QLineEdit::Password);
    ui->againText->setEchoMode(QLineEdit::Password);
    ui->attackBtn->setEnabled(false);
    ui->attackBtn_2->setEnabled(false);
    ui->attackBtn_3->setEnabled(false);
    ui->attackBtn_4->setEnabled(false);
    ui->discardBtn->hide();

}
void MainWindow::InitConnect()
{
    connect(client, &QTcpSocket::readyRead, this, &MainWindow::getServerMsg);
    // 触发
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//关联信号和槽
    connect(ui->changeBtn,SIGNAL(clicked()),this,SLOT(ChangeGif()));
    connect(ui->discardBtn,SIGNAL(clicked()),this,SLOT(Discard()));
    connect(ui->myBagBtn,SIGNAL(clicked()),this,SLOT(ChangeToBag()));
    connect(ui->userListBtn,SIGNAL(clicked()),this,SLOT(ChangeToList()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(ChangeToLogin()));
    connect(ui->changePasswordBtn,SIGNAL(clicked()),this,SLOT(ChangeToPass()));
    connect(ui->backBtn,SIGNAL(clicked()),this,SLOT(ChangeToMain()));
    connect(ui->backBtn_2,SIGNAL(clicked()),this,SLOT(ChangeToMain()));
    connect(ui->confirmpassBtn,SIGNAL(clicked()),this,SLOT(ChangePassword()));
    connect(ui->gobackBtn,SIGNAL(clicked()),this,SLOT(ChangeToMain()));
    connect(ui->fightBtn,SIGNAL(clicked()),this,SLOT(ChangeToFight()));
    connect(ui->backBtn_3,SIGNAL(clicked()),this,SLOT(ChangeToMain()));
    connect(ui->levelupBtn,SIGNAL(clicked()),this,SLOT(ChooseLvupBet()));
    connect(ui->duelBtn,SIGNAL(clicked()),this,SLOT(ChooseDuelBet()));
    connect(ui->startBtn,SIGNAL(clicked()),this,SLOT(StartFight()));
    connect(ui->attackBtn,&QPushButton::clicked,this, [this] {
        skillIndex=0;
        ui->attackBtn->setEnabled(false);
        ui->attackBtn_2->setEnabled(false);
        ui->attackBtn_3->setEnabled(false);
        ui->attackBtn_4->setEnabled(false);
        QString s=QString::number(skillIndex);
        client->write(s.toLocal8Bit(),BUF_LENGTH);
    });
    connect(ui->attackBtn_2,&QPushButton::clicked,this, [this] {
        skillIndex=1;
        ui->attackBtn->setEnabled(false);
        ui->attackBtn_2->setEnabled(false);
        ui->attackBtn_3->setEnabled(false);
        ui->attackBtn_4->setEnabled(false);
        QString s=QString::number(skillIndex);
        client->write(s.toLocal8Bit(),BUF_LENGTH);
    });
    connect(ui->attackBtn_3,&QPushButton::clicked,this, [this] {
        skillIndex=2;
        ui->attackBtn->setEnabled(false);
        ui->attackBtn_2->setEnabled(false);
        ui->attackBtn_3->setEnabled(false);
        ui->attackBtn_4->setEnabled(false);
        QString s=QString::number(skillIndex);
        client->write(s.toLocal8Bit(),BUF_LENGTH);
    });
    connect(ui->attackBtn_4,&QPushButton::clicked,this, [this] {
        skillIndex=3;
        ui->attackBtn->setEnabled(false);
        ui->attackBtn_2->setEnabled(false);
        ui->attackBtn_3->setEnabled(false);
        ui->attackBtn_4->setEnabled(false);
        QString s=QString::number(skillIndex);
        client->write(s.toLocal8Bit(),BUF_LENGTH);
    });

}
void MainWindow::ChangeGif()
{
    QMovie *movie=new QMovie(QString::fromStdString(":/picture/dynamic/bulbasaur .gif"));
    /*movie->setScaledSize(QSize(1000,1000));*/
    ui->op1->setMovie(movie);
               ui->op1->movie()->start();
               ui->op1->setGeometry(-200, -500, 1000,1000);
}
void MainWindow::OnTimeOut()
{
    dateTime=QDateTime::currentDateTime();
    ui->day->setText(dateTime.toString("yyyy/MM/dd"));
    ui->time->setText(dateTime.toString("hh:mm:ss"));

}
void MainWindow::StartFight()
{
    if(choose1&&choose2)
    {
        duringFight=true;
        QString buf;
        buf+="battle ";
        if(whichFight==DUEL)
        {
            buf+="DUEL ";
            buf+=QString::number(yourNum,10);
            buf+=" ";
            buf+=QString::number(enemyNum,10);
        }
        else if(whichFight==LEVELUP)
        {
            buf+="LVUP ";
            buf+=QString::number(yourNum,10);
            buf+=" ";
            buf+=QString::number(enemyNum,10);
        }
        ChangeToFightRoom();
        client->write(buf.toLocal8Bit(),BUF_LENGTH);
    }
    else if (!choose1)
    {
        QMessageBox::warning(this, tr("错误"), tr("请选择要对战的精灵"));
    }
    else
    {
        QMessageBox::warning(this, tr("错误"), tr("请选择要出战的精灵"));
    }

}
void MainWindow::ChangeToFightRoom()
{
    ui->userlistPage->setFixedSize(1280, 720);

    // 设置背景
    ui->userlistPage->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/picture/281831.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);
    ui->stackedWidget->setCurrentWidget(ui->fightroomPage);
}
void MainWindow::ChooseDuelBet()
{
    client->write("getPokemonList 1", BUF_LENGTH);
    whichFight=DUEL;
}
void MainWindow::ChooseLvupBet()
{
    client->write("getPokemonList 2", BUF_LENGTH);
    whichFight=LEVELUP;
}
void MainWindow::ChangeToFight()
{
    ui->userlistPage->setFixedSize(1280, 720);
    choose1=false;
    choose2=false;
    // 设置背景
    ui->userlistPage->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/picture/fight.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);
    draw=false;
    ui->stackedWidget->setCurrentWidget(ui->battlePage);
    ui->bagListArea_3->setStyleSheet("background-color: rgb(255, 233, 140);");
    ui->bagListArea_4->setStyleSheet("background-color: rgb(255, 233, 140);");
}
void MainWindow::ChangeToPass()
{
    ui->stackedWidget->setCurrentWidget(ui->passPage);
}
void MainWindow::ChangeToMain()
{

    CheckW();
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background,QColor(255,240,216));
    this->setPalette(palette);
    QMovie *movie=new QMovie(QString::fromStdString(":/picture/dynamic/tyrantrum.gif"));
    /*movie->setScaledSize(QSize(1000,1000));*/
    ui->op1->setMovie(movie);
               ui->op1->movie()->start();
               ui->op1->setGeometry(100, 0, 500,500);
    // 设置背景
   /* ui->userlistPage->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap("://picture/VGN94976413.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);*/
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}
void MainWindow::ChangeToChoose()
{
    ui->userlistPage->setFixedSize(1280, 720);

    // 设置背景
    ui->userlistPage->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/picture/discard.png");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);
    ui->stackedWidget->setCurrentWidget(ui->choosePage);
}
void MainWindow::ChangeToBag()
{
    ui->userlistPage->setFixedSize(1280, 720);

    // 设置背景
    ui->userlistPage->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap("://picture/bag.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);
    client->write("getPokemonList", BUF_LENGTH);
    ui->stackedWidget->setCurrentWidget(ui->packagePage);
}
void MainWindow::ChangeToList()
{
    ui->userlistPage->setFixedSize(1280, 720);

    // 设置背景
    ui->userlistPage->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap("://picture/userlist.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
    setPalette(palette);
    client->write("getPlayerList", BUF_LENGTH);
    ui->stackedWidget->setCurrentWidget(ui->userlistPage);
}
void MainWindow::ChangeToLogin()
{
    client->disconnectFromHost();
    login *log=new login;
    log->show();
    this->close();

}
void MainWindow::ShowPokemonList(QString msg)
{
    auto pokemon=msg.split('\n');

    QWidget *containWidget = new QWidget(ui->bagListArea);
    containWidget->setStyleSheet("background-color: rgb(141, 220, 163);");
    QGridLayout *gridLayout = new QGridLayout(containWidget);
    containWidget->setLayout(gridLayout);

    // 将背包里的每个小精灵加载到gridlayout中
    int row = 1, col = 1;
    pokemonNum=pokemon.size()-1;
    lv15Num=0;
    for(int i=0;i<pokemon.size()-1;i++)
    {
        auto detail=pokemon[i].split(' ');
        //设置小精灵图片
        QPushButton *pokemonBtn =new QPushButton(this);
        pokemonBtn->setFixedSize(110,110);
        pokemonBtn->setIconSize(QSize(110,110));
        pokemonBtn->setIcon(QIcon("://picture/"+detail[2]+".png"));
        pokemonBtn->setFlat(true);
        //设置等级
        connect(pokemonBtn,&QPushButton::clicked,this, [this, detail] {
            QString str = "getPokemon ";
            str += detail[0];
            client->write(str.toStdString().c_str(), BUF_LENGTH);
            showPokemonDetail = true;
        });
        QLabel *textLabel = new QLabel();
        textLabel->setText(QString::fromStdString("Lv.")+detail[3]);
        if(detail[3].toInt()==15)
        {
            lv15Num++;
        }
        textLabel->setStyleSheet("font: 75 13pt \"Arial Black\"");
        textLabel->setAlignment(Qt::AlignCenter);
        //
        QVBoxLayout *rowlayout = new QVBoxLayout();

        rowlayout->addWidget(pokemonBtn);
        rowlayout->addWidget(textLabel);
        gridLayout->addLayout(rowlayout, row, col);
        if (++col > 3)
        {
            row++;
            col = 1;
        }
    }
    if (row == 1 && col != 1)
    {
        for (int i = col; i <= 3; i++)
        {
            QVBoxLayout *rowlayout = new QVBoxLayout();
            QLabel *picLabel = new QLabel();
            picLabel->resize(120, 120);
            rowlayout->addWidget(picLabel);
            gridLayout->addLayout(rowlayout, 1, i);
        }
    }
    gridLayout->setAlignment(Qt::AlignTop);
    ui->bagListArea->setWidget(containWidget);
    showuser=true;
    client->write("getDuelStatistic",BUF_LENGTH);
}
void MainWindow::ShowUserList(QString msg)
{
    auto players = msg.split('\n');
    QWidget *containWidget = new QWidget(ui->userListArea_6);
    containWidget->setStyleSheet("background-color: rgb(255, 175, 77);");
    QWidget *containWidget1 = new QWidget(ui->bagListArea_7);
    containWidget1->setStyleSheet("background-color: rgb(255, 175, 77);");
    QVBoxLayout *listLayout = new QVBoxLayout();
    listLayout->setAlignment(Qt::AlignTop);
    containWidget->setLayout(listLayout);

    _signalMapper = new QSignalMapper(containWidget);
    for (int i = 0; i < players.size() - 1; ++i)
    {
        auto p=players[i];
        auto detail=p.split(' ');
        if(detail[2]=='1')
        {
            // 显示用户状态
            QLabel *stateLabel = new QLabel();
            stateLabel->setPixmap(QPixmap("://picture/online.png")
                                  .scaled(36, 36, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            // 显示用户名
            QLabel *textLabel = new QLabel();
            textLabel->setText(detail[1]);
            textLabel->setStyleSheet("font: 75 13pt \"Arial Black\"");

            QPushButton *bagButton = new QPushButton();
            bagButton->setIcon(QPixmap("://picture/ball.png"));
            bagButton->setFlat(true);

            connect(bagButton,&QPushButton::clicked,this, [this, detail] {
                QString str = "getPokemonList ";
                str += detail[0];
                client->write(str.toStdString().c_str(), BUF_LENGTH);
                showbag = true;
            });


            // 构建显示条目的layout
            QHBoxLayout *rowlayout = new QHBoxLayout;
            rowlayout->addSpacing(5);
            rowlayout->addWidget(stateLabel);
            rowlayout->addStretch(1);
            rowlayout->addWidget(textLabel);
            rowlayout->addStretch(1);
            rowlayout->addWidget(bagButton);
            rowlayout->addSpacing(5);
            listLayout->addLayout(rowlayout);
        }

    }
    for (int i = 0; i < players.size() - 1; ++i)
    {
        auto p=players[i];
        auto detail=p.split(' ');
        if(detail[2]=='0')
        {
            // 显示用户状态
            QLabel *stateLabel = new QLabel();
            stateLabel->setPixmap(QPixmap("://picture/offline.png")
                                  .scaled(36, 36, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            // 显示用户名
            QLabel *textLabel = new QLabel();
            textLabel->setText(detail[1]);
            textLabel->setStyleSheet("font: 75 13pt \"Arial Black\"");

            QPushButton *bagButton = new QPushButton();
            bagButton->setIcon(QPixmap("://picture/ball.png"));
            bagButton->setFlat(true);

            connect(bagButton,&QPushButton::clicked,this, [this, detail] {
                QString str = "getPokemonList ";
                str += detail[0];
                client->write(str.toStdString().c_str(), BUF_LENGTH);
                showbag = true;
            });


            // 构建显示条目的layout
            QHBoxLayout *rowlayout = new QHBoxLayout;
            rowlayout->addSpacing(5);
            rowlayout->addWidget(stateLabel);
            rowlayout->addStretch(1);
            rowlayout->addWidget(textLabel);
            rowlayout->addStretch(1);
            rowlayout->addWidget(bagButton);
            rowlayout->addSpacing(5);
            listLayout->addLayout(rowlayout);
        }

    }

    ui->userListArea_6->setWidget(containWidget);
    ui->bagListArea_7->setWidget(containWidget1);

}
void MainWindow::ShowBag(QString msg)
{
    showbag=false;
    auto pokemon=msg.split('\n');

    QWidget *containWidget = new QWidget(ui->bagListArea_7);
    containWidget->setStyleSheet("background-color: rgb(255, 175, 77);");

    QGridLayout *gridLayout = new QGridLayout(containWidget);
    containWidget->setLayout(gridLayout);
    gridLayout->setAlignment(Qt::AlignTop);

    // 将背包里的每个小精灵加载到gridlayout中
    int row = 1, col = 1;
    for(int i=0;i<pokemon.size()-1;i++)
    {
        auto detail=pokemon[i].split(' ');
        //设置小精灵图片
        QPushButton *pokemonBtn =new QPushButton(this);
        pokemonBtn->setFixedSize(110,110);
        QIcon ico("://picture/"+detail[2]+".png");
        pokemonBtn->setIconSize(QSize(110,110));
        pokemonBtn->setIcon(QIcon("://picture/"+detail[2]+".png"));
        //设置等级
        connect(pokemonBtn,&QPushButton::clicked,this, [this, detail] {
            QString str = "getPokemon ";
            str += detail[0];
            client->write(str.toStdString().c_str(), BUF_LENGTH);
            showPokemonDetail = true;
        });
        pokemonBtn->setFlat(true);
        QLabel *textLabel = new QLabel();
        textLabel->setText(QString::fromStdString("Lv.")+detail[3]);
        textLabel->setStyleSheet("font: 75 13pt \"Arial Black\"");
        textLabel->setAlignment(Qt::AlignCenter);
        //
        QVBoxLayout *rowlayout = new QVBoxLayout();

        rowlayout->addWidget(pokemonBtn);
        rowlayout->addWidget(textLabel);
        gridLayout->addLayout(rowlayout, row, col);
        if (++col > 3)
        {
            row++;
            col = 1;
        }
    }
    // 用空layout填充第一行的空位置
    if (row == 1 && col != 1)
    {
        for (int i = col; i <= 3; i++)
        {
            QVBoxLayout *rowlayout = new QVBoxLayout();
            QLabel *picLabel = new QLabel();
            picLabel->resize(120, 120);
            rowlayout->addWidget(picLabel);
            gridLayout->addLayout(rowlayout, 1, i);
        }
    }
    gridLayout->setAlignment(Qt::AlignTop);
    ui->bagListArea_7->setWidget(containWidget);}
void MainWindow::ChangePassword()
{
    if (!isValidPassword(ui->oldPasswordText->text()))
    {
        QMessageBox::warning(this, tr("错误"), tr("旧密码格式错误"));
        ui->oldPasswordText->clear();
    }
    else if (!isValidPassword(ui->newPasswordText->text()))
    {
        QMessageBox::warning(this, tr("错误"), tr("新密码格式错误"));
        ui->newPasswordText->clear();
    }
    else if(ui->againText->text()!=ui->newPasswordText->text())
    {
        QMessageBox::warning(this, tr("错误"), tr("两次输入密码不同"));
        ui->newPasswordText->clear();
        ui->againText->clear();
    }
    else
    {
        QString str = "resetPassword ";
        str += ui->oldPasswordText->text();
        str += ' ';
        str += ui->newPasswordText->text();
        client->write(str.toStdString().c_str(), BUF_LENGTH);
    }
}
void MainWindow::ShowYourPokem(QString msg)
{
    showFightlist=false;
    choose1=false;
    auto pokemon=msg.split('\n');

    QWidget *containWidget = new QWidget(ui->bagListArea_4);
    containWidget->setStyleSheet("background-color: rgb(255, 233, 140);");

    QGridLayout *gridLayout = new QGridLayout(containWidget);
    containWidget->setLayout(gridLayout);
    gridLayout->setAlignment(Qt::AlignTop);

    // 将背包里的每个小精灵加载到gridlayout中
    int row = 1, col = 1;
    for(int i=0;i<pokemon.size()-1;i++)
    {
        auto detail=pokemon[i].split(' ');
        //设置小精灵图片
        QPushButton *pokemonBtn =new QPushButton(this);
        pokemonBtn->setFixedSize(110,110);
        pokemonBtn->setFlat(true);
        QIcon ico("://picture/"+detail[2]+".png");
        pokemonBtn->setIconSize(QSize(110,110));
        pokemonBtn->setIcon(QIcon("://picture/"+detail[2]+".png"));
        QPushButton *selectBtn=new QPushButton(this);
        selectBtn->setText("选择此精灵");
        //设置等级
        connect(pokemonBtn,&QPushButton::clicked,this, [this, detail] {
            QString str = "getPokemon ";
            str += detail[0];
            client->write(str.toStdString().c_str(), BUF_LENGTH);
            showPokemonDetail = true;
        });
        connect(selectBtn,&QPushButton::clicked,this, [this, detail]{
            yourNum=detail[0].toInt();
            ui->yourpkm->setPixmap(QPixmap(":/picture/"+detail[2]+".png").scaled(100,100));
            choose1=true;
        });
        QLabel *textLabel = new QLabel();
        textLabel->setText(QString::fromStdString("Lv.")+detail[3]);
        textLabel->setStyleSheet("font: 75 13pt \"Arial Black\"");
        textLabel->setAlignment(Qt::AlignCenter);
        //
        QVBoxLayout *rowlayout = new QVBoxLayout();

        rowlayout->addWidget(pokemonBtn);
        rowlayout->addWidget(textLabel);
        rowlayout->addWidget(selectBtn);
        gridLayout->addLayout(rowlayout, row, col);
        if (++col > 3)
        {
            row++;
            col = 1;
        }
    }
    if (row == 1 && col != 1)
    {
        for (int i = col; i <= 3; i++)
        {
            QVBoxLayout *rowlayout = new QVBoxLayout();
            QLabel *picLabel = new QLabel();
            picLabel->resize(120, 120);
            rowlayout->addWidget(picLabel);
            gridLayout->addLayout(rowlayout, 1, i);
        }
    }
    gridLayout->setAlignment(Qt::AlignTop);
    ui->bagListArea_4->setWidget(containWidget);
    ui->startBtn->show();
}
void MainWindow::ShowFightList(QString msg)
{
    showFightlist=true;
    auto pokemon=msg.split('\n');

    QWidget *containWidget = new QWidget(ui->bagListArea_3);
    containWidget->setStyleSheet("background-color: rgb(255, 233, 140);");

    QGridLayout *gridLayout = new QGridLayout(containWidget);
    containWidget->setLayout(gridLayout);
    gridLayout->setAlignment(Qt::AlignTop);
    choose2=false;
    // 将背包里的每个小精灵加载到gridlayout中
    int row = 1, col = 1;
    for(int i=0;i<pokemon.size()-1;i++)
    {
        auto detail=pokemon[i].split(' ');
        //设置小精灵图片
        QPushButton *pokemonBtn =new QPushButton(this);
        pokemonBtn->setFixedSize(110,110);
        pokemonBtn->setFlat(true);
        QIcon ico("://picture/"+detail[2]+".png");
        pokemonBtn->setIconSize(QSize(110,110));
        pokemonBtn->setIcon(QIcon("://picture/"+detail[2]+".png"));
        QPushButton *selectBtn=new QPushButton(this);
        selectBtn->setText("选择此精灵");
        //设置等级
        connect(pokemonBtn,&QPushButton::clicked,this, [this, detail] {
            QString str = "getPokemon ";
            str += detail[0];
            client->write(str.toStdString().c_str(), BUF_LENGTH);
            showPokemonDetail = true;
        });
        connect(selectBtn,&QPushButton::clicked,this, [this, detail]{
            enemyNum=detail[0].toInt();
            ui->enemypkm->setPixmap(QPixmap(":/picture/"+detail[2]+".png").scaled(100,100));
            choose2=true;
        });
        QLabel *textLabel = new QLabel();
        textLabel->setText(QString::fromStdString("Lv.")+detail[3]);
        textLabel->setStyleSheet("font: 75 13pt \"Arial Black\"");
        textLabel->setAlignment(Qt::AlignCenter);
        //
        QVBoxLayout *rowlayout = new QVBoxLayout();

        rowlayout->addWidget(pokemonBtn);
        rowlayout->addWidget(textLabel);
        rowlayout->addWidget(selectBtn);
        gridLayout->addLayout(rowlayout, row, col);
        if (++col > 3)
        {
            row++;
            col = 1;
        }
        // 用空layout填充第一行的空位置
    }
    // 用空layout填充第一行的空位置
    if (row == 1 && col != 1)
    {
        for (int i = col; i <= 3; i++)
        {
            QVBoxLayout *rowlayout = new QVBoxLayout();
            QLabel *picLabel = new QLabel();
            picLabel->resize(120, 120);
            rowlayout->addWidget(picLabel);
            gridLayout->addLayout(rowlayout, 1, i);
        }
    }
    gridLayout->setAlignment(Qt::AlignTop);
    ui->bagListArea_3->setWidget(containWidget);
    client->write("getPokemonList", BUF_LENGTH);
}
void MainWindow::DrawBasic(QString msg)
{
    auto detail=msg.split('\n');
    auto pokemonyou=detail[0].split(' ');
    auto pokemonene=detail[9].split(' ');
    ui->yourHead->setPixmap(QPixmap("://picture/"+pokemonyou[2]+".png")
                             .scaled(120, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->enemyHead->setPixmap(QPixmap("://picture/"+pokemonene[2]+".png")
                             .scaled(120, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->yourpokemon->setPixmap(QPixmap("://picture/"+pokemonyou[2]+".png")
            .scaled(120, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->enemypokemon->setPixmap(QPixmap("://picture/"+pokemonene[2]+".png")
                             .scaled(120, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    ui->attackBtn->setText(detail[1]+"\n"+detail[2]);
    ui->attackBtn_2->setText(detail[3]+"\n"+detail[4]);
    ui->attackBtn_3->setText(detail[5]+"\n"+detail[6]);
    ui->attackBtn_4->setText(detail[7]+"\n"+detail[8]);
    ui->yourHp->setRange(0,pokemonyou[4].toInt());
    ui->enemyHp->setRange(0,pokemonene[4].toInt());
    ui->yourHp->setValue(pokemonyou[4].toInt());
    ui->enemyHp->setValue(pokemonene[4].toInt());
    client->write("done.",BUF_LENGTH);

}
void MainWindow::setAnimation(QLabel *attacker, QLabel *suffer,bool miss)
    {
        auto x = attacker->x();
        auto y = attacker->y();
        QSequentialAnimationGroup *actions = new QSequentialAnimationGroup(this);
        // 攻击方出击
        QPropertyAnimation *animation1 = new QPropertyAnimation(attacker, "pos");
        animation1->setDuration(1000);
        animation1->setEasingCurve(QEasingCurve::InQuad);
        animation1->setEndValue(QPoint(suffer->x() - 10, suffer->y()));
        actions->addAnimation(animation1);
        if(miss)
        {
            if(attacker==ui->yourpokemon)
            {
                ui->missLabel->setPixmap(QPixmap(":/picture/miss.png"));
                ui->missLabel->show();
                QTimer::singleShot(1000,ui->missLabel,SLOT(hide()));
            }
            else
            {
                ui->missLabel_2->setPixmap(QPixmap(":/picture/miss.png"));
                ui->missLabel_2->show();
                QTimer::singleShot(1000,ui->missLabel_2,SLOT(hide()));
            }

        }
        // 攻击方回退
        QPropertyAnimation *animation2 = new QPropertyAnimation(attacker, "pos");
        animation2->setDuration(500);
        animation2->setEndValue(QPoint(x, y));
        actions->addAnimation(animation2);

        actions->start();
    }
void MainWindow::DrawBuff(int enemyatk,int enemydef,int enemyspd,int youratk,int yourdef,int yourspd)
{
    QLabel *enemybuf=new QLabel(this);
    QLabel *yourbuf=new QLabel(this);
    enemybuf->setGeometry(930,250,200,40);
    yourbuf->setGeometry(120,250,200,40);
    if(enemyatk==0)
    {
        enemybuf->setPixmap(QPixmap(":/picture/buff/attackdown.png").scaled(QSize(200,40)));
        enemybuf->show();
        QTimer::singleShot(1500,enemybuf,SLOT(hide()));
        return;
    }
    else if(enemyatk==2)
    {
        enemybuf->setPixmap(QPixmap(":/picture/buff/attackup.png").scaled(QSize(200,40)));
        enemybuf->show();
        QTimer::singleShot(1500,enemybuf,SLOT(hide()));
        return;
    }
    else if(enemydef==0)
    {
        enemybuf->setPixmap(QPixmap(":/picture/buff/defencedown.png").scaled(QSize(200,40)));
        enemybuf->show();
        QTimer::singleShot(1500,enemybuf,SLOT(hide()));
        return;
    }
    else if(enemydef==2)
    {
        enemybuf->setPixmap(QPixmap(":/picture/buff/defenceup.png").scaled(QSize(200,40)));
        enemybuf->show();
        QTimer::singleShot(1500,enemybuf,SLOT(hide()));
        return;
    }
    else if(enemyspd==0)
    {
        enemybuf->setPixmap(QPixmap(":/picture/buff/speeddown.png").scaled(QSize(200,40)));
        enemybuf->show();
        QTimer::singleShot(1500,enemybuf,SLOT(hide()));
        return;
    }
    else if(enemyspd==2)
    {
        enemybuf->setPixmap(QPixmap(":/picture/buff/speedup.png").scaled(QSize(200,40)));
        enemybuf->show();
        QTimer::singleShot(1500,enemybuf,SLOT(hide()));
        return;
    }
    if(youratk==0)
    {
        yourbuf->setPixmap(QPixmap(":/picture/buff/attackdown.png").scaled(QSize(200,40)));
        yourbuf->show();
        QTimer::singleShot(1500,yourbuf,SLOT(hide()));
        return;
    }
    else if(youratk==2)
    {
        yourbuf->setPixmap(QPixmap(":/picture/buff/attackup.png").scaled(QSize(200,40)));
        yourbuf->show();
        QTimer::singleShot(1500,yourbuf,SLOT(hide()));
        return;
    }
    else if(yourdef==0)
    {
        yourbuf->setPixmap(QPixmap(":/picture/buff/defencedown.png").scaled(QSize(200,40)));
        yourbuf->show();
        QTimer::singleShot(1500,yourbuf,SLOT(hide()));
        return;
    }
    else if(yourdef==2)
    {
        yourbuf->setPixmap(QPixmap(":/picture/buff/defenceup.png").scaled(QSize(200,40)));
        yourbuf->show();
        QTimer::singleShot(1500,yourbuf,SLOT(hide()));
        return;
    }
    else if(yourspd==0)
    {
        yourbuf->setPixmap(QPixmap(":/picture/buff/speeddown.png").scaled(QSize(200,40)));
        yourbuf->show();
        QTimer::singleShot(1500,yourbuf,SLOT(hide()));
        return;
    }
    else if(yourspd==2)
    {
        yourbuf->setPixmap(QPixmap(":/picture/buff/speedup.png").scaled(QSize(200,40)));
        yourbuf->show();
        QTimer::singleShot(1500,yourbuf,SLOT(hide()));
        return;
    }
}
void MainWindow::DrawBattle(QString msg)
{
    auto detail=msg.split(' ');
    //turn,dodge,otherhp,1 1 1,thishp,1 1 1 youpower

    if(detail[1]=="1")//dodge
    {
        ui->energyBar->setDecMode();
        ui->energyBar->display(detail[2].toInt());
        if(detail[0]=="0")//enemy turn
        {
            setAnimation(ui->enemypokemon,ui->yourpokemon,true);
        }
        else//your turn
        {
            setAnimation(ui->yourpokemon,ui->enemypokemon,true);
        }

        client->write("done.",BUF_LENGTH);
    }
    else//not dodge
    {
        ui->energyBar->setDecMode();
        ui->energyBar->display(detail[10].toInt());
        if(detail[0]=="0")//enemy turn
        {
            if(ui->yourHp->value()!=detail[2].toInt())
            {
                ui->enemyHp->setValue(detail[6].toInt());
                ui->yourHp->setValue(detail[2].toInt());
                setAnimation(ui->enemypokemon,ui->yourpokemon,false);
                DrawBuff(detail[7].toInt(),detail[8].toInt(),detail[9].toInt(),detail[3].toInt(),detail[4].toInt(),detail[5].toInt());
            }
            else//无闪避但是被攻击者hp不变，对方加buf，不动
            {
                ui->enemyHp->setValue(detail[6].toInt());
                ui->yourHp->setValue(detail[2].toInt());
                DrawBuff(detail[7].toInt(),detail[8].toInt(),detail[9].toInt(),detail[3].toInt(),detail[4].toInt(),detail[5].toInt());
            }
            if(detail[2]=="0")//you lose
            {
                if(whichFight==DUEL)
                {
                    QMessageBox::warning(this, tr("很遗憾"), tr("您输掉了决斗赛并且要丢弃一个精灵！"));
                    client->write("getPokemonList", BUF_LENGTH);
                    ChangeToChoose();
                }
                else
                {
                    QMessageBox::warning(this, tr("很遗憾"), tr("您输了！"));
                    ChangeToBag();
                }

            }
            else
            {
               client->write("done.",BUF_LENGTH);
            }
        }
        else//your turn
        {
            if(ui->enemyHp->value()!=detail[2].toInt())
            {
                ui->yourHp->setValue(detail[6].toInt());
                ui->enemyHp->setValue(detail[2].toInt());
                setAnimation(ui->yourpokemon,ui->enemypokemon,false);
                DrawBuff(detail[3].toInt(),detail[4].toInt(),detail[5].toInt(),detail[7].toInt(),detail[8].toInt(),detail[9].toInt());
            }
            else//无闪避但是被攻击者hp不变，自己加buf，不动
            {
                ui->yourHp->setValue(detail[6].toInt());
                ui->enemyHp->setValue(detail[2].toInt());
                DrawBuff(detail[3].toInt(),detail[4].toInt(),detail[5].toInt(),detail[7].toInt(),detail[8].toInt(),detail[9].toInt());
            }
            if(detail[2]=="0")//you win
            {
                if(whichFight==DUEL)
                {
                    QMessageBox::warning(this, tr("恭喜"), tr("您已获胜并获得了一个新的小精灵！"));
                }
                else
                {
                    QMessageBox::warning(this, tr("恭喜"), tr("您已获胜！"));
                }
                ChangeToBag();
            }
            else
            {
               client->write("done.",BUF_LENGTH);
            }
        }

    }


}
void MainWindow::WaitForSkill()
{
    int pp=ui->energyBar->value();
    if(pp>=20)
    {
        ui->attackBtn->setEnabled(true);
        ui->attackBtn_2->setEnabled(true);
        ui->attackBtn_3->setEnabled(true);
        ui->attackBtn_4->setEnabled(true);
    }
    else if(pp>=15)
    {
        ui->attackBtn->setEnabled(true);
        ui->attackBtn_2->setEnabled(true);
        ui->attackBtn_3->setEnabled(true);
    }
    else if(pp>=10)
    {
        ui->attackBtn->setEnabled(true);
        ui->attackBtn_2->setEnabled(true);
    }
    else
    {
        ui->attackBtn->setEnabled(true);
    }

}
void MainWindow::Discard()
{
    QString msg="discard ";
    msg+=QString::number(discardNum);
    client->write(msg.toLocal8Bit(),BUF_LENGTH);
    ChangeToBag();
}
void MainWindow::Choose(QString msg)
{
    auto pokemon=msg.split('\n');
    int pkm[5];
    pkm[0]=rand()%(pokemon.size()-1);
    pkm[1]=rand()%(pokemon.size()-1);
    pkm[2]=rand()%(pokemon.size()-1);
    QWidget *containWidget = new QWidget(ui->bagListArea_2);
    containWidget->setStyleSheet("background-color: rgb(61, 116, 159);");

    QGridLayout *gridLayout = new QGridLayout(containWidget);
    containWidget->setLayout(gridLayout);
    gridLayout->setAlignment(Qt::AlignTop);
    int row = 1, col = 1;
    for(int i=0;i<3;i++)
    {
        auto detail=pokemon[pkm[i]].split(' ');
        //设置小精灵图片
        QPushButton *pokemonBtn =new QPushButton(this);
        pokemonBtn->setFixedSize(200,200);
        QIcon ico("://picture/"+detail[2]+".png");
        pokemonBtn->setIconSize(QSize(200,200));
        pokemonBtn->setIcon(QIcon("://picture/"+detail[2]+".png"));
        //设置等级
        connect(pokemonBtn,&QPushButton::clicked,this, [this, detail] {
            QString str = "getPokemon ";
            str += detail[0];
            client->write(str.toStdString().c_str(), BUF_LENGTH);
            showPokemonDetail = true;
        });
        QPushButton *selectBtn=new QPushButton(this);
        selectBtn->setText("选择此精灵");
        //设置等级
        connect(selectBtn,&QPushButton::clicked,this, [this, detail]{
            discardNum=detail[0].toInt();
            ui->yourdiscard->setPixmap(QPixmap("://picture/"+detail[2]+".png").scaled(130,130));
            ui->discardBtn->show();
        });
        pokemonBtn->setFlat(true);
        QLabel *textLabel = new QLabel();
        textLabel->setText(QString::fromStdString("Lv.")+detail[3]);
        textLabel->setStyleSheet("font: 75 13pt \"Arial Black\"");
        textLabel->setAlignment(Qt::AlignCenter);
        //
        QVBoxLayout *rowlayout = new QVBoxLayout();

        rowlayout->addWidget(pokemonBtn);
        rowlayout->addWidget(textLabel);
        rowlayout->addWidget(selectBtn);
        gridLayout->addLayout(rowlayout, row, col);
        if (++col > 3)
        {
            row++;
            col = 1;
        }
    }
    if (row == 1 && col != 1)
    {
        for (int i = col; i <= 3; i++)
        {
            QVBoxLayout *rowlayout = new QVBoxLayout();
            QLabel *picLabel = new QLabel();
            picLabel->resize(120, 120);
            rowlayout->addWidget(picLabel);
            gridLayout->addLayout(rowlayout, 1, i);
        }
    }
    gridLayout->setAlignment(Qt::AlignTop);
    ui->bagListArea_2->setWidget(containWidget);


}
void MainWindow::ShowUser(QString msg)
{
    auto detail=msg.split(' ');
    ui->usernameLabel->setText(detail[0]);
    ui->winrateLabel->setText(detail[1]+"/"+detail[2]);
    if(pokemonNum>=10)
    {
        ui->sumLabel->setPixmap(QPixmap(":/picture/golden.png").scaled(QSize(25,25)));
    }
    else if(pokemonNum>=8)
    {
        ui->sumLabel->setPixmap(QPixmap(":/picture/silver.png").scaled(QSize(25,25)));
    }
    else if(pokemonNum>=5)
    {
        ui->sumLabel->setPixmap(QPixmap(":/picture/bronze.png").scaled(QSize(25,25)));
    }

    if(lv15Num>=6)
    {
        ui->adsumLabel->setPixmap(QPixmap(":/picture/golden.png").scaled(QSize(25,25)));
    }
    else if(lv15Num>=4)
    {
        ui->adsumLabel->setPixmap(QPixmap(":/picture/silver.png").scaled(QSize(25,25)));
    }
    else if(lv15Num>=2)
    {
        ui->adsumLabel->setPixmap(QPixmap(":/picture/bronze.png").scaled(QSize(25,25)));
    }


}
void MainWindow::getServerMsg()
{
    auto ret = client->read(BUF_LENGTH);
    QString msg = QString::fromLocal8Bit(ret);

    switch(ui->stackedWidget->currentIndex())
    {
    case 0://main
    {

    }
    case 1://bag
    {

        if(showPokemonDetail)
        {
            showpokemon *show=new showpokemon(client,port,msg,true);
            show->show();
            showPokemonDetail=false;
        }
        else
        {
            if(showuser)
            {
                ShowUser(msg);
                showuser=false;
            }
            else
            {
                ShowPokemonList(msg);
            }
        }
        break;
    }
    case 2://userlist
    {
        if(!showbag)
        {
            if(showPokemonDetail)
            {
                showpokemon *show=new showpokemon(client,port,msg,false);
                show->show();
                showPokemonDetail=false;
            }
            else
            {
                lastmsg=msg;
                ShowUserList(msg);
            }
        }
        else
        {
            ShowUserList(lastmsg);
            ShowBag(msg);
            showbag=false;
        }
        break;
    }
    case 3://changepass
    {
        if (msg == "Accept.\n")
        {
            QMessageBox::information(this, tr("密码重置"), tr("密码重置成功"));
            ChangeToMain();
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), msg);
        }
        break;
    }
    case 4://fightchoose
    {
        if(showPokemonDetail)
        {
            showpokemon *show=new showpokemon(client,port,msg,false);
            show->show();
            showPokemonDetail=false;
        }
        else if(!showFightlist)
        {
            ui->startBtn->hide();
            ShowFightList(msg);
        }
        else
        {
            ShowYourPokem(msg);
        }
        break;
    }
    case 5://fight
    {
        if(!draw)
        {
            DrawBasic(msg);
            draw=true;
            break;
        }
        else
        {
            if(msg=="turn")
            {
                WaitForSkill();
                turn=true;
                break;
            }
            else
            {
                DrawBattle(msg);
                break;
            }
        }

    }
    case 6://choose
    {
        if(showPokemonDetail)
        {
            showpokemon *show=new showpokemon(client,port,msg,false);
            show->show();
            showPokemonDetail=false;
        }
        else
        {
            Choose(msg);
        }
    }
    default:break;

    }
}
void MainWindow::CheckW()
{
    QString local_city = "北京"; //获得需要查询天气的城市名称
    char quest_array[256] = "https://api.seniverse.com/v3/weather/now.json?key=SuxdtVhXg9lCaBm2U&location=beijing&language=zh-Hans&unit=c";
    QNetworkRequest quest;
    /*sprintf(quest_array, "%s%s", quest_array, local_city.toUtf8().data());*/
    quest.setUrl(QUrl(quest_array));
    /* Set header */
    quest.setRawHeader(QByteArray("User-Agent"), QByteArray("Mozilla/5.0"));
    /* Auto redirect */
    quest.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    /*发送get网络请求*/
    manager->get(quest);
}
void MainWindow::replyFinished(QNetworkReply *reply)  //天气数据处理槽函数
{
    qDebug() << "recv weather data!!";
    QString all = reply->readAll();

    /*ui->textBrowser->setText(all);*/ //将接收到的数据显示出来

    QJsonParseError err;
    QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(), &err);//解析json对象
    qDebug() << err.error;
    if(err.error != QJsonParseError::NoError) {
           qDebug() << "Parsed error";
           return;
       }
       QJsonObject obj = json_recv.object();
       QJsonValue result=obj.value("results");
       if(result.isArray())
       {
           QJsonObject weather_obj=result.toArray().at(0).toObject();

           QJsonObject location=weather_obj.value("location").toObject();
           QString city=location.value("name").toString();

           QJsonObject now=weather_obj.value("now").toObject();
           QString type=now.value("text").toString();
           QString temp=now.value("temperature").toString();
            QString we=now.value("code").toString();

           ui->type->setPixmap(QPixmap(":/picture/type/"+we+".png"));
           ui->type->setScaledContents(true);
           ui->wendu->setText(temp);
           ui->fengli->setText(city);
       }
    reply->deleteLater(); //销毁请求对象
}
