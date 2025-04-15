#include "usergreeting.h"

UserGreeting::UserGreeting(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    greetingLabel = new QLabel("안녕하세요,", this);
    greetingLabel->setStyleSheet("font-size: 20pt;");

    nameLabel = new QLabel("김땡땡님!", this);
    nameLabel->setStyleSheet("font-size: 30pt; font-weight: bold; color: #2E7D32;");

    tierCard = new UserTierCard(this);  // 💡 생성
    tierCard->setTier("Gold I", 1234);  // 초기 더미값

    layout->addWidget(greetingLabel);
    layout->addWidget(nameLabel);
    layout->addWidget(tierCard);

    layout->setSpacing(10);
    layout->setContentsMargins(15, 15, 15, 15);
    setLayout(layout);
}

void UserGreeting::setUserName(const QString& name)
{
    nameLabel->setText(name + "님!");
}

void UserGreeting::setTier(const QString& tierName, int solvedCount)
{
    tierCard->setTier(tierName, solvedCount);
}
