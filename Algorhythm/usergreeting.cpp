#include "usergreeting.h"

UserGreeting::UserGreeting(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);

    QVBoxLayout* greetingLayout = new QVBoxLayout;
    greetingLabel = new QLabel("안녕하세요,", this);
    greetingLabel->setStyleSheet("font-size: 20pt;");
    nameLabel = new QLabel("김땡땡님!", this);
    nameLabel->setStyleSheet("font-size: 30pt; font-weight: bold; color: #2E7D32;");

    greetingLayout->addWidget(greetingLabel);
    greetingLayout->addWidget(nameLabel);
    greetingLayout->addStretch();

    tierCard = new UserTierCard(this);
    tierCard->setTier("Gold I", 1234);

    layout->addLayout(greetingLayout); // 왼쪽 인사
    layout->addWidget(tierCard);  // 오른쪽 티어
    layout->setSpacing(40);
    layout->setContentsMargins(15, 15, 15, 15);
}

void UserGreeting::setUserName(const QString& name)
{
    nameLabel->setText(name + "님!");
}

void UserGreeting::setTier(const QString& tierName, int solvedCount)
{
    tierCard->setTier(tierName, solvedCount);
}
