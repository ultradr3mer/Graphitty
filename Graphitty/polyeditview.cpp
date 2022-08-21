#include "polyeditview.h"
#include "ui_polyeditview.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QValueAxis>
#include <QtCore/QtMath>
#include <qlineseries.h>
#include <qvalueaxis.h>

std::vector<double> solveLes(std::vector<std::vector<double>> les)
{
  unsigned int degree = les.size();
  for (unsigned int i = 0; i < degree; i++)
  {
    if (les.at(i).at(i) == 0.0)
    {
      for (unsigned int u = i + 1; u < degree; u++)
      {
        if (les.at(u).at(i) == 0.0)
        {
          continue;
        }

        std::swap(les.at(i), les.at(u));
        break;
      }
    }

    double valueAtBegin = les.at(i).at(i);
    auto line = &les.at(i);
    if (valueAtBegin != 1.0)
    {
      for (unsigned int v = i; v < line->size(); v++)
      {
        line->at(v) /= valueAtBegin;
      }
    }

    for (unsigned int u = 0; u < degree; u++)
    {
      if (u == i)
      {
        continue;
      }

      auto lineOther = &les.at(u);
      double valueAtBeginOther = lineOther->at(i);
      if (valueAtBeginOther == 0.0)
      {
        continue;
      }

      for (unsigned int v = i; v < line->size(); v++)
      {
        lineOther->at(v) -= line->at(v) * valueAtBeginOther;
      }
    }
  }

  std::vector<double> result;
  for (auto line : les)
  {
    result.push_back(line.back());
  }

  return result;
}

std::vector<double> findPolynomial(std::vector<std::array<double, 2>> points)
{
  int degree = points.size();
  std::vector<std::vector<double>> les;
  for (auto point : points)
  {
    std::vector<double> line;

    for (int i = degree - 1; i >= 0; i--)
    {
      line.push_back(pow(point[0], i));
    }

    line.push_back(point[1]);

    les.push_back(std::move(line));
  }

  return solveLes(std::move(les));
}

double calculatePolynomial(std::vector<double>* coefficients, double x)
{
  int degree = coefficients->size();
  double value = 0;
  for (int i = 0; i < degree; i++)
  {
    value += coefficients->at(i) * pow(x, degree - 1 - i);
  }
  return value;
}

PolyEditView::PolyEditView(QWidget* parent) : QDialog(parent), ui(new Ui::PolyEditView)
{
  ui->setupUi(this);

  m_clicked = false;

  this->initialize();
  this->updatePoly();
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

  QChart* chart = new QChart();
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

  QObject::connect(series, &QLineSeries::pressed, this, &PolyEditView::clickPoint);
  this->ui->graphicsView->setRenderHint(QPainter::Antialiasing);
  this->ui->graphicsView->setChart(chart);
  this->ui->graphicsView->setView(this);
  this->ui->tableWidget->setColumnWidth(0, 80);
  this->ui->tableWidget->setColumnWidth(1, 80);

  this->pointsSeries = series;
  this->polySeries = poly;
  this->chart = chart;
}

void PolyEditView::clickPoint(const QPointF& point)
{
  // Find the closest data point
  movingPoint = QPoint();
  m_clicked = false;
  const auto points = this->pointsSeries->points();
  for (QPointF p : points)
  {
    if (distance(p, point) < distance(movingPoint, point))
    {
      movingPoint = p;
      m_clicked = true;
    }
  }
}

qreal PolyEditView::distance(const QPointF& p1, const QPointF& p2)
{
  return qSqrt((p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y()));
}

void PolyEditView::updatePoly()
{
  double from = 0.0;
  double to = 4.0;
  double delta = to - from;
  int resolution = 1000;
  double stepSize = (double)delta / (double)resolution;

  this->ui->tableWidget->setRowCount(pointsSeries->count());
  int row = 0;
  std::vector<std::array<double, 2>> pointsVector;
  foreach (auto point, pointsSeries->points())
  {
    pointsVector.push_back({point.x(), point.y()});

    auto stringX = QString::number(point.x());
    this->ui->tableWidget->setItem(row, 0, new QTableWidgetItem(stringX));
    auto stringY = QString::number(point.x());
    this->ui->tableWidget->setItem(row, 1, new QTableWidgetItem(stringY));
    row++;
  }

  auto coefficients = findPolynomial(std::move(pointsVector));

  this->polySeries->clear();
  for (int i = 0; i <= resolution; i++)
  {
    double r = from + stepSize * i;
    this->polySeries->append(r, calculatePolynomial(&coefficients, r));
  }
}

void PolyEditView::setPointClicked(bool clicked)
{
  m_clicked = clicked;

  if (!clicked)
  {
    this->updatePoly();
  }
}

void PolyEditView::handlePointMove(const QPoint& point)
{
  if (!m_clicked)
  {
    return;
  }
  // Map the point clicked from the ChartView
  // to the area occupied by the chart.
  QPoint mappedPoint = point;
  mappedPoint.setX(point.x() - this->chart->plotArea().x());
  mappedPoint.setY(point.y() - this->chart->plotArea().y());

  // Get the x- and y axis to be able to convert the mapped
  // coordinate point to the charts scale.
  QAbstractAxis* axisx = this->chart->axes(Qt::Horizontal).first();
  QValueAxis* haxis = 0;
  if (axisx->type() == QAbstractAxis::AxisTypeValue)
    haxis = qobject_cast<QValueAxis*>(axisx);

  QAbstractAxis* axisy = this->chart->axes(Qt::Vertical).first();
  QValueAxis* vaxis = 0;
  if (axisy->type() == QAbstractAxis::AxisTypeValue)
    vaxis = qobject_cast<QValueAxis*>(axisy);

  if (!(haxis && vaxis))
  {
    return;
  }

  // Calculate the "unit" between points on the x
  // y axis.
  double xUnit = this->chart->plotArea().width() / haxis->max();
  double yUnit = this->chart->plotArea().height() / vaxis->max();

  // Convert the mappedPoint to the actual chart scale.
  double x = mappedPoint.x() / xUnit;
  double y = vaxis->max() - mappedPoint.y() / yUnit;

  // Replace the old point with the new one.
  pointsSeries->replace(movingPoint, QPointF(x, y));

  // Update the m_movingPoint so we are able to
  // do the replace also during mousemoveEvent.
  movingPoint.setX(x);
  movingPoint.setY(y);
}
