#include "networkmanager.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QSslConfiguration>
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &NetworkManager::handleNetworkReply);
}

NetworkManager::~NetworkManager() = default;

void NetworkManager::setupRequest(QNetworkRequest &request)
{
    QSslConfiguration ssl = QSslConfiguration::defaultConfiguration();
    ssl.setProtocol(QSsl::TlsV1_2OrLater);
    request.setSslConfiguration(ssl);
    request.setRawHeader("User-Agent", "AlgorhythmClient/1.0");
}

void NetworkManager::userData(const QString &handle)
{
    QUrl url("https://solved.ac/api/v3/user/show");
    QUrlQuery query;
    query.addQueryItem("handle", handle);
    url.setQuery(query);

    QNetworkRequest request(url);
    setupRequest(request);
    manager->get(request);
}

void NetworkManager::problemData(const QString &problemId)
{
    QUrl url("https://solved.ac/api/v3/problem/show");
    QUrlQuery query;
    query.addQueryItem("problemId", problemId);
    url.setQuery(query);

    QNetworkRequest request(url);
    setupRequest(request);
    manager->get(request);
}

void NetworkManager::graphData(const QString &handle)
{
    QUrl url("https://solved.ac/api/v3/user/contribution_stats");
    QUrlQuery query;
    query.addQueryItem("handle", handle);
    url.setQuery(query);

    QNetworkRequest request(url);
    setupRequest(request);
    manager->get(request);
}

void NetworkManager::recommendData(const QString &handle)
{
    QUrl url("https://solved.ac/api/v3/user/top_100");
    QUrlQuery query;
    query.addQueryItem("handle", handle);
    url.setQuery(query);

    QNetworkRequest request(url);
    setupRequest(request);
    manager->get(request);
}

void NetworkManager::handleNetworkReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "[Network Error]" << reply->errorString();
        emit onNetworkError(reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        emit onNetworkError("Invalid JSON format");
        reply->deleteLater();
        return;
    }

    QJsonObject obj = doc.object();
    QString path = reply->request().url().path();

    if (path.contains("user/show")) {
        emit onUserDataReceived(obj);
    } else if (path.contains("problem/show")) {
        emit onProblemDataReceived(obj);
    } else if (path.contains("contribution_stats")) {
        emit onGraphDataReceived(obj);
    } else if (path.contains("top_100")) {
        emit onRandomProblemsReceived(obj);
    }

    reply->deleteLater();
}
