#include "todolist.h"
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>

// ToDoList 기능 구현
TodoList::TodoList(){


}

TodoList::~TodoList(){


}

void TodoList::saveTag(QLineEdit* tagEdit, const QString& filepath){
    QString tagText = tagEdit->text().trimmed();

    if (tagText.isEmpty()) {
        QMessageBox::warning(nullptr, "태그 없음", "태그를 입력해주세요!");
        return;
    }

    QJsonObject obj;
    obj["tag"] = tagText;

    QJsonDocument doc(obj);
    QFile file(filepath);

    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        QMessageBox::information(nullptr, "저장 완료", "저장되었습니다!");
    } else {
        QMessageBox::warning(nullptr, "저장 실패", "저장에 실패했습니다.");
    }


}
void TodoList::addTask(QListWidget* listWidget, const QString& taskText){
    if (taskText.isEmpty()) {
        QMessageBox::warning(nullptr, "경고", "할 일을 입력하세요.");
        return;
    }

    QListWidgetItem* item = new QListWidgetItem(listWidget);
    item->setSizeHint(QSize(0,40)); // 리스트 높이값 지정

    // 할 일 아래 행에 추가
    QWidget* taskWidget = new QWidget;
    QBoxLayout* layout = new QHBoxLayout(taskWidget);
    //layout->setContentsMargins(0,0,0,0);

    QCheckBox* checkbox = new QCheckBox(taskText, taskWidget);
    QPushButton* delButton = new QPushButton(QIcon::fromTheme("edit-delete"), "", taskWidget); // 기본 삭제 아이콘, 추후 변경
    delButton->setFixedSize(24,24);

    layout->addWidget(checkbox);
    layout->addWidget(delButton);
    taskWidget->setLayout(layout);

    listWidget->setItemWidget(item, taskWidget);
    isModified = true;

    QObject::connect(checkbox, &QCheckBox::checkStateChanged, [checkbox](int state){
        QFont font = checkbox->font();
        font.setStrikeOut(state==Qt::Checked);
        checkbox->setFont(font);

    });


    QObject::connect(delButton, &QPushButton::clicked, [=](){
        deleteTask(listWidget, taskWidget);
    });

}


void TodoList::deleteTask(QListWidget* listWidget, QWidget* taskWidget){
    for(int i=0; i<listWidget->count(); ++i){
        QListWidgetItem* item = listWidget->item(i);
        if(listWidget->itemWidget(item) == taskWidget){
            delete listWidget->takeItem(i); //메모리 제거
            break;
        }
    }
    isModified = true;
}


// 날짜에 해당되는 tag, todo 삭제
void TodoList::clearTasks(QListWidget* listWidget, QLineEdit* tagEdit, const QDate& date) {
    listWidget->clear();
    tagEdit->clear();

    QString dirPath = QDir::currentPath() + "/todo";
    QString filepath = dirPath + "/" + date.toString("yyyy-MM-dd") + ".json";

    if (QFile::exists(filepath)) {
        QFile::remove(filepath);
    }
    //json에서도 제거
    QString tagPath = QDir::currentPath() + "/todo/tag_list.json";
    QFile tagFile(tagPath);

    if (tagFile.open(QIODevice::ReadOnly)) {
        QByteArray data = tagFile.readAll();
        tagFile.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonArray tagArray = doc.array();

        for (int i = 0; i < tagArray.size(); ++i) {
            QJsonObject obj = tagArray[i].toObject();
            if (obj["date"].toString() == date.toString("yyyy-MM-dd")) {
                tagArray.removeAt(i);
                break;
            }
        }

        if (tagFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            tagFile.write(QJsonDocument(tagArray).toJson());
            tagFile.close();
        }
    }
}


void TodoList::loadFromFile(QListWidget* listWidget, const QString& filepath, QLineEdit* tagEdit){
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        listWidget->clear();
        tagEdit->clear();

        QDate fileDate = QDate::fromString(QFileInfo(filepath).completeBaseName(), "yyyy-MM-dd");

        if (fileDate != QDate::currentDate() && !isModified) {
            QMessageBox::warning(nullptr, "새로운 TODO", "작성된 TODO가 없습니다.");
        }

        return;
    }


    QByteArray data = file.readAll();
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (doc.isNull()) {
        QMessageBox::warning(nullptr, "파싱 실패", "JSON 파싱에 실패했습니다.");
        return;
    }

    QJsonObject root = doc.object();

    QString dateStr = root["date"].toString();
    QString tagStr = root["tag"].toString();
    if (tagEdit) tagEdit->setText(tagStr);

    listWidget->clear();



    QJsonArray taskArray = root["tasks"].toArray();
    for (const auto& val : taskArray) {
        QJsonObject obj = val.toObject();
        QString taskText = obj["task"].toString();
        bool done = obj["done"].toBool();

        QListWidgetItem* item = new QListWidgetItem(listWidget);
        item->setSizeHint(QSize(0, 40));

        QWidget* taskWidget = new QWidget;
        QHBoxLayout* layout = new QHBoxLayout(taskWidget);
        layout->setContentsMargins(10, 0, 10, 0);

        QCheckBox* checkBox = new QCheckBox(taskText);
        checkBox->setChecked(done);

        QFont font = checkBox->font();
        font.setStrikeOut(done);
        checkBox->setFont(font);

        QPushButton* delButton = new QPushButton(QIcon::fromTheme("edit-delete"), "");
        delButton->setFixedSize(24, 24);

        layout->addWidget(checkBox);
        layout->addWidget(delButton);
        listWidget->setItemWidget(item, taskWidget);

        QObject::connect(delButton, &QPushButton::clicked, [=]() {
            deleteTask(listWidget, taskWidget);
        });

        QObject::connect(checkBox, &QCheckBox::checkStateChanged, [=](int state) {
            QFont font = checkBox->font();
            font.setStrikeOut(state == Qt::Checked);
            checkBox->setFont(font);
        });
    }
    isModified = false;

}
void TodoList::saveToFile(QListWidget* listWidget, const QString& filepath, const QString& tagText, const QDate& selectedDate){
    QJsonArray taskArray;

    for(int i=0; i< listWidget->count(); ++i){
        QListWidgetItem* item = listWidget->item(i);
        QWidget* widget = listWidget->itemWidget(item);

        if(widget){
            QCheckBox* checkbox = widget->findChild<QCheckBox*>();
            if(checkbox){
                QJsonObject taskob;
                taskob["task"] = checkbox->text();
                taskob["done"] = checkbox->isChecked();
                taskArray.append(taskob);
            }
        }
    }

    QJsonObject root;
    root["date"] = selectedDate.toString("yyyy-MM-dd");
    root["tag"] = tagText;
    root["tasks"] = taskArray;

    QJsonDocument doc(root);
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        QMessageBox::information(nullptr, "저장 완료", "할 일이 저장되었습니다!");
        isModified = false;
    } else {
        QMessageBox::warning(nullptr, "저장 실패", "파일을 저장할 수 없습니다.");
    }
}


void TodoList::updateTagDate(const QString& dateStr, const QString& tag){
    QString path = QDir::currentPath() + "/todo/tag_list.json";
    QFile file(path);

    QJsonArray tagArray;

    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        tagArray = doc.array();
        file.close();
    }
    // gk.. 똑같은 날짜 중복 제거
    QJsonArray updatedArray;
    for (const auto& val : tagArray) {
        QJsonObject obj = val.toObject();
        if (obj["date"].toString() != dateStr) {
            updatedArray.append(obj);
        }
    }

    QJsonObject newTag;
    newTag["date"] = dateStr;
    newTag["tag"] = tag;
    updatedArray.append(newTag);

    // 저장
    QJsonDocument saveDoc(updatedArray);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(saveDoc.toJson());
        file.close();
    }
}

void TodoList::searchTagDates(QLineEdit* input, QListWidget* resultList)
{
    QString keyword = input->text().trimmed();
    if (keyword.isEmpty()) {
        QMessageBox::information(nullptr, "알림", "검색할 태그를 입력하세요.");
        return;
    }

    QString path = QDir::currentPath() + "/todo/tag_list.json";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "파일 열기 실패", "파일을 열 수 없습니다.");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        QMessageBox::warning(nullptr, "파싱 실패", "JSON 파싱에 실패했습니다.");
        return;
    }

    resultList->clear();
    QJsonArray array = doc.array();

    for (const auto& val : array) {
        QJsonObject obj = val.toObject();
        QString tag = obj["tag"].toString();
        QString date = obj["date"].toString();

        if (tag == keyword) {
            resultList->addItem(date);
        }
    }

    if (resultList->count() == 0) {
        resultList->addItem("해당 태그를 가진 날짜가 없습니다.");
    }
}

void TodoList::handleChangeDate(QCalendarWidget* calendar, QListWidget* listWidget, QLineEdit* tagEdit) {
    QDate selectedDate = calendar->selectedDate();

    if (isModified) {
        int result = QMessageBox::warning(nullptr, "저장되지 않음",
                                          "저장하지 않은 작업이 있습니다. 그래도 이동하시겠습니까?",
                                          QMessageBox::Yes | QMessageBox::No);

        if (result == QMessageBox::No) {
            calendar->setSelectedDate(curSelectedDate);  // 이전 날짜로 복구
            return;
        }
    }

    isModified = false;
    curSelectedDate = selectedDate;

    QString dirPath = QDir::currentPath() + "/todo";
    QString filepath = dirPath + "/" + selectedDate.toString("yyyy-MM-dd") + ".json";
    loadFromFile(listWidget, filepath, tagEdit);
}

void TodoList::handleAddTask(QListWidget* listWidget, QLineEdit* taskEdit, QLineEdit* tagEdit)
{
    QString taskText = taskEdit->text().trimmed();
    if (taskText.isEmpty()) {
        QMessageBox::warning(nullptr, "경고", "할 일을 입력해주세요!");
        return;
    }

    if (!isTagValid(tagEdit)) return;

    addTask(listWidget, taskText);
    taskEdit->clear();
}


void TodoList::handleSave(QListWidget* listWidget, QLineEdit* tagEdit, const QDate& selectedDate)
{
    if (!isTagValid(tagEdit)) return;

    QString filepath = getTodoPath(selectedDate);
    QString tagText = tagEdit->text().trimmed();

    saveToFile(listWidget, filepath, tagText, selectedDate);
    updateTagDate(selectedDate.toString("yyyy-MM-dd"), tagText);
}



QString TodoList::getTodoPath(const QDate& date) const {
    QString dirPath = QDir::currentPath() + "/todo";
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return dirPath + "/" + date.toString("yyyy-MM-dd") + ".json";
}

bool TodoList::isTagValid(QLineEdit* tagEdit) const {
    if (!tagEdit) return false;
    QString tagText = tagEdit->text().trimmed();
    if (tagText.isEmpty()) {
        QMessageBox::warning(nullptr, "태그 없음", "먼저 태그를 입력하세요.");
        return false;
    }
    return true;
}

void TodoList::moveDate(const QDate& date, QCalendarWidget* calendar, QListWidget* listWidget, QLineEdit* tagEdit)
{
    if (!date.isValid()) {
        QMessageBox::warning(nullptr, "날짜 오류", "날짜 데이터에 오류가 있습니다.");
        return;
    }

    calendar->setSelectedDate(date);

    QString filepath = getTodoPath(date);
    if (!QFile::exists(filepath)) {
        QMessageBox::information(nullptr, "알림", "해당 날짜의 TODO 파일이 존재하지 않습니다.");
        return;
    }

    loadFromFile(listWidget, filepath, tagEdit);
}



//ㅋ ㅐㄹ린더 꾸미기
void TodoList::applyCalendarStyle(QCalendarWidget* calendar)
{
    if (!calendar) return;

    calendar->setStyleSheet(R"(
    QCalendarWidget {
        background-color: #ffffff;
        border: none;
    }

    QCalendarWidget QWidget#qt_calendar_navigationbar {
        background-color: transparent;
        margin-bottom: 10px;
    }

    QCalendarWidget QToolButton {
        background-color: transparent;
        color: #333333;
        font-size: 16px;
        font-weight: bold;
        border: none;
    }

    QCalendarWidget QToolButton::menu-indicator {
        image: none;
    }

    QCalendarWidget QToolButton:hover {
        color: #4CAF50;
    }

    QCalendarWidget QTableView QHeaderView::section {
        background-color: transparent;
        color: #888888;
        font-size: 13px;
        font-weight: 600;
        border: none;
        padding-bottom: 5px;
    }

    QCalendarWidget QAbstractItemView:enabled {
        color: #2B2B2B;
        font-size: 14px;
        selection-background-color: #4CAF50;
        selection-color: #ffffff;
        gridline-color: #eeeeee;
    }

    QCalendarWidget QAbstractItemView::item:selected {
        background-color: #116329;
        color: white;
        border-radius: 5px;
    }


    )");
}


void TodoList::applyButtonStyle(QPushButton* button)
{
    if (!button) return;

    //button->setMinimumSize(100, 30);

    button->setStyleSheet(R"(
        QPushButton {
            background-color: #4AC26B;
            color: white;
            border: none;
            border-radius: 4px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #3FA75C;
        }

        QPushButton:pressed {
            background-color: #368E50;
        }
    )");
}


