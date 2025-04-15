#include "usergreeting.h"
#include <QMessageBox>
#include <QMouseEvent>

UserGreeting::UserGreeting(QWidget* parent)
    : QWidget(parent), loggedIn(false), currentUsername("")
{
    networkManager = new NetworkManager(this);
    QHBoxLayout* layout = new QHBoxLayout(this);

    // 클릭 가능한 영역 생성
    clickableArea = new QWidget(this);
    clickableArea->setCursor(Qt::PointingHandCursor);
    clickableArea->setToolTip("클릭하여 로그인하세요");
    clickableArea->installEventFilter(this);

    // clickableArea에 대한 레이아웃 생성
    QVBoxLayout* clickableLayout = new QVBoxLayout(clickableArea);

    // 라벨들을 clickableArea의 자식으로 생성
    greetingLabel = new QLabel("안녕하세요,", clickableArea);
    greetingLabel->setStyleSheet("font-size: 20pt;");
    nameLabel = new QLabel("로그인이 필요합니다.", clickableArea);
    nameLabel->setStyleSheet("font-size: 30pt; font-weight: bold; color: #2E7D32;");

    // clickableArea의 레이아웃에 라벨 추가
    clickableLayout->addWidget(greetingLabel);
    clickableLayout->addWidget(nameLabel);
    clickableLayout->addStretch();
    clickableLayout->setContentsMargins(10, 10, 10, 10);

    tierCard = new UserTierCard(this);
    tierCard->setTier("Gold I", 1234);
    tierCard->setMaximumWidth(250);

    // 메인 레이아웃에 clickableArea 추가
    layout->addWidget(clickableArea); // 왼쪽 인사
    layout->addWidget(tierCard);  // 오른쪽 티어
    layout->setSpacing(40);
    layout->setContentsMargins(15, 15, 15, 15);
    loginDialog = new QDialog(this);
    loginDialog->setWindowTitle("Solved.ac 로그인");
    loginDialog->setMinimumWidth(300);

    QVBoxLayout* dialogLayout = new QVBoxLayout(loginDialog);

    QLabel* usernameLabel = new QLabel("백준 아이디를 입력하세요:", loginDialog);
    usernameInput = new QLineEdit(loginDialog);

    loginButton = new QPushButton("로그인", loginDialog);
    statusLabel = new QLabel(loginDialog);
    statusLabel->setStyleSheet("color: red;");

    dialogLayout->addWidget(usernameLabel);
    dialogLayout->addWidget(usernameInput);
    dialogLayout->addWidget(loginButton);
    dialogLayout->addWidget(statusLabel);

    // 시그널 연결
    connect(loginButton, &QPushButton::clicked, this, &UserGreeting::attemptLogin);
    connect(usernameInput, &QLineEdit::returnPressed, this, &UserGreeting::attemptLogin);
    connect(networkManager, &NetworkManager::onUserDataReceived, this, &UserGreeting::processUserData);
    connect(networkManager, &NetworkManager::onNetworkError, this, &UserGreeting::handleNetworkError);
}
bool UserGreeting::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == clickableArea && event->type() == QEvent::MouseButtonPress) {
        if (loggedIn) {
            // 로그아웃 확인
            QMessageBox::StandardButton reply = QMessageBox::question(
                this, "로그아웃", "로그아웃 하시겠습니까?",
                QMessageBox::Yes | QMessageBox::No
                );

            if (reply == QMessageBox::Yes) {
                logoutUser();
            }
        } else {
            // 로그인 다이얼로그 표시
            showLoginDialog();
        }
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void UserGreeting::showLoginDialog()
{
    // 다이얼로그 초기화
    usernameInput->clear();
    statusLabel->clear();

    // 다이얼로그 표시
    loginDialog->exec();
}

void UserGreeting::attemptLogin()
{
    QString username = usernameInput->text().trimmed();
    if (username.isEmpty()) {
        statusLabel->setText("사용자 이름을 입력해주세요!");
        return;
    }

    // NetworkManager를 통해 사용자 데이터 요청
    currentUsername = username;
    networkManager->userData(username);
}

void UserGreeting::processUserData(const QJsonObject &userData)
{
    if (userData.contains("handle")) {
        // 로그인 성공
        loggedIn = true;

        // UI 업데이트
        QString username = userData["handle"].toString();
        setUserName(username);

        // 티어 및 문제 해결 수 설정
        int tier = userData.contains("tier") ? userData["tier"].toInt() : 0;
        int solvedCount = userData.contains("solvedCount") ? userData["solvedCount"].toInt() : 0;

        // 티어 번호를 이름으로 변환
        QString tierName;
        switch (tier) {
        case 0: tierName = "Unrated"; break;
        case 1: case 2: case 3: case 4: case 5: tierName = "Bronze " + QString::number(6 - tier); break;
        case 6: case 7: case 8: case 9: case 10: tierName = "Silver " + QString::number(11 - tier); break;
        case 11: case 12: case 13: case 14: case 15: tierName = "Gold " + QString::number(16 - tier); break;
        case 16: case 17: case 18: case 19: case 20: tierName = "Platinum " + QString::number(21 - tier); break;
        case 21: case 22: case 23: case 24: case 25: tierName = "Diamond " + QString::number(26 - tier); break;
        case 26: case 27: case 28: case 29: case 30: tierName = "Ruby " + QString::number(31 - tier); break;
        default: tierName = "Master"; break;
        }

        setTier(tierName, solvedCount);

        // 다이얼로그 닫기
        loginDialog->accept();

        // 툴팁 업데이트
        clickableArea->setToolTip("클릭하여 로그아웃하세요");
    } else {
        // 로그인 실패
        statusLabel->setText("사용자를 찾을 수 없습니다.");
    }
}

void UserGreeting::handleNetworkError(const QString &errorMsg)
{
    if (loginDialog->isVisible()) {
        statusLabel->setText("네트워크 오류: " + errorMsg);
    } else {
        QMessageBox::critical(this, "네트워크 오류", "연결 오류: " + errorMsg);
    }
}

void UserGreeting::logoutUser()
{
    loggedIn = false;
    currentUsername = "";
    nameLabel->setText("로그인이 필요합니다.");
    tierCard->setTier("Unrated", 0);
    clickableArea->setToolTip("클릭하여 로그인하세요");
}
void UserGreeting::setUserName(const QString& name)
{
    nameLabel->setText(name + "님!");
}

void UserGreeting::setTier(const QString& tierName, int solvedCount)
{
    tierCard->setTier(tierName, solvedCount);
}
