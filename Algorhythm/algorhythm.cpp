#include "algorhythm.h"
#include "./ui_algorhythm.h"
#include "todolist.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

TodoList todo;

Algorhythm::Algorhythm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Algorhythm)
{
    ui->setupUi(this);
    applyTabStyle(ui->tabWidget);

    ui->USER->setStyleSheet("background-color: white;");
    ui->TODO->setStyleSheet("background-color: white;");

    this->setStyleSheet("background-color: #F9FAFB;");


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


    // 그냥 이동하면 경고
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, [=](const QDate &date) {
        todo.handleChangeDate(ui->calendarWidget, ui->listWidgetTasks, ui->today_tag);
    });

    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    todo.applyCalendarStyle(ui->calendarWidget);


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



Algorhythm::~Algorhythm()
{
    delete ui;
}
