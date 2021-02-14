#include "gamelobby.h"
#include "ui_gamelobby.h"

gamelobby::gamelobby(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gamelobby)
{
    ui->setupUi(this);
}

gamelobby::~gamelobby()
{
    delete ui;
}
