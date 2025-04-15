#ifndef ALGORHYTHM_H
#define ALGORHYTHM_H
#include "networkmanager.h"
#include <QMainWindow>

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
    void setupWidgets();
};
#endif // ALGORHYTHM_H
