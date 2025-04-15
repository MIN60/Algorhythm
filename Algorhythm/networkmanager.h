#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    // API 요청 함수
    void userData(const QString &handle); // user 정보
    void problemData(const QString &problemId);  // 문제
    void graphData(const QString &handle); // 활동 통계
    void recommendData(const QString &handle);  // 추천 문제

signals:
    void onUserDataReceived(const QJsonObject &userData);
    void onProblemDataReceived(const QJsonObject &problemData);
    void onGraphDataReceived(const QJsonObject &graphData);
    void onRandomProblemsReceived(const QJsonObject &randomData);
    void onNetworkError(const QString &errorMsg);

private slots:
    void handleNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;

    void setupRequest(QNetworkRequest &request);
};

#endif // NETWORKMANAGER_H
