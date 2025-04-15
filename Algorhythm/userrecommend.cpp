#include "userrecommend.h"
#include <QDesktopServices>


UserRecommend::UserRecommend(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    titleLabel = new QLabel("오늘의 알고리듬 복습", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14pt;");

    listWidget = new QListWidget(this);
    listWidget->setAlternatingRowColors(true);

    layout->addWidget(titleLabel);
    layout->addWidget(listWidget);

    setLayout(layout);
}

void UserRecommend::setRecommend(const QList<RecommendProblem>& list)
{
    listWidget->clear();

    for (const auto& p : list) {
        QString displayText = QString("%1 [%2]").arg(p.title, p.re_problemtier);
        QListWidgetItem* item = new QListWidgetItem(displayText);

        item->setData(Qt::UserRole, p.id);  //문제 번호

        listWidget->addItem(item);
    }

    connect(listWidget, &QListWidget::itemDoubleClicked, this, [](QListWidgetItem* item) {
        QString id = item->data(Qt::UserRole).toString();
        QDesktopServices::openUrl(QUrl("https://www.acmicpc.net/problem/" + id));
    });
}
