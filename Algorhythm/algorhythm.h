#ifndef ALGORHYTHM_H
#define ALGORHYTHM_H
#include "networkmanager.h"
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
    void applyTabStyle(QTabWidget* tabWidget);

private slots:
    void updateTimeLabel();

private:
    Ui::Algorhythm *ui;
    NetworkManager* networkManager;
    QTimer* timeTimer;
    void setupWidgets();
};
#endif // ALGORHYTHM_H
