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

    // 시작 시 기존 todo있으면 불러오기
    QDate today = ui->calendarWidget->selectedDate();
    QString dirPath = QDir::currentPath() + "/todo";
    QString filepath = todo.getTodoPath(today);
    todo.loadFromFile(ui->listWidgetTasks, filepath, ui->today_tag);


    //todolist ui연결
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, [=](){
        QString taskText = ui->lineEditTask->text();
        if (taskText.isEmpty()) {
            QMessageBox::warning(this, "경고", "할 일을 입력해주세요!");
            return;
        }

        // 태그가 비어있으면 경고
        if (ui->today_tag->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "태그 없음", "먼저 태그를 입력해주세요!");
            return;
        }

        todo.addTask(ui->listWidgetTasks, taskText);
        ui->lineEditTask->clear();
    });

    // 저장
    connect(ui->pushButtonSave, &QPushButton::clicked, this, [=]() {
        QDate date = ui->calendarWidget->selectedDate();
        if (!todo.isTagValid(ui->today_tag)) return;

        QString filepath = todo.getTodoPath(date);
        QString tagText = ui->today_tag->text().trimmed();

        todo.saveToFile(ui->listWidgetTasks, filepath, tagText, date);
        todo.updateTagDate(date.toString("yyyy-MM-dd"), tagText);
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
        QString dateStr = item->text().trimmed();
        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");

        if (!date.isValid()) {
            QMessageBox::warning(this, "날짜 오류", "날짜 데이터에 오류가 있습니다.");
            return;
        }

        ui->calendarWidget->setSelectedDate(date);
        QString dirPath = QDir::currentPath() + "/todo";
        QString filepath = todo.getTodoPath(date);

        if (!QFile::exists(filepath)) {
            QMessageBox::information(this, "알림", "해당 날짜의 TODO 파일이 존재하지 않습니다.");
            return;
        }

        todo.loadFromFile(ui->listWidgetTasks, filepath, ui->today_tag);
    });

    // 그냥 이동하면 경고
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, [=](const QDate &date) {
        todo.handleChangeDate(ui->calendarWidget, ui->listWidgetTasks, ui->today_tag);
    });




}

Algorhythm::~Algorhythm()
{
    delete ui;
}
