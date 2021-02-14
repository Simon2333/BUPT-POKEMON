#include "signupp.h"
#include "ui_signupp.h"

signupp::signupp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::signupp)
{
    ui->setupUi(this);
}

signupp::~signupp()
{
    delete ui;
}
