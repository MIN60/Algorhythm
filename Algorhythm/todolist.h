#ifndef TODOLIST_H
#define TODOLIST_H

#include<QListWidgetItem>
#include<QString>
#include<QLabel>

class TodoList
{
public:
    TodoList();
    ~TodoList();

    void saveTag(QLineEdit* tagEdit, const QString& filepath);  //tag
    void addTask(QListWidget* listWidget, const QString& taskText);  //할 일 저장
    void checkTask(QListWidgetItem* item); //다 한 일
    void deleteTask(QListWidget* listWidget, QWidget* taskWidget);

    void loadFromFile(QListWidget* listWidget, const QString& filepath, QLineEdit* tagEdit);
    void saveToFile(QListWidget* listWidget, const QString& filepath, const QString& tagText, const QDate& selectedDate);
    void updateTagDate(const QString& dateStr, const QString& tag);
    void searchTagDates(QLineEdit* input, QListWidget* resultList);


};

#endif // TODOLIST_H
