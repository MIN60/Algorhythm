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

UserChart::UserChart(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QBarSet* set = new QBarSet("푼 문제 수");
    *set << 10 << 20 << 15 << 5 << 1 << 3;

    QBarSeries* series = new QBarSeries();
    series->append(set);

    // 티어
    QStringList categories;
    categories << "Bronze" << "Silver" << "Gold" << "Platinum" << "Diamond" << "Ruby";
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);

    // 값
    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("문제 수");
    axisY->setRange(0, 25);  // 최대 푼 문제 수

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->legend()->setVisible(false);  // 굳이 필요 없을 경우
    chart->setTitle("티어별 푼 문제 수");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(chartView);
    setLayout(layout);
}
