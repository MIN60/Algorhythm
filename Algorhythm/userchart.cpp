#include "userchart.h"
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>
#include <QVBoxLayout>
#include <QPainter>
#include <QToolTip>

#include <QMouseEvent>
#include "networkmanager.h"
#include <QNetworkAccessManager>

UserChart::UserChart(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    chart = new QChart();
    chart->setTitle("Tier");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(false);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(chartView);
    setLayout(layout);
}


void UserChart::drawChart(const QJsonObject& graphData)
{
    if (!graphData.contains("level") || !graphData["level"].isObject())
        return;

    chart->setTitle("티어별 푼 문제 수");

    QJsonObject levelObj = graphData["level"].toObject();

    QVector<int> tierCounts(6, 0);  // Bronze ~ Ruby

    for (const QString& key : levelObj.keys()) {
        int level = key.toInt();
        int count = levelObj.value(key).toInt();

        if (level >= 1 && level <= 5)
            tierCounts[0] += count;
        else if (level <= 10)
            tierCounts[1] += count;
        else if (level <= 15)
            tierCounts[2] += count;
        else if (level <= 20)
            tierCounts[3] += count;
        else if (level <= 25)
            tierCounts[4] += count;
        else if (level <= 30)
            tierCounts[5] += count;
    }

    chart->removeAllSeries();

    const QList<QAbstractAxis*>& horizontalAxes = chart->axes(Qt::Horizontal);
    for (QAbstractAxis* axis : horizontalAxes) {
        chart->removeAxis(axis);
    }

    const QList<QAbstractAxis*>& verticalAxes = chart->axes(Qt::Vertical);
    for (QAbstractAxis* axis : verticalAxes) {
        chart->removeAxis(axis);
    }


    QBarSet* set = new QBarSet("푼 문제 수");
    for (const int& count : tierCounts) {
        *set << count;
    }

    set->setColor(QColor("#4AC26B"));
    set->setBrush(QColor("#4AC26B"));



    QBarSeries* series = new QBarSeries();
    series->append(set);

    QStringList categories = {"Bronze", "Silver", "Gold", "Platinum", "Diamond", "Ruby"};
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);

    connect(set, &QBarSet::hovered, this, [=](bool status, int index) {
        if (status) {
            QString text = QString("%1: %2문제").arg(categories[index]).arg(tierCounts[index]);
            QToolTip::showText(QCursor::pos(), text, this);
        } else {
            QToolTip::hideText();
        }
    });


    QValueAxis* axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    axisY->setRange(0, *std::max_element(tierCounts.begin(), tierCounts.end()) + 5);

    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
}

void UserChart::setUsername(const QString& username)
{
    if (username.isEmpty()) return;

    if (!networkManager) {
        networkManager = new NetworkManager(this);
        connect(networkManager, &NetworkManager::onGraphDataReceived, this, &UserChart::drawChart);
    }

    networkManager->graphData(username);
}


void UserChart::handleUserChartData(const QJsonObject& userData)
{
    if (userData.contains("level")) {
        drawChart(userData);
    }
}


void UserChart::clearChart()
{
    chart->removeAllSeries();

    const QList<QAbstractAxis*>& horizontalAxes = chart->axes(Qt::Horizontal);
    for (QAbstractAxis* axis : horizontalAxes)
        chart->removeAxis(axis);

    const QList<QAbstractAxis*>& verticalAxes = chart->axes(Qt::Vertical);
    for (QAbstractAxis* axis : verticalAxes)
        chart->removeAxis(axis);

    chart->setTitle("푼 문제 수 차트 생성을 위해 로그인이 필요합니다");

    QBarSet* emptySet = new QBarSet("푼 문제 수");
    for (int i = 0; i < 6; ++i){
        *emptySet << 0;
    }

    QBarSeries* emptySeries = new QBarSeries();
    emptySeries->append(emptySet);

    QStringList categories = {"Bronze", "Silver", "Gold", "Platinum", "Diamond", "Ruby"};
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);

    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 1);

    chart->addSeries(emptySeries);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    emptySeries->attachAxis(axisX);
    emptySeries->attachAxis(axisY);
}
