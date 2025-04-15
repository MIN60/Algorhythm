#ifndef USERGREETING_H
#define USERGREETING_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "usertiercard.h"

class UserGreeting : public QWidget {
    Q_OBJECT
public:
    explicit UserGreeting(QWidget* parent = nullptr);
    void setUserName(const QString& name);
    void setTier(const QString& tierName, int solvedCount);

private:
    QLabel* greetingLabel;
    QLabel* nameLabel;
    UserTierCard* tierCard;
};


#endif // USERGREETING_H
