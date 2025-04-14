#include "algorhythm.h"
#include "./ui_algorhythm.h"

Algorhythm::Algorhythm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Algorhythm)
{
    ui->setupUi(this);
}

Algorhythm::~Algorhythm()
{
    delete ui;
}
