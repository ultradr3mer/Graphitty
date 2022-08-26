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
  fromX = 0.0;
  toX = 4.0;
  fromY = 0.0;
  toY = 4.0;

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
  axisX->setRange(fromX, toX);
  series->attachAxis(axisX);
  poly->attachAxis(axisX);

  QValueAxis* axisY = new QValueAxis();
  chart->addAxis(axisY, Qt::AlignLeft);
  axisY->setRange(fromY, toY);
  series->attachAxis(axisY);
  poly->attachAxis(axisY);

  //  QObject::connect(series, &QLineSeries::pressed, this, &PolyEditView::clickPoint);
  this->ui->graphicsView->setRenderHint(QPainter::Antialiasing);
  this->ui->graphicsView->setChart(chart);
  this->ui->graphicsView->setView(this);
  this->ui->tableWidget->setColumnWidth(0, 80);
  this->ui->tableWidget->setColumnWidth(1, 80);

  this->pointsSeries = series;
  this->polySeries = poly;
  this->chart = chart;
  this->axisX = axisX;
  this->axisY = axisY;
}

qreal PolyEditView::distance(const QPointF& p1, const QPointF& p2)
{
  return qSqrt((p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y()));
}

QPointF PolyEditView::pointOnGraph(const QPoint& point)
{
  QPoint mappedPoint = point;
  mappedPoint.setX(point.x() - this->chart->plotArea().x());
  mappedPoint.setY(point.y() - this->chart->plotArea().y());

  double xUnit = this->chart->plotArea().width() / (toX - fromX);
  double yUnit = this->chart->plotArea().height() / (toY - fromY);

  double x = mappedPoint.x() / xUnit;
  double y = toY - mappedPoint.y() / yUnit;

  return QPointF(x, y);
}

QPointF PolyEditView::pointOnWidget(const QPointF& point)
{
  QPointF mappedPoint = point;
  mappedPoint.setX(point.x() - fromX);
  mappedPoint.setY(point.y() - fromY);

  double xUnit = this->chart->plotArea().width() / (toX - fromX);
  double yUnit = this->chart->plotArea().height() / (toY - fromY);

  double x = this->chart->plotArea().x() + mappedPoint.x() * xUnit;
  double y =
      this->chart->plotArea().y() + this->chart->plotArea().height() - mappedPoint.y() * yUnit;

  return QPointF(x, y);
}

void PolyEditView::updatePoly()
{
  double delta = toX - fromX;
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

  this->chart->removeSeries(this->polySeries);
  this->polySeries->clear();
  for (int i = 0; i <= resolution; i++)
  {
    double r = fromX + stepSize * i;
    this->polySeries->append(r, calculatePolynomial(&coefficients, r));
  }

  this->chart->addSeries(this->polySeries);
  this->polySeries->attachAxis(this->axisX);
  this->polySeries->attachAxis(this->axisY);
}

void PolyEditView::setPointClicked(bool clicked)
{
  m_clicked = clicked;

  if (!clicked)
  {
    if (this->movingPoint.x() < fromX || this->movingPoint.x() > toX ||
        this->movingPoint.y() < fromY || this->movingPoint.y() > toY)
    {
      this->pointsSeries->remove(movingPoint);
    }

    this->updatePoly();
  }
}

void PolyEditView::handlePointMove(const QPoint& point)
{
  if (!m_clicked)
  {
    return;
  }

  auto mappedPoint = this->pointOnGraph(point);

  pointsSeries->replace(movingPoint, mappedPoint);

  movingPoint.setX(mappedPoint.x());
  movingPoint.setY(mappedPoint.y());
}

void PolyEditView::handleMousePress(const QPoint& point)
{
  movingPoint = QPointF();
  m_clicked = false;
  const auto points = this->pointsSeries->points();
  for (QPointF p : points)
  {
    auto pointOnWidget = this->pointOnWidget(p);
    auto distanceToPoint = distance(pointOnWidget, point);
    if (distanceToPoint < 10.0f)
    {
      movingPoint = p;
      m_clicked = true;
      return;
    }
  }

  QPointF pointOnGraph = this->pointOnGraph(point);
  this->pointsSeries->append(pointOnGraph);
  movingPoint = point;
  m_clicked = true;
}

void PolyEditView::on_update_clicked()
{
  pointsSeries->clear();
  for (int i = 0; i < this->ui->tableWidget->rowCount(); i++)
  {
    auto x = this->ui->tableWidget->item(i, 0)->text().toFloat();
    auto y = this->ui->tableWidget->item(i, 1)->text().toFloat();
    pointsSeries->append(QPointF(x, y));
  }

  this->updatePoly();
}

void PolyEditView::on_add_clicked()
{
  float maxX = std::numeric_limits<float>().min();
  float maxY = std::numeric_limits<float>().min();
  float minX = std::numeric_limits<float>().max();
  float minY = std::numeric_limits<float>().max();

  const auto points = this->pointsSeries->points();
  for (QPointF p : points)
  {
    maxX = (maxX < p.x()) ? p.x() : maxX;
    maxY = (maxY < p.y()) ? p.y() : maxY;
    minX = (minX > p.x()) ? p.x() : minX;
    minY = (minY > p.y()) ? p.y() : minY;
  }

  QPointF newPoint = QPointF(maxX + (maxX - minX) / 10.0f, maxY + (maxY - minY) / 10.0f);
  this->pointsSeries->append(newPoint);
  this->updatePoly();
}

void PolyEditView::on_remove_clicked()
{
  QList<int> rowList{};
  for (QTableWidgetItem* item : this->ui->tableWidget->selectedItems())
  {
    if (rowList.contains(item->row()))
    {
      continue;
    }

    rowList.insert(0, item->row());
  }

  for (int row : rowList)
  {
    this->pointsSeries->remove(row - 1);
  }

  this->updatePoly();
}
