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

    // 그래프 초기화 및 그리기
    chart->removeAllSeries();
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());

    QBarSet* set = new QBarSet("푼 문제 수");
    for (int count : tierCounts)
        *set << count;


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
