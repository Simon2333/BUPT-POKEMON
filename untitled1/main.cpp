#include "mainwindow.h"
#include "login.h"
#include "signup.h"
#include <QApplication>
#undef main
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*MainWindow w;*/
    login log;
    signup sign;
    log.show();
    /*w.show();*/
    return a.exec();
}
