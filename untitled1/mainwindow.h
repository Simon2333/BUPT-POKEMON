#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QLCDNumber>
#include <QtNetwork/QtNetwork>
#include "gamelobby.h"
#include <QDateTime>
#include <QSignalMapper>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class QTime;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int port,QWidget *parent = nullptr);
    ~MainWindow();
    bool changePokemonName=false;
    QTcpSocket *client;
public slots:
    void OnTimeOut();
private:
    Ui::MainWindow *ui;
    int state;
    void InitUi();
    void InitConnect();
    QString username;
    bool showbag=false;
    bool showPokemonDetail=false;
    QSignalMapper *_signalMapper;
    int port;
    QString lastmsg;
    bool choose1=false;
    bool choose2=false;
    int enemyNum;
    int yourNum;
    bool showFightlist=false;
    int whichFight;
    bool duringFight=false;
    bool draw=false;
    bool turn=false;
    int skillIndex;
    int discardNum;
    int pokemonNum;
    int lv15Num;
    bool showuser=false;
    QTimer *timer;
    QDateTime dateTime;
    QNetworkAccessManager *manager;  //请求句柄
    QString fengli;       //风力
    QString wendu;        //温度
    QString weather_type;  //天气类型
private slots:
    void CheckW();
    void replyFinished(QNetworkReply *reply);
    void ChangeToBag();
    void ChangeToList();
    void ChangeToLogin();
    void ChangeToMain();
    void ChangeToPass();
    void ChangeToChoose();
    void ChangeToFightRoom();
    void ChangeToFight();
    void ChooseLvupBet();
    void ChooseDuelBet();
    void getServerMsg();
    void ChangePassword();
    void StartFight();
    void WaitForSkill();
    void Discard();
    void ChangeGif();
    void DrawBuff(int enemyatk,int enemydef,int enemyspd,int youratk,int yourdef,int yourspd);
    void ShowUser(QString msg);
    void Choose(QString msg);
    void DrawBattle(QString msg);
    void DrawBasic(QString msg);
    void ShowYourPokem(QString msg);
    void ShowPokemonList(QString msg);
    void ShowUserList(QString msg);
    void ShowBag(QString msg);
    void ShowFightList(QString msg);
    void setAnimation(QLabel *attacker, QLabel *suffer,bool miss);
signals:
    void SkillChoosed();

};

#endif // MAINWINDOW_H
