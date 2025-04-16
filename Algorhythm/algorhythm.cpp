#include "algorhythm.h"
#include "./ui_algorhythm.h"
#include "todolist.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include "usergreeting.h"
#include "usersearch.h"

TodoList todo;


Algorhythm::Algorhythm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Algorhythm)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0); //유저탭으로 시작
    ui->user_greeting->setUserSearch(ui->user_search);

    networkManager = new NetworkManager(this);
    qDebug() << "user_chart:" << ui->user_chart;
    qDebug() << "[DEBUG] Algorhythm::networkManager is:" << networkManager;
    ui->user_chart->clearChart();

    // 네트워크 매니저 생성 및 설정
    networkManager = new NetworkManager(this);
    ui->user_search->setNetworkManager(networkManager);
    ui->user_recommend->setNetworkManager(networkManager);

    ui->user_greeting->setUserName("로그인이 필요합니다");

    // 차트
    connect(networkManager, &NetworkManager::onGraphDataReceived,
            ui->user_chart, &UserChart::drawChart);

    ui->user_greeting->setUserChart(ui->user_chart);

    qDebug() << "Algorhythm UI 설정 완료";
    qDebug() << "UserSearch 표시 상태:" << ui->user_search->isVisible();
    ui->user_greeting->setUserRecommend(ui->user_recommend);



    // 신호-슬롯 연결
    connect(ui->user_greeting, &UserGreeting::loginSuccess,
            ui->user_recommend, &UserRecommend::loadRecommendations);

    qDebug() << "신호-슬롯 연결 완료";


    applyTabStyle(ui->tabWidget);

    ui->USER->setStyleSheet("background-color: white;");
    ui->TODO->setStyleSheet("background-color: white;");
    this->setStyleSheet("background-color: #F9FAFB;");

    timeTimer = new QTimer(this);
    connect(timeTimer, &QTimer::timeout, this, &Algorhythm::updateTimeLabel);
    timeTimer->start(1000);
    updateTimeLabel();

    todo.applySectionStyle(ui->calender_item);
    todo.applySectionStyle(ui->todo_item);


    ui->lineEditTask->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->lineEditTask->setMinimumHeight(50);

    ui->pushButtonAdd->setFixedWidth(100);
    ui->pushButtonAdd->setMinimumHeight(50);

    // 시작 시 기존 todo있으면 불러오기
    QDate today = ui->calendarWidget->selectedDate();
    QString dirPath = QDir::currentPath() + "/todo";
    QString filepath = todo.getTodoPath(today);
    todo.loadFromFile(ui->listWidgetTasks, filepath, ui->today_tag);


    //todolist ui연결
    todo.applyButtonStyle(ui->pushButtonAdd);
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, [=]() {
        todo.handleAddTask(ui->listWidgetTasks, ui->lineEditTask, ui->today_tag);
    });


    // 저장
    todo.applyButtonStyle(ui->pushButtonSave);
    connect(ui->pushButtonSave, &QPushButton::clicked, this, [=]() {
        todo.handleSave(ui->listWidgetTasks, ui->today_tag, ui->calendarWidget->selectedDate());
    });


    //삭제
    connect(ui->pushButtonClear, &QPushButton::clicked, this, [=]() {
        int result = QMessageBox::question(this, "전체 삭제", "정말 삭제하시겠습니까?");
        if (result == QMessageBox::Yes) {
            QDate date = ui->calendarWidget->selectedDate();
            todo.clearTasks(ui->listWidgetTasks, ui->today_tag, date);
            QMessageBox::information(this, "삭제 완료", "삭제되었습니다.");
        }
    });

    connect(ui->tag_input, &QLineEdit::returnPressed, this, [=]() {
        todo.searchTagDates(ui->tag_input, ui->tag_search_result);
    });


    // 태그 검색시 더블클릭하면 이동하게
    connect(ui->tag_search_result, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem* item) {
        QDate date = QDate::fromString(item->text().trimmed(), "yyyy-MM-dd");
        todo.moveDate(date, ui->calendarWidget, ui->listWidgetTasks, ui->today_tag);
    });

    ui->tag_search_result->setToolTip("더블클릭하면 해당 날짜의 TODO로 이동합니다");


    // 그냥 이동하면 경고
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, [=](const QDate &date) {
        todo.handleChangeDate(ui->calendarWidget, ui->listWidgetTasks, ui->today_tag);
    });

    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    todo.applyCalendarStyle(ui->calendarWidget);

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


void Algorhythm::applyTabStyle(QTabWidget* tabWidget)
{
    if (!tabWidget) return;

    tabWidget->setStyleSheet(R"(
        QTabWidget::pane {
            border: none;
        }

        QTabBar::tab {
            background: #E5F0F8;
            color: #90A2B6;
            font-weight: bold;
            padding: 8px 24px;
            margin-right: 4px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
        }

        QTabBar::tab:selected {
            background: #116329;
            color: white;
        }
    )");
}

void Algorhythm::updateTimeLabel()
{
    QDateTime now = QDateTime::currentDateTime();
    QString timeStr = now.toString("MM/dd   hh:mm AP");
    ui->timeLabel->setText(timeStr);
}


Algorhythm::~Algorhythm()
{
    delete ui;
}
