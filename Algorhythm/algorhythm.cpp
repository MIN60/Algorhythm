#include "algorhythm.h"
#include "./ui_algorhythm.h"

Algorhythm::Algorhythm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Algorhythm)
{
    ui->setupUi(this);

    ui->user_greeting->setUserName("로그인이 필요합니다");
    ui->user_greeting->setTier("Gold I", 1234);
    qDebug() << ui->user_search->isVisible();


}

Algorhythm::~Algorhythm()
{
    delete ui;
}
