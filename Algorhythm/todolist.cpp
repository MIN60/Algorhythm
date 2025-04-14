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
}

void TodoList::loadFromFile(QListWidget* listWidget, const QString& filepath, QLineEdit* tagEdit){
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        listWidget->clear();
        tagEdit->clear();

        // 오늘 날짜는 경고 안띄워야됨
        if (QDate::fromString(QFileInfo(filepath).completeBaseName(), "yyyy-MM-dd") != QDate::currentDate()) {
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
        layout->setContentsMargins(0, 0, 0, 0);

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

