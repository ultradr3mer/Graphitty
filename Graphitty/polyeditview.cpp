/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#include "polyeditview.h"
#include "Math/polynomialfinder.h"
#include "ui_polyeditview.h"
#include <QPointF>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QValueAxis>
#include <QtCore/QtMath>
#include <qlineseries.h>
#include <qvalueaxis.h>

PolyEditView::PolyEditView(QWidget* parent) : QDialog(parent), ui(new Ui::PolyEditView)
{
  ui->setupUi(this);

  m_clicked = false;
}

PolyEditView::~PolyEditView()
{
  delete ui;
}

void PolyEditView::initializeChart()
{
  QScatterSeries* series = new QScatterSeries();

  QList<QPointF> points = {this->viewArea.mapPointToView(QPointF(0.25f, 0.25f)),
                           this->viewArea.mapPointToView(QPointF(0.75f, 0.75f))};
  for (auto point : points)
  {
    series->append(point.x(), point.y());
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
  axisX->setRange(this->viewArea.getFromX(), this->viewArea.getToX());
  series->attachAxis(axisX);
  poly->attachAxis(axisX);

  QValueAxis* axisY = new QValueAxis();
  chart->addAxis(axisY, Qt::AlignLeft);
  axisY->setRange(this->viewArea.getFromY(), this->viewArea.getToY());
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

  double xUnit = this->chart->plotArea().width() / this->viewArea.getWidth();
  double yUnit = this->chart->plotArea().height() / this->viewArea.getHeight();

  double x = mappedPoint.x() / xUnit;
  double y = this->viewArea.getToY() - mappedPoint.y() / yUnit;

  return QPointF(x, y);
}

QPointF PolyEditView::pointOnWidget(const QPointF& point)
{
  QPointF mappedPoint = point;
  mappedPoint.setX(point.x() - this->viewArea.getFromX());
  mappedPoint.setY(point.y() - this->viewArea.getFromY());

  double xUnit = this->chart->plotArea().width() / this->viewArea.getWidth();
  double yUnit = this->chart->plotArea().height() / this->viewArea.getHeight();

  double x = this->chart->plotArea().x() + mappedPoint.x() * xUnit;
  double y =
      this->chart->plotArea().y() + this->chart->plotArea().height() - mappedPoint.y() * yUnit;

  return QPointF(x, y);
}

void PolyEditView::updatePoly()
{
  double delta = this->viewArea.getWidth();
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
    auto stringY = QString::number(point.y());
    this->ui->tableWidget->setItem(row, 1, new QTableWidgetItem(stringY));
    row++;
  }

  auto coefficients = findPolynomial(std::move(pointsVector));

  this->chart->removeSeries(this->polySeries);
  this->polySeries->clear();
  for (int i = 0; i <= resolution; i++)
  {
    double r = this->viewArea.getFromX() + stepSize * i;
    this->polySeries->append(r, calculatePolynomial(&coefficients, r));
  }

  this->chart->addSeries(this->polySeries);
  this->polySeries->attachAxis(this->axisX);
  this->polySeries->attachAxis(this->axisY);

  this->formula = generateFormula(&coefficients);
  this->ui->formulaField->setText(this->formula);
}

void PolyEditView::setPointClicked(bool clicked)
{
  m_clicked = clicked;

  if (!clicked)
  {
    if (!this->viewArea.isInside(this->movingPoint))
    {
      this->pointsSeries->remove(movingPoint);
    }

    this->updatePoly();
  }
}

void PolyEditView::initialize(ViewArea& viewArea)
{
  this->viewArea = viewArea;
  this->ui->fromX->setValue(this->viewArea.getFromX());
  this->ui->toX->setValue(this->viewArea.getToX());
  this->ui->fromY->setValue(this->viewArea.getFromY());
  this->ui->toY->setValue(this->viewArea.getToY());

  this->initializeChart();
  this->updatePoly();
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
  movingPoint = pointOnGraph;
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

void PolyEditView::on_buttonBox_accepted()
{
}
