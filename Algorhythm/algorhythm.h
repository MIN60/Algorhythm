#ifndef ALGORHYTHM_H
#define ALGORHYTHM_H

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
    void applyTabStyle(QTabWidget* tabWidget);

private:
    Ui::Algorhythm *ui;
};
#endif // ALGORHYTHM_H
