#ifndef USER_RECOMMEND_H
#define USER_RECOMMEND_H
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "networkmanager.h"

struct RecommendProblem {
    QString id; //문제 번호
    QString title; //문제 제목
    QString tier;; //난이도
};

class UserRecommend : public QWidget
{
    Q_OBJECT
public:
    explicit UserRecommend(QWidget* parent = nullptr);
    void setRecommend(const QList<RecommendProblem>& list);
    void setNetworkManager(NetworkManager* manager);
    void clearList();

public slots:
    void loadRecommendations(const QString& handle);

private slots:
    void handleRecommendDate(const QJsonObject &recommendData);
    void handleNetworkError(const QString &errorMsg);

private:
    QLabel* titleLabel;
    QListWidget* listWidget;
    NetworkManager* networkManager;
    QLabel* statusLabel;
    void setupConnections();

};
#endif // USER_RECOMMEND_H
