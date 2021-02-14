#ifndef GAMELOBBY_H
#define GAMELOBBY_H

#include <QWidget>

namespace Ui {
class gamelobby;
}

class gamelobby : public QWidget
{
    Q_OBJECT

public:
    explicit gamelobby(QWidget *parent = nullptr);
    ~gamelobby();

private:
    Ui::gamelobby *ui;
};

#endif // GAMELOBBY_H
