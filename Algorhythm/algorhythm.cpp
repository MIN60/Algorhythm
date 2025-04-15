#include "algorhythm.h"
#include "./ui_algorhythm.h"

Algorhythm::Algorhythm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Algorhythm)
{
    ui->setupUi(this);

    ui->user_greeting->setUserName("로그인이 필요합니다");
    ui->user_greeting->setTier("Gold I", 1234);
    qDebug() << ui->user_search->isVisible();


    // 추천 문제 더미
    QList<RecommendProblem> recList = {
        { "1260", "DFS와 BFS", "실버 2" },
        { "11725", "트리의 부모 찾기", "실버 1" },
        { "1753", "최단경로", "골드 5" }
    };

    ui->user_recommend->setRecommend(recList);


}

Algorhythm::~Algorhythm()
{
    delete ui;
}
