#ifndef SIGNUPP_H
#define SIGNUPP_H

#include <QDialog>

namespace Ui {
class signupp;
}

class signupp : public QDialog
{
    Q_OBJECT

public:
    explicit signupp(QWidget *parent = nullptr);
    ~signupp();

private:
    Ui::signupp *ui;
};

#endif // SIGNUPP_H
