#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include "../services/StudentManager.h"

// Fenêtre statistiques : données globales + histogramme des GPA
class StatsDialog : public QDialog {
    Q_OBJECT
public:
    StatsDialog(const StudentManager& mgr, QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("Statistiques");
        setMinimumSize(600, 450);

        QVBoxLayout* layout = new QVBoxLayout(this);

        // Stats textuelles
        layout->addWidget(new QLabel(QString("<b>Total étudiants :</b> %1").arg(mgr.count())));
        layout->addWidget(new QLabel(QString("<b>Moyenne GPA :</b> %1")
            .arg(QString::number(mgr.averageGpa(), 'f', 2))));
        layout->addWidget(new QLabel(QString("<b>Licence :</b> %1   <b>Master :</b> %2   <b>Doctorat :</b> %3")
            .arg(mgr.countByType("Licence"))
            .arg(mgr.countByType("Master"))
            .arg(mgr.countByType("Doctorat"))));

        // Histogramme GPA par tranches
        QBarSet* barSet = new QBarSet("Étudiants");
        QStringList categories;
        int ranges[5] = {0};
        for (const auto& [id, s] : mgr.getAll()) {
            float g = s->getGpa();
            if      (g < 1.0f) ranges[0]++;
            else if (g < 2.0f) ranges[1]++;
            else if (g < 3.0f) ranges[2]++;
            else if (g < 3.5f) ranges[3]++;
            else               ranges[4]++;
        }
        *barSet << ranges[0] << ranges[1] << ranges[2] << ranges[3] << ranges[4];
        categories << "0-1" << "1-2" << "2-3" << "3-3.5" << "3.5-4";

        QBarSeries* series = new QBarSeries;
        series->append(barSet);

        QChart* chart = new QChart;
        chart->addSeries(series);
        chart->setTitle("Répartition des GPA");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        QBarCategoryAxis* axisX = new QBarCategoryAxis;
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis* axisY = new QValueAxis;
        axisY->setTitleText("Nombre");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        QChartView* chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        layout->addWidget(chartView);

        auto* btns = new QDialogButtonBox(QDialogButtonBox::Close);
        connect(btns, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addWidget(btns);
    }
};

#endif // STATSDIALOG_H
