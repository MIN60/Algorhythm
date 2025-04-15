#ifndef USERTIERCARD_H
#define USERTIERCARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include <QCoreApplication>
#include <QDir>
#include <QPair>

class UserTierCard : public QWidget {
    Q_OBJECT
public:
    explicit UserTierCard(QWidget* parent = nullptr);
    void setTier(const QString& tierName);
    void setTierImage(const QString& imagePath);

private:
    void initializeTierImages();
    void loadTierImage(const QString& tierName);

    QLabel* tierLabel;
    QFrame* cardFrame;
    QLabel* tierImageLabel;

    // 티어 이미지 경로 저장 구조체
    struct TierImagePaths {
        QString resourcePath;  // 리소스 시스템 경로
        QString filePath;      // 파일 시스템 경로
    };

    QMap<QString, TierImagePaths> tierImages;
    QString alternativeResourcePrefix;  // 대체 리소스 경로
};

#endif // USERTIERCARD_H
