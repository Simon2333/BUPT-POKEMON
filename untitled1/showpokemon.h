#ifndef SHOWPOKEMON_H
#define SHOWPOKEMON_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>
#include <QMainWindow>
#include <QtNetwork/QTcpServer>

namespace Ui {
class showpokemon;
}

class showpokemon : public QWidget
{
    Q_OBJECT

public:
    explicit showpokemon(QTcpSocket *clients,int port,QString msg,bool self,QWidget *parent = nullptr);
    ~showpokemon();
    void InitUi(QString msg);
private slots:
    QString changerace(int race);
    QString changetype(int type);
    QString changeele(int element);
    void ChangeName();
    void getServerMsg();

private:
    Ui::showpokemon *ui;
    QTcpSocket *client;
    QTcpSocket *clients;
    int port;
    bool wheherself;
    QString msg;

};

#endif // SHOWPOKEMON_H
