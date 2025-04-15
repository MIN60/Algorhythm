#include "algorhythm.h"
#include "./ui_algorhythm.h"
#include <QTimer>

Algorhythm::Algorhythm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Algorhythm)
{
    ui->setupUi(this);

    QTimer::singleShot(0, this, [this]() {
        ui->user_greeting->setUserName("김땡땡");
        ui->user_greeting->setTier("Gold I", 1234);
    });

}

Algorhythm::~Algorhythm()
{
    delete ui;
}
