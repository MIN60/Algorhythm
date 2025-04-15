#ifndef TODOLIST_H
#define TODOLIST_H

#include<QListWidgetItem>
#include<QString>
#include<QLabel>
#include <QCalendarWidget>
#include <QPushButton>

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

    void clearTasks(QListWidget* listWidget, QLineEdit* tagEdit, const QDate& date); // 삭제버튼 기능
    void handleChangeDate(QCalendarWidget* calendar, QListWidget* listWidget, QLineEdit* tagEdit); //날짜 이동

    QString getTodoPath(const QDate& date) const;
    bool isTagValid(QLineEdit* tagEdit) const;
    void moveDate(const QDate& date, QCalendarWidget* calendar, QListWidget* listWidget, QLineEdit* tagEdit);
    void handleAddTask(QListWidget* listWidget, QLineEdit* taskEdit, QLineEdit* tagEdit);
    void handleSave(QListWidget* listWidget, QLineEdit* tagEdit, const QDate& selectedDate);

    void applyCalendarStyle(QCalendarWidget* calendar); // 캘꾸
    void applyButtonStyle(QPushButton* button);


private:
    bool isModified = false;
    QDate curSelectedDate = QDate::currentDate();


};

#endif // TODOLIST_H
