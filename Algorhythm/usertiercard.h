#ifndef USERTIERCARD_H
#define USERTIERCARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>

class UserTierCard : public QWidget {
    Q_OBJECT
public:
    explicit UserTierCard(QWidget* parent = nullptr);
    void setTier(const QString& tierName, int solvedCount);

private:
    QLabel* tierLabel;
    QLabel* solvedLabel;
};


#endif // USERTIERCARD_H
