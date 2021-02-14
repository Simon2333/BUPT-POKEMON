#include "changepassword.h"
#include "ui_changepassword.h"

changepassword::changepassword(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::changepassword)
{
    ui->setupUi(this);
}

changepassword::~changepassword()
{
    delete ui;
}
