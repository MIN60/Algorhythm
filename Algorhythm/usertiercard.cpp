#include "usertiercard.h"


UserTierCard::UserTierCard(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 카드 프레임
    QFrame* cardFrame = new QFrame(this);
    cardFrame->setFrameShape(QFrame::StyledPanel);
    cardFrame->setStyleSheet("background-color: #FFF3CD; border-radius: 10px; padding: 15px;");
    cardFrame->setMinimumHeight(100);
    cardFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QVBoxLayout* cardLayout = new QVBoxLayout(cardFrame);

    // 티어 라벨 더미
    tierLabel = new QLabel("Gold I", cardFrame);
    tierLabel->setStyleSheet("font-size: 24pt; font-weight: bold; color: #2E7D32;");
    tierLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    tierLabel->setWordWrap(true);
    tierLabel->setAlignment(Qt::AlignLeft);
    tierLabel->setMinimumHeight(30);

    // 해결 문제 수 라벨 더미
    solvedLabel = new QLabel("해결한 문제 수: 1234개", cardFrame);
    solvedLabel->setStyleSheet("font-size: 14pt; color: #333333;");
    solvedLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    solvedLabel->setWordWrap(true);
    solvedLabel->setMinimumHeight(20);

    // 카드 레이아웃 구성
    cardLayout->addWidget(tierLabel);
    cardLayout->addWidget(solvedLabel);
    cardLayout->setSpacing(5);
    cardFrame->setLayout(cardLayout);

    layout->addWidget(cardFrame);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

void UserTierCard::setTier(const QString& tierName, int solvedCount)
{
    tierLabel->setText(tierName);
    solvedLabel->setText("해결한 문제 수: " + QString::number(solvedCount) + "개");
    this->setStyleSheet("background-color: #E3F2FD;");

}
