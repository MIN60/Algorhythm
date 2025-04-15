#ifndef USERGREETING_H
#define USERGREETING_H
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "usertiercard.h"
#include "networkmanager.h"
#include "userchart.h"

class UserGreeting : public QWidget {
    Q_OBJECT
public:
    explicit UserGreeting(QWidget* parent = nullptr);
    void setUserName(const QString& name);
    void setTier(const QString& tierName);
    bool isLoggedIn() const { return loggedIn; }
    QString getCurrentUsername() const { return currentUsername; }

    void setUserChart(UserChart* chart);

signals:
    void loginSuccess(const QString &username);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    //void loadRecommendations(const QString& handle);

private slots:
    void showLoginDialog();
    void attemptLogin();
    void processUserData(const QJsonObject &userData);
    void handleNetworkError(const QString &errorMsg);
    void updateTierImage(const QString& tierName);
    void logoutUser();
    void handleMainButtonClick();

private:
    QString convertTierNumberToName(int tier);
    QLabel* greetingLabel;
    QLabel* nameLabel;
    UserTierCard* tierCard;
    QWidget* clickableArea;
    NetworkManager* networkManager;
    bool loggedIn;
    QString currentUsername;

    // 로그인 다이얼로그
    QDialog* loginDialog;
    QLineEdit* usernameInput;
    QPushButton* loginButton;
    QLabel* statusLabel;
    QPushButton* loginButtonMain;

    // 차트
    UserChart* userChart = nullptr;



};
#endif // USERGREETING_H
