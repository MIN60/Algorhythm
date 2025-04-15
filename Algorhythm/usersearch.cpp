#include "usersearch.h"
#include <QDesktopServices>
#include <QUrl>
#include <QListWidgetItem>
#include <QPushButton>

UserSearch::UserSearch(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    setupData();
}

void UserSearch::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("문제 검색하기", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14pt;");

    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("문제 번호를 입력하세요");

    QPushButton* searchButton = new QPushButton("검색", this);
    searchButton->setFixedWidth(60);

    QHBoxLayout* inputLayout = new QHBoxLayout;
    inputLayout->addWidget(searchInput);
    inputLayout->addWidget(searchButton);

    resultList = new QListWidget(this);
    resultList->setAlternatingRowColors(true);

    layout->addWidget(titleLabel);
    layout->addLayout(inputLayout);
    layout->addWidget(resultList);

    connect(searchButton, &QPushButton::clicked, this, [this]() {
        filterList(searchInput->text());  // 버튼 누르면 실행
    });

    connect(resultList, &QListWidget::itemDoubleClicked, this, [](QListWidgetItem* item) {
        QString id = item->data(Qt::UserRole).toString();
        QDesktopServices::openUrl(QUrl("https://www.acmicpc.net/problem/" + id));
    });
}


void UserSearch::setupData()
{
    // 더미 이따 연결할거임
    problem = {
        { "1000", "A+B", "브론즈 5" },
        { "1001", "A-B", "브론즈 5" },
        { "27", "Hello World", "실버 5" }
    };
}

void UserSearch::filterList(const QString& input)
{
    resultList->clear();

    QString trimmed = input.trimmed();
    if (trimmed.isEmpty()) return;

    for (const auto& p : problem) {
        if (p.id == trimmed) {
            QListWidgetItem* item = new QListWidgetItem(
                QString("%1 - %2 [%3]").arg(p.id, p.title, p.problemtier));
            item->setData(Qt::UserRole, p.id);
            resultList->addItem(item);
            break;
        }
    }
}
