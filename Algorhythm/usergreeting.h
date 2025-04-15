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

class UserGreeting : public QWidget {
    Q_OBJECT
public:
    explicit UserGreeting(QWidget* parent = nullptr);
    void setUserName(const QString& name);
    void setTier(const QString& tierName, int solvedCount);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void showLoginDialog();
    void attemptLogin();
    void processUserData(const QJsonObject &userData);
    void handleNetworkError(const QString &errorMsg);

private:
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

    void logoutUser();
};
#endif // USERGREETING_H
