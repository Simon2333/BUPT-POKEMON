#ifndef LOGIN_H
#define LOGIN_H

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
#include "gamelobby.h"

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    int GetPort();
protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

public slots:
    void Login();
    void Signup();

private:
    Ui::login *ui;

    int port;
    int state;
    void InitUi();
    void InitConnect();
    QString username;
    QTcpSocket *client;
    QPoint p;

private slots:
    void getServerMsg();
};

#endif // LOGIN_H
