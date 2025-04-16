#ifndef USER_SEARCH_H
#define USER_SEARCH_H
#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "networkmanager.h"

struct Problem {
    QString id;
    QString title;
    QString problemtier;
};

class UserSearch : public QWidget
{
    Q_OBJECT
public:
    // 기본 생성자 - UI 자동 생성용
    explicit UserSearch(QWidget* parent = nullptr);

    // NetworkManager를 받는 생성자
    void setNetworkManager(NetworkManager* networkManager);
    void clearSearchResult();

private slots:
    void handleProblemData(const QJsonObject &problemData);
    void handleNetworkError(const QString &errorMsg);

private:
    QLineEdit* searchInput;
    QListWidget* resultList;
    std::vector<Problem> problem;
    NetworkManager* networkManager;
    QLabel* statusLabel;

    void setupUI();
    void setupData();
    void filterList(const QString& input);
    void searchProblem(const QString& problemId);
    void addProblemToList(const Problem& p);
    void setupConnections();

};
#endif // USER_SEARCH_H
