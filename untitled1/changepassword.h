#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QWidget>

namespace Ui {
class changepassword;
}

class changepassword : public QWidget
{
    Q_OBJECT

public:
    explicit changepassword(QWidget *parent = nullptr);
    ~changepassword();

private:
    Ui::changepassword *ui;
};

#endif // CHANGEPASSWORD_H
