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

    // 로그인 버튼 생성 (위치 변경)
    loginButtonMain = new QPushButton("로그인", clickableArea);
    loginButtonMain->setFixedWidth(120); // 버튼 크기 조정
    loginButtonMain->setStyleSheet("font-size: 12pt; padding: 5px;");

    // clickableArea의 레이아웃에 라벨 추가
    clickableLayout->addWidget(greetingLabel);
    clickableLayout->addWidget(nameLabel);
    clickableLayout->addWidget(loginButtonMain); // 버튼 위치 변경
    clickableLayout->addStretch();
    clickableLayout->setContentsMargins(10, 10, 10, 10);

    tierCard = new UserTierCard(this);
    tierCard->setTier("");
    tierCard->setMaximumWidth(250);

    // 메인 레이아웃에 clickableArea 추가
    layout->addWidget(clickableArea); // 왼쪽 인사
    layout->addWidget(tierCard);  // 오른쪽 티어
    layout->setSpacing(40);
    layout->setContentsMargins(15, 15, 15, 15);
    loginDialog = new QDialog(this);
    loginDialog->setWindowTitle("solved.ac 로그인");
    loginDialog->setMinimumWidth(300);

    QVBoxLayout* dialogLayout = new QVBoxLayout(loginDialog);

    QLabel* usernameLabel = new QLabel("solved.ac에 연동된 백준 아이디를 입력하세요:", loginDialog);
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
    connect(loginButtonMain, &QPushButton::clicked, this, &UserGreeting::handleMainButtonClick);
}


// 티어에 따른 이미지 설정
void UserGreeting::updateTierImage(const QString& tierName)
{
    // 티어에 따른 이미지 파일명 결정
    QString imagePath;

    for(int i =0; i<5; i++){
    if (tierName.contains("Bronze")) {
        imagePath = ":/images/tier/bronze.png";
    } else if (tierName.contains("Silver")) {
        imagePath = ":/images/tier/silver.png";
    } else if (tierName.contains("Gold")) {
        imagePath = ":/images/tier/gold.png";
    } else if (tierName.contains("Platinum")) {
        imagePath = ":/images/tier/platinum.png";
    } else if (tierName.contains("Diamond")) {
        imagePath = ":/images/tier/diamond.png";
    } else if (tierName.contains("Ruby")) {
        imagePath = ":/images/tier/ruby.png";
    } else {
        imagePath = ":/images/tier/unrated.png";
    }
}

    // 이미지 로드 및 설정
    QPixmap tierImage(imagePath);
    if (tierImage.isNull()) {
        // 이미지가 없을 경우 대체 표시
        //tierImageLabel->setText(tierName);
        //tierImageLabel->setStyleSheet("background-color: #FFD700; color: white; font-size: 18pt; font-weight: bold;");
    } else {
        //tierImageLabel->setPixmap(tierImage.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        //tierImageLabel->setStyleSheet("");
    }
}

bool UserGreeting::eventFilter(QObject *watched, QEvent *event)
{
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
        currentUsername = username;

        // 티어 및 문제 해결 수 설정
        int tier = userData.contains("tier") ? userData["tier"].toInt() : 0;
        int solvedCount = userData.contains("solvedCount") ? userData["solvedCount"].toInt() : 0;

        // 티어 번호를 이름으로 변환 (for문 사용)
        QString tierName = convertTierNumberToName(tier);

        // 티어 설정 (솔브드 카운트 인자 제거)
        setTier(tierName);

        // 다이얼로그 닫기
        loginDialog->accept();

        // 로그인 성공 시그널 발생
        emit loginSuccess(username);

        // 버튼 텍스트 변경 (hide 대신)
        loginButtonMain->setText("로그아웃");
        loginButtonMain->setStyleSheet("font-size: 12pt; padding: 5px; background-color: #A1DEB2; color: white;");

        //차트
        if (userChart) {
            userChart->setUsername(username);
        }

    } else {
        // 로그인 실패
        statusLabel->setText("사용자를 찾을 수 없습니다.");
    }


}

void UserGreeting::handleMainButtonClick()
{
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
}

void UserGreeting::handleNetworkError(const QString &errorMsg)
{
    if (loginDialog->isVisible()) {
        statusLabel->setText("아이디를 다시 확인해주세요.");
    } else {

        QMessageBox::critical(this, "네트워크 오류", "연결 오류: " + errorMsg);
    }
}

QString UserGreeting::convertTierNumberToName(int tier)
{
    // Unrated 처리
    if (tier == 0) {
        return "Unrated";
    }

    // 티어 범위와 이름 정의
    int tierStarts[] = {1, 6, 11, 16, 21, 26, 31};
    QString tierNames[] = {"Bronze", "Silver", "Gold", "Platinum", "Diamond", "Ruby", "Master"};

    // for문으로 티어 범위 확인
    for (int i = 0; i < 7; i++) {
        // 마스터 티어 확인 (마지막 티어)
        if (i == 6 && tier >= tierStarts[i]) {
            return "Master";
        }
        // 다른 티어들 확인
        else if (i < 6 && tier >= tierStarts[i] && tier < tierStarts[i+1]) {
            int level;

            if (i == 0) level = 6 - tier;  // Bronze
            else if (i == 1) level = 11 - tier;  // Silver
            else if (i == 2) level = 16 - tier;  // Gold
            else if (i == 3) level = 21 - tier;  // Platinum
            else if (i == 4) level = 26 - tier;  // Diamond
            else level = 31 - tier;  // Ruby

            return tierNames[i] + " " + QString::number(level);
        }
    }

    // 범위 밖이면 기본값
    return "Unrated";
}

void UserGreeting::logoutUser()
{
    qDebug() << "UserGreeting::logoutUser 호출됨";

    loggedIn = false;
    currentUsername = "";
    nameLabel->setText("로그인이 필요합니다.");
    tierCard->setTier("Unrated");

    // 버튼 상태 되돌리기 (show 대신)
    loginButtonMain->setText("로그인");
    loginButtonMain->setStyleSheet("font-size: 12pt; padding: 5px;");

    // 차트 비우기
    if (userChart) {
        userChart->clearChart();
    }



    // 로그아웃 완료 시그널 발생
    qDebug() << "로그아웃 완료 시그널 발생";
    //emit logoutComplete();
}

void UserGreeting::setUserName(const QString& name)
{
    nameLabel->setText(name + "님!");
}

void UserGreeting::setTier(const QString& tierName)
{
    tierCard->setTier(tierName);

    // 티어 이미지 업데이트
    updateTierImage(tierName);
}


void UserGreeting::setUserChart(UserChart* chart)
{
    this->userChart = chart;
}
