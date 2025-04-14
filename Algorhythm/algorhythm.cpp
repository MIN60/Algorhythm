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



}

Algorhythm::~Algorhythm()
{
    delete ui;
}
