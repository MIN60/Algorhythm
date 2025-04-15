#include "usertiercard.h"
#include <QSizePolicy>
#include <QPalette>
#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QPolygon>
#include <QtMath>
#include <QRegularExpression>

// Constructor implementation
UserTierCard::UserTierCard(QWidget* parent)
    : QWidget(parent)
{
    // 전체 레이아웃
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // 카드 프레임 생성 - 배경색을 흰색(#FFFFFF)으로 설정
    cardFrame = new QFrame(this);
    cardFrame->setFrameShape(QFrame::NoFrame);
    cardFrame->setStyleSheet("background-color: #FFFFFF; border-radius: 10px; padding: 10px;");
    cardFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // 카드 내부 레이아웃
    QVBoxLayout* cardLayout = new QVBoxLayout(cardFrame);
    cardLayout->setContentsMargins(10, 10, 10, 10);
    cardLayout->setSpacing(10);

    // 상단 "현재 나의 티어는?" 라벨 추가
    QLabel* titleLabel = new QLabel("현재 나의 티어는?", cardFrame);
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: black; text-align: center;");
    titleLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(titleLabel);

    // 티어 이미지 라벨
    tierImageLabel = new QLabel(cardFrame);
    tierImageLabel->setFixedSize(150, 150);
    tierImageLabel->setScaledContents(true);
    tierImageLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(tierImageLabel, 0, Qt::AlignCenter);

    // 티어 라벨
    tierLabel = new QLabel(cardFrame);
    tierLabel->setStyleSheet("font-size: 18pt; font-weight: bold; color: black; text-align: center;");
    tierLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(tierLabel, 0, Qt::AlignCenter);

    cardLayout->addStretch();

    // 카드 프레임을 메인 레이아웃에 추가
    layout->addWidget(cardFrame);
    setLayout(layout);

    // 티어 정보 초기화
    initializeTierImages();
}

// Missing setTier implementation
void UserTierCard::setTier(const QString& tierName)
{
    // 티어 라벨 설정
    tierLabel->setText(tierName);

    // 티어에 맞는 이미지 설정
    if (tierImages.contains(tierName)) {
        loadTierImage(tierName);
    } else {
        // 티어가 없으면 Unrated로 설정
        qDebug() << "알 수 없는 티어:" << tierName;
        loadTierImage("Unrated");
    }
}

// Missing setTierImage implementation
void UserTierCard::setTierImage(const QString& imagePath)
{
    // 경로 디버깅
    qDebug() << "이미지 경로: " << imagePath;

    // 티어 이미지 설정
    QPixmap pixmap(imagePath);
    qDebug() << "이미지 로드 성공 여부: " << !pixmap.isNull();

    if (!pixmap.isNull()) {
        tierImageLabel->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        tierImageLabel->setStyleSheet("");  // 스타일시트 초기화
    } else {
        // 이미지를 찾을 수 없는 경우 텍스트 표시
        tierImageLabel->setText("No Image");
        tierImageLabel->setStyleSheet("background-color: #cccccc; color: black; font-size: 10pt; text-align: center;");
    }
}

// Missing initializeTierImages implementation
void UserTierCard::initializeTierImages()
{
    // 리소스 기반 경로와 파일 기반 경로 모두 설정
    QString resourcePrefix = ":/images/tierqrc";
    QString fileBasePath = QCoreApplication::applicationDirPath() + "/images/tierqrc";

    // 디버깅을 위해 경로 출력
    qDebug() << "리소스 기본 경로:" << resourcePrefix;
    qDebug() << "파일 기본 경로:" << fileBasePath;

    // 파일 경로 디렉토리 존재 여부 확인
    QDir dir(fileBasePath);
    if (!dir.exists()) {
        qDebug() << "경고: 이미지 디렉토리가 존재하지 않습니다. 디렉토리 생성:" << fileBasePath;
        QDir baseDir(QCoreApplication::applicationDirPath());
        baseDir.mkpath("images/tierqrc");
    }

    // 이미지 경로에 대한 대체 리소스 프리픽스 설정
    alternativeResourcePrefix = ":/new/prefix1";
    qDebug() << "대체 리소스 경로:" << alternativeResourcePrefix;

    // 티어 이미지 경로 두 가지 버전(리소스와 파일) 저장
    // Unrated 티어 처리
    tierImages["Unrated"] = {
        resourcePrefix + "/unrated.PNG",         // 리소스 경로
        fileBasePath + "/unrated.PNG"            // 파일 경로
    };

    // Bronze 티어 (1-5)
    for (int i = 1; i <= 5; i++) {
        QString tierName = "Bronze " + QString::number(i);
        tierImages[tierName] = {
            resourcePrefix + "/bronze_" + QString::number(i) + ".PNG",
            fileBasePath + "/bronze_" + QString::number(i) + ".PNG"
        };
    }

    // Silver 티어 (1-5)
    for (int i = 1; i <= 5; i++) {
        QString tierName = "Silver " + QString::number(i);
        tierImages[tierName] = {
            resourcePrefix + "/silver_" + QString::number(i) + ".PNG",
            fileBasePath + "/silver_" + QString::number(i) + ".PNG"
        };
    }

    // Gold 티어 (1-5)
    for (int i = 1; i <= 5; i++) {
        QString tierName = "Gold " + QString::number(i);
        tierImages[tierName] = {
            resourcePrefix + "/gold_" + QString::number(i) + ".PNG",
            fileBasePath + "/gold_" + QString::number(i) + ".PNG"
        };
    }

    // Platinum 티어 (1-5)
    for (int i = 1; i <= 5; i++) {
        QString tierName = "Platinum " + QString::number(i);
        tierImages[tierName] = {
            resourcePrefix + "/platinum_" + QString::number(i) + ".PNG",
            fileBasePath + "/platinum_" + QString::number(i) + ".PNG"
        };
    }

    // Diamond 티어 (1-5)
    for (int i = 1; i <= 5; i++) {
        QString tierName = "Diamond " + QString::number(i);
        tierImages[tierName] = {
            resourcePrefix + "/diamond_" + QString::number(i) + ".PNG",
            fileBasePath + "/diamond_" + QString::number(i) + ".PNG"
        };
    }

    // Ruby 티어 (1-5)
    for (int i = 1; i <= 5; i++) {
        QString tierName = "Ruby " + QString::number(i);
        tierImages[tierName] = {
            resourcePrefix + "/ruby_" + QString::number(i) + ".PNG",
            fileBasePath + "/ruby_" + QString::number(i) + ".PNG"
        };
    }

    // Master 티어
    tierImages["Master"] = {
        resourcePrefix + "/master.PNG",
        fileBasePath + "/master.PNG"
    };
}

// loadTierImage implementation
void UserTierCard::loadTierImage(const QString& tierName)
{
    if (!tierImages.contains(tierName)) {
        qDebug() << "티어 정보 없음:" << tierName;
        return;
    }

    // 로딩 성공 플래그
    bool loaded = false;
    QPixmap pixmap;

    // 방법 1: 리소스 경로에서 로드 시도
    QString resourcePath = tierImages[tierName].resourcePath;
    qDebug() << "리소스 경로 시도:" << resourcePath;
    pixmap = QPixmap(resourcePath);
    if (!pixmap.isNull()) {
        loaded = true;
        qDebug() << "이미지 리소스에서 로드 성공!";
    }

    // 방법 2: 대체 리소스 경로 시도 (new/prefix1)
    if (!loaded) {
        // 티어 이름에서 언더스코어로 변환 (예: Bronze 4 -> bronze_4)
        QString tierKey = tierName.toLower().replace(" ", "_");

        // 첫 번째 시도: 원래 파일명 스타일
        QString altPath = alternativeResourcePrefix + "/" + tierKey + ".PNG";
        qDebug() << "대체 리소스 경로 시도(1):" << altPath;
        pixmap = QPixmap(altPath);

        // 두 번째 시도: 숫자만 있는 파일명 스타일 (예: bronze_4 -> bronze4)
        if (pixmap.isNull()) {
            QString tierKeyNoUnderscore = tierKey;
            tierKeyNoUnderscore.remove("_");
            altPath = alternativeResourcePrefix + "/" + tierKeyNoUnderscore + ".PNG";
            qDebug() << "대체 리소스 경로 시도(2):" << altPath;
            pixmap = QPixmap(altPath);
        }

        if (!pixmap.isNull()) {
            loaded = true;
            qDebug() << "대체 리소스에서 로드 성공!";
        }
    }

    // 방법 3: 파일 경로에서 로드 시도
    if (!loaded) {
        QString filePath = tierImages[tierName].filePath;
        qDebug() << "파일 경로 시도:" << filePath;
        if (QFile::exists(filePath)) {
            pixmap = QPixmap(filePath);
            if (!pixmap.isNull()) {
                loaded = true;
                qDebug() << "파일에서 로드 성공!";
            }
        }
    }

    // 이미지 설정
    if (loaded) {
        // 이미지 비율 유지하면서 확대 (크기를 150x150으로 설정)
        tierImageLabel->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        tierImageLabel->setStyleSheet("");  // 스타일시트 초기화
        tierImageLabel->setText("");  // 텍스트 지우기
    } else {
        // 이미지를 찾을 수 없는 경우 l_o.PNG 이미지 사용
        qDebug() << "이미지를 찾을 수 없습니다: " << tierName;
        QPixmap fallbackPixmap(":/images/l_o.PNG"); // l_o.PNG 이미지 경로

        if (!fallbackPixmap.isNull()) {
            tierImageLabel->setPixmap(fallbackPixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            tierImageLabel->setStyleSheet("");
        } else {
            // l_o.PNG도 없는 경우 텍스트 표시
            tierImageLabel->clear();
            tierImageLabel->setText("No Image");
            tierImageLabel->setStyleSheet("background-color: #cccccc; color: black; font-size: 14pt; text-align: center; border-radius: 10px;");
        }
    }
}
