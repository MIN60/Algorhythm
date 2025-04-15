#include "userrecommend.h"
#include <QDesktopServices>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

UserRecommend::UserRecommend(QWidget* parent)
    : QWidget(parent), networkManager(nullptr)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    titleLabel = new QLabel("오늘의 알고리듬 복습", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14pt;");

    listWidget = new QListWidget(this);
    listWidget->setAlternatingRowColors(true);

    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: gray; font-style: italic;");

    layout->addWidget(titleLabel);
    layout->addWidget(listWidget); 
    layout->addWidget(statusLabel);

    connect(listWidget, &QListWidget::itemDoubleClicked, this, [](QListWidgetItem* item) {
        QString id = item->data(Qt::UserRole).toString();
        QDesktopServices::openUrl(QUrl("https://www.acmicpc.net/problem/" + id));
    });

    qDebug() << "UserRecommend 생성자 호출됨";
}
void UserRecommend::setNetworkManager(NetworkManager* manager)
{
    qDebug() << "UserRecommend::setNetworkManager 호출됨";

    if (!manager) {
        qDebug() << "경고: NULL NetworkManager가 설정되었습니다!";
        return;
    }

    networkManager = manager;
    setupConnections();
}

void UserRecommend::setupConnections()
{
    qDebug() << "UserRecommend::setupConnections 호출됨";

    if (!networkManager) {
        qDebug() << "NetworkManager가 NULL입니다!";
        return;
    }

    // 연결 해제 후 다시 연결 (중복 연결 방지)
    disconnect(networkManager, &NetworkManager::onRandomProblemsReceived,
               this, &UserRecommend::handleRecommendDate);
    disconnect(networkManager, &NetworkManager::onNetworkError,
               this, &UserRecommend::handleNetworkError);

    // NetworkManager 시그널 연결
    connect(networkManager, &NetworkManager::onRandomProblemsReceived,
            this, &UserRecommend::handleRecommendDate);
    connect(networkManager, &NetworkManager::onNetworkError,
            this, &UserRecommend::handleNetworkError);

    qDebug() << "UserRecommend: 시그널-슬롯 연결 완료";
}

void UserRecommend::loadRecommendations(const QString& handle)
{
    qDebug() << "UserRecommend::loadRecommendations 호출됨, handle=" << handle;

    if (!networkManager) {
        qDebug() << "NetworkManager가 NULL입니다!";
        statusLabel->setText("NetworkManager가 설정되지 않았습니다.");
        return;
    }

    listWidget->clear();

    // NetworkManager를 통해 추천 문제 요청
    qDebug() << "NetworkManager::recommendData 호출 중...";
    networkManager->recommendData(handle);
}

void UserRecommend::handleRecommendDate(const QJsonObject &recommendData)
{
    qDebug() << "UserRecommend::handleRecommendDate 호출됨";
    qDebug() << "추천 데이터 형식:" << recommendData.keys();

    listWidget->clear();

    // API 응답 처리
    QList<RecommendProblem> recommendList;

    if (recommendData.contains("items") && recommendData["items"].isArray()) {
        QJsonArray items = recommendData["items"].toArray();
        qDebug() << "항목 수:" << items.size();

        for (const QJsonValue &item : items) {
            QJsonObject problemObj = item.toObject();

            // 수정된 부분: 직접 problemObj에서 필드를 추출
            // 기존 코드는 problemObj["problem"]을 찾았지만, 실제로는 problemObj 자체가 문제 객체임
            if (problemObj.contains("problemId") && problemObj.contains("titleKo")) {
                QString id = QString::number(problemObj["problemId"].toInt());
                QString title = problemObj["titleKo"].toString();

                // 티어 정보 (level을 사용할 수 있음)
                QString tierName = "";
                if (problemObj.contains("level")) {
                    int level = problemObj["level"].toInt();
                    // 필요하다면 level을 티어 이름으로 변환
                    switch(level) {
                    case 0: tierName = "Unrated"; break;
                    case 1: tierName = "Bronze V"; break;
                    case 2: tierName = "Bronze IV"; break;
                    case 3: tierName = "Bronze III"; break;
                    case 4: tierName = "Bronze II"; break;
                    case 5: tierName = "Bronze I"; break;
                    case 6: tierName = "Silver V"; break;
                    case 7: tierName = "Silver IV"; break;
                    case 8: tierName = "Silver III"; break;
                    case 9: tierName = "Silver II"; break;
                    case 10: tierName = "Silver I"; break;
                    case 11: tierName = "Gold V"; break;
                    case 12: tierName = "Gold IV"; break;
                    case 13: tierName = "Gold III"; break;
                    case 14: tierName = "Gold II"; break;
                    case 15: tierName = "Gold I"; break;
                    case 16: tierName = "Platinum V"; break;
                    case 17: tierName = "Platinum IV"; break;
                    case 18: tierName = "Platinum III"; break;
                    case 19: tierName = "Platinum II"; break;
                    case 20: tierName = "Platinum I"; break;
                    case 21: tierName = "Diamond V"; break;
                    case 22: tierName = "Diamond IV"; break;
                    case 23: tierName = "Diamond III"; break;
                    case 24: tierName = "Diamond II"; break;
                    case 25: tierName = "Diamond I"; break;
                    case 26: tierName = "Ruby V"; break;
                    case 27: tierName = "Ruby IV"; break;
                    case 28: tierName = "Ruby III"; break;
                    case 29: tierName = "Ruby II"; break;
                    case 30: tierName = "Ruby I"; break;
                    default: tierName = ""; break;
                    }
                }

                RecommendProblem recProblem = {id, title, tierName};
                recommendList.append(recProblem);

                qDebug() << "문제 추가:" << id << title << "(티어:" << tierName << ")";

                // 최대 5개만 가져오기
                if (recommendList.size() >= 5) {
                    qDebug() << "5개 문제 제한에 도달, 루프 종료";
                    break;
                }
            } else {
                qDebug() << "필요한 필드가 없는 문제 항목:" << problemObj;
            }
        }

        setRecommend(recommendList);

        if (recommendList.isEmpty()) {
            qDebug() << "추천 문제가 없습니다";
            statusLabel->setText("추천 문제가 없습니다.");
        } else {
            qDebug() << "총 " << recommendList.size() << "개의 추천 문제 로드 완료";
            //statusLabel->setText("추천 문제 로드 완료");
        }
    } else {
        qDebug() << "items 배열을 찾을 수 없습니다";
        statusLabel->setText("추천 문제를 찾을 수 없습니다.");
    }
}

void UserRecommend::handleNetworkError(const QString &errorMsg)
{
    qDebug() << "UserRecommend::handleNetworkError 호출됨:" << errorMsg;

    statusLabel->setText("오류: " + errorMsg);

    if (errorMsg == "NOT_FOUND") {
        statusLabel->setText("사용자를 찾을 수 없습니다.");
    }
}

void UserRecommend::setRecommend(const QList<RecommendProblem>& list)
{
    qDebug() << "UserRecommend::setRecommend 호출됨, 문제 수:" << list.size();

    listWidget->clear();

    for (const auto& p : list) {
        // 티어 정보 없이 제목만 표시
        //QString displayText = p.title;
        QString displayText = QString("%1 [%2]").arg(p.title, p.tier);
        QListWidgetItem* item = new QListWidgetItem(displayText);

        //QListWidgetItem* item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, p.id);  // 문제 번호
        listWidget->addItem(item);

        qDebug() << "리스트에 추가:" << p.id << p.title;
    }
}
