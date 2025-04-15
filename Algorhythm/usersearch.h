#ifndef USER_SEARCH_H
#define USER_SEARCH_H

#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>

struct Problem {
    QString id;
    QString title;
    QString problemtier;
};

class UserSearch : public QWidget
{
    Q_OBJECT

public:
    explicit UserSearch(QWidget* parent = nullptr);

private:
    QLineEdit* searchInput;
    QListWidget* resultList;
    std::vector<Problem> problem;

    void setupUI();
    void setupData();
    void filterList(const QString& input);
};

#endif // USER_SEARCH_H
