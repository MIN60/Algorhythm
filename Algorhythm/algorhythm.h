#ifndef ALGORHYTHM_H
#define ALGORHYTHM_H

#include <QMainWindow>
#include "networkmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Algorhythm;
}
QT_END_NAMESPACE

class Algorhythm : public QMainWindow
{
    Q_OBJECT

public:
    Algorhythm(QWidget *parent = nullptr);
    ~Algorhythm();

private:
    Ui::Algorhythm *ui;
    NetworkManager* networkManager;
};
#endif // ALGORHYTHM_H
