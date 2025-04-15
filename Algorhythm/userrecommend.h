#ifndef USER_RECOMMEND_H
#define USER_RECOMMEND_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>

struct RecommendProblem {
    QString id; //문제 번호
    QString title; //문제 제목
    QString re_problemtier; //난이도
};


class UserRecommend : public QWidget
{
    Q_OBJECT

public:
    explicit UserRecommend(QWidget* parent = nullptr);
    void setRecommend(const QList<RecommendProblem>& list);

private:
    QLabel* titleLabel;
    QListWidget* listWidget;
};

#endif // USER_RECOMMEND_H
