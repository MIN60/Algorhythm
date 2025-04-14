#include "algorhythm.h"
#include "./ui_algorhythm.h"
#include "todolist.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

TodoList todo;

Algorhythm::Algorhythm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Algorhythm)
{
    ui->setupUi(this);

    // 시작 시 기존 todo있으면 불러오기
    QDate today = ui->calendarWidget->selectedDate();
    QString dirPath = QDir::currentPath() + "/todo";
    QString filepath = dirPath + "/" + today.toString("yyyy-MM-dd") + ".json";
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
    connect(ui->pushButtonExit, &QPushButton::clicked, this, [=](){
        QDate date = ui->calendarWidget->selectedDate();
        QString dirPath = QDir::currentPath() + "/todo";
        QDir dir(dirPath);

        if (!dir.exists()) {
            dir.mkpath(".");
        }

        QString filepath = dirPath + "/" + date.toString("yyyy-MM-dd") + ".json";
        todo.saveToFile(ui->listWidgetTasks, filepath, ui->today_tag->text().trimmed(), date);

    });



    // 캘린더 버튼 누르면 불러오기
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, [=](const QDate &date) {
        QString dirPath = QDir::currentPath() + "/todo";
        QString filepath = dirPath + "/" + date.toString("yyyy-MM-dd") + ".json";
        todo.loadFromFile(ui->listWidgetTasks, filepath, ui->today_tag);
    });

    connect(ui->pushButtonAddTag, &QPushButton::clicked, this, [=](){
        QString tagText = ui->today_tag->text().trimmed();
        if (tagText.isEmpty()) {
            QMessageBox::warning(this, "태그 없음", "태그를 입력해주세요!");
            return;
        }

        QDate date = ui->calendarWidget->selectedDate();
        QString dirPath = QDir::currentPath() + "/todo";
        QDir dir(dirPath);

        if (!dir.exists()) {
            dir.mkpath(".");  // 폴더 없으면 생성
        }

        QString filepath = dirPath + "/" + date.toString("yyyy-MM-dd") + ".json";
        qDebug() << "태그/할 일 저장 경로:" << filepath;

        todo.saveTag(ui->today_tag, filepath);
        todo.updateTagDate(date.toString("yyyy-MM-dd"), tagText);
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
        QString filepath = dirPath + "/" + dateStr + ".json";

        if (!QFile::exists(filepath)) {
            QMessageBox::information(this, "알림", "해당 날짜의 TODO 파일이 존재하지 않습니다.");
            return;
        }

        todo.loadFromFile(ui->listWidgetTasks, filepath, ui->today_tag);
    });




}

Algorhythm::~Algorhythm()
{
    delete ui;
}
