#include "algorhythm.h"
#include "./ui_algorhythm.h"

Algorhythm::Algorhythm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Algorhythm)
{
    ui->setupUi(this);

    // 네트워크 매니저 생성 및 설정
    networkManager = new NetworkManager(this);
    ui->user_search->setNetworkManager(networkManager);
    ui->user_recommend->setNetworkManager(networkManager);

    ui->user_greeting->setUserName("로그인이 필요합니다");
    //ui->user_greeting->setTier("Gold I", 1234);

    qDebug() << "Algorhythm UI 설정 완료";
    qDebug() << "UserSearch 표시 상태:" << ui->user_search->isVisible();

    // 새로운 방식으로 신호-슬롯 연결 (Qt5 방식)
    connect(ui->user_greeting, &UserGreeting::loginSuccess,
            ui->user_recommend, &UserRecommend::loadRecommendations);

    qDebug() << "신호-슬롯 연결 완료";

    // 로그인 여부에 따라 UserRecommend 위젯 표시/숨김 설정
    if (ui->user_greeting->isLoggedIn()) {
        qDebug() << "이미 로그인된 상태: 추천 문제 로드 시도";
        // 이미 로그인된 경우 추천 문제 로드
        QString username = ui->user_greeting->getCurrentUsername();
        qDebug() << "현재 사용자명:" << username;
        ui->user_recommend->loadRecommendations(username);
    } else {
        qDebug() << "로그인되지 않은 상태: 추천 문제 위젯 준비";
        // 로그인되지 않은 경우에도 위젯은 표시하되 안내 메시지만 표시
        // ui->user_recommend->hide(); // 숨기고 싶은 경우 주석 해제
    }

    qDebug() << "Algorhythm 초기화 완료";
}

Algorhythm::~Algorhythm()
{
    delete ui;
}
