#include "usersearch.h"
#include <QDesktopServices>
#include <QUrl>
#include <QListWidgetItem>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

UserSearch::UserSearch(QWidget* parent)
    : QWidget(parent), networkManager(nullptr)
{
    setupUI();
    setupData();
}

void UserSearch::setNetworkManager(NetworkManager* manager)
{
    networkManager = manager;
    setupConnections();
}

void UserSearch::setupConnections()
{
    if (!networkManager) return;

    // NetworkManager 시그널 연결
    connect(networkManager, &NetworkManager::onProblemDataReceived,
            this, &UserSearch::handleProblemData);
    connect(networkManager, &NetworkManager::onNetworkError,
            this, &UserSearch::handleNetworkError);
}

void UserSearch::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* titleLabel = new QLabel("문제 검색", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16pt;");

    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("문제 번호를 입력하세요");
    searchInput->setStyleSheet("font-size: 14pt; padding: 6px;");

    QPushButton* searchButton = new QPushButton("검색", this);
    searchButton->setFixedHeight(40);
    searchButton->setStyleSheet(R"(
        QPushButton {
            font-size: 14pt;
            background-color: #4AC26B;
            color: white;
            border: none;
            font-weight: bold;
            border-radius: 6px;
            padding: 6px 12px;
        }
        QPushButton:hover {
            background-color: #3bb25a;
        }
    )");

    QHBoxLayout* inputLayout = new QHBoxLayout;
    inputLayout->addWidget(searchInput);
    inputLayout->addWidget(searchButton);

    resultList = new QListWidget(this);
    resultList->setAlternatingRowColors(true);
    resultList->setStyleSheet("font-size: 14pt;");

    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: gray; font-style: italic;");

    layout->addWidget(titleLabel);
    layout->addLayout(inputLayout);
    layout->addWidget(resultList);
    layout->addWidget(statusLabel);

    // 검색 버튼 클릭 시 API로 검색 실행
    connect(searchButton, &QPushButton::clicked, this, [this]() {
        QString problemId = searchInput->text().trimmed();
        if (!problemId.isEmpty()) {
            searchProblem(problemId);
        }
    });

    // 엔터 키 입력 시에도 검색 실행
    connect(searchInput, &QLineEdit::returnPressed, this, [this]() {
        QString problemId = searchInput->text().trimmed();
        if (!problemId.isEmpty()) {
            searchProblem(problemId);
        }
    });

    // 문제 클릭 시 브라우저로 열기
    connect(resultList, &QListWidget::itemDoubleClicked, this, [](QListWidgetItem* item) {
        QString id = item->data(Qt::UserRole).toString();
        QDesktopServices::openUrl(QUrl("https://www.acmicpc.net/problem/" + id));
    });
}

void UserSearch::setupData()
{
    // 기존 문제 데이터는 유지하되, API 결과로 대체될 예정
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
            addProblemToList(p);
            break;
        }
    }
}

void UserSearch::searchProblem(const QString& problemId)
{
    // 검색 시작 시 상태 표시
    resultList->clear();

    if (networkManager) {
        // NetworkManager를 통해 API 요청
        networkManager->problemData(problemId);
    } else {
        // NetworkManager가 설정되지 않은 경우 로컬 검색
        filterList(problemId);
        //statusLabel->setText("NetworkManager가 설정되지 않아 로컬 검색만 수행됩니다.");
    }
}

void UserSearch::handleProblemData(const QJsonObject &problemData)
{
    if (!problemData.isEmpty() && problemData.contains("problemId")) {
        // API 응답에서 문제 정보 추출
        QString id = QString::number(problemData["problemId"].toInt());
        QString title = problemData["titleKo"].toString();

        // 티어 정보 변환
        int tier = problemData.contains("level") ? problemData["level"].toInt() : 0;
        QString tierName;

        switch (tier) {
        case 0: tierName = "Unrated"; break;
        case 1: case 2: case 3: case 4: case 5: tierName = "브론즈 " + QString::number(6 - tier); break;
        case 6: case 7: case 8: case 9: case 10: tierName = "실버 " + QString::number(11 - tier); break;
        case 11: case 12: case 13: case 14: case 15: tierName = "골드 " + QString::number(16 - tier); break;
        case 16: case 17: case 18: case 19: case 20: tierName = "플래티넘 " + QString::number(21 - tier); break;
        case 21: case 22: case 23: case 24: case 25: tierName = "다이아몬드 " + QString::number(26 - tier); break;
        case 26: case 27: case 28: case 29: case 30: tierName = "루비 " + QString::number(31 - tier); break;
        default: tierName = "마스터"; break;
        }

        // 결과 리스트에 문제 추가
        Problem newProblem = {id, title, tierName};
        addProblemToList(newProblem);

        // 검색된 문제를 로컬 캐시에 추가 (중복 확인)
        bool exists = false;
        for (auto& p : problem) {
            if (p.id == id) {
                p = newProblem; // 기존 정보 업데이트
                exists = true;
                break;
            }
        }

        if (!exists) {
            problem.push_back(newProblem); // 새 문제 추가
        }

        //statusLabel->setText("검색 완료");
    } else {
        QListWidgetItem* item = new QListWidgetItem("문제를 찾을 수 없습니다.");
        item->setFlags(Qt::NoItemFlags);  // 클릭 불가능
        resultList->addItem(item);
    }

    statusLabel->clear();
}

void UserSearch::handleNetworkError(const QString &errorMsg)
{
    resultList->clear();

    QListWidgetItem* item = new QListWidgetItem("문제를 찾을 수 없습니다.");
    item->setFlags(Qt::NoItemFlags);
    resultList->addItem(item);

    statusLabel->clear();
}

void UserSearch::addProblemToList(const Problem& p)
{
    QWidget* itemWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(10, 4, 10, 4);

    //왼: 문제 번호, 제목
    QLabel* titleLabel = new QLabel(QString("%1 - %2").arg(p.id, p.title));
    titleLabel->setStyleSheet("font-size: 14pt;");
    layout->addWidget(titleLabel);

    // 오른쪽 티어
    QLabel* tierLabel = new QLabel(p.problemtier);
    tierLabel->setStyleSheet("font-size: 14pt; color: gray;");
    layout->addStretch();  // 가운데 빈 공간 밀어냄
    layout->addWidget(tierLabel);

    QListWidgetItem* item = new QListWidgetItem(resultList);
    item->setSizeHint(itemWidget->sizeHint());
    resultList->addItem(item);
    resultList->setItemWidget(item, itemWidget);

    item->setData(Qt::UserRole, p.id);
    item->setToolTip("더블클릭 시 해당 문제의 페이지로 이동합니다.");
}

void UserSearch::clearSearchResult() {
    resultList->clear();
    statusLabel->clear();
    searchInput->clear();
}
