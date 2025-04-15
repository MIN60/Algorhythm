#ifndef USER_CHART_H
#define USER_CHART_H

#include <QWidget>
#include <QJsonObject>
#include <QtCharts/QChartView>

class QChartView;
class QBarSet;
class QChart;

class UserChart : public QWidget
{
    Q_OBJECT

public:
    explicit UserChart(QWidget* parent = nullptr);

public slots:
    void drawChart(const QJsonObject& graphData);

private:
    QChartView* chartView;
    QChart* chart;
};

#endif // USER_CHART_H
