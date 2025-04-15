#ifndef USER_CHART_H
#define USER_CHART_H

#include <QWidget>
#include <QJsonObject>
#include <QtCharts/QChartView>
#include "networkmanager.h"

class QChartView;
class QBarSet;
class QChart;

class UserChart : public QWidget
{
    Q_OBJECT

public:
    explicit UserChart(QWidget* parent = nullptr);
    void setUsername(const QString& username);
    void handleUserChartData(const QJsonObject& userData);
    void clearChart();

public slots:
    void drawChart(const QJsonObject& graphData);

private:
    QChartView* chartView;
    QChart* chart;
    NetworkManager* networkManager = nullptr;
};

#endif // USER_CHART_H
