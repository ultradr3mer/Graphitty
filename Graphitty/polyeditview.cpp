#include "polyeditview.h"
#include "polyeditchart.h"
#include "ui_polyeditview.h"

#include <qlineseries.h>
#include <qvalueaxis.h>

PolyEditView::PolyEditView(QWidget* parent) : QDialog(parent), ui(new Ui::PolyEditView)
{
  ui->setupUi(this);

  this->initialize();
}

PolyEditView::~PolyEditView()
{
  delete ui;
}

void PolyEditView::initialize()
{
  QScatterSeries* series = new QScatterSeries();

  std::vector<std::array<double, 2>> points = {
      {0.0, 0.0}, {1.0, 1.0}, {2.0, 3.0}, {3.0, 3.0}, {4.0, 3.0}};
  for (auto point : points)
  {
    series->append(point[0], point[1]);
  }

  QLineSeries* poly = new QLineSeries();

  PolyEditChart* chart = new PolyEditChart(0, {}, series, poly);
  chart->legend()->hide();
  chart->addSeries(poly);
  chart->addSeries(series);
  QPen p = series->pen();
  p.setWidth(5);
  series->setPen(p);
  chart->setTitle("Drag'n drop to move data points");

  QValueAxis* axisX = new QValueAxis();
  chart->addAxis(axisX, Qt::AlignBottom);
  axisX->setRange(0, 4);
  series->attachAxis(axisX);
  poly->attachAxis(axisX);

  QValueAxis* axisY = new QValueAxis();
  chart->addAxis(axisY, Qt::AlignLeft);
  axisY->setRange(0, 4);
  series->attachAxis(axisY);
  poly->attachAxis(axisY);

  QObject::connect(series, &QLineSeries::pressed, chart, &PolyEditChart::clickPoint);

  this->ui->graphicsView->setChart(chart);
}
