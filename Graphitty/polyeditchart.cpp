// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "polyeditchart.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QValueAxis>
#include <QtCore/QtMath>

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

PolyEditChart::PolyEditChart(QGraphicsItem* parent, Qt::WindowFlags wFlags, QScatterSeries* points,
                             QLineSeries* poly)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags), m_points(points), m_poly(poly)
{
  m_clicked = false;

  double from = 0.0;
  double to = 4.0;
  double delta = to - from;
  int resolution = 1000;
  double stepSize = (double)delta / (double)resolution;

  std::vector<std::array<double, 2>> pointsVector;
  foreach (auto point, points->points())
  {
    pointsVector.push_back({point.x(), point.y()});
  }

  auto coefficients = findPolynomial(std::move(pointsVector));

  for (int i = 0; i <= resolution; i++)
  {
    double r = from + stepSize * i;
    poly->append(r, calculatePolynomial(&coefficients, r));
  }
}

PolyEditChart::~PolyEditChart()
{
}

void PolyEditChart::clickPoint(const QPointF& point)
{
  // Find the closest data point
  m_movingPoint = QPoint();
  m_clicked = false;
  const auto points = m_points->points();
  for (QPointF p : points)
  {
    if (distance(p, point) < distance(m_movingPoint, point))
    {
      m_movingPoint = p;
      m_clicked = true;
    }
  }
}

qreal PolyEditChart::distance(const QPointF& p1, const QPointF& p2)
{
  return qSqrt((p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y()));
}

void PolyEditChart::setPointClicked(bool clicked)
{
  m_clicked = clicked;
}

void PolyEditChart::handlePointMove(const QPoint& point)
{
  if (m_clicked)
  {
    // Map the point clicked from the ChartView
    // to the area occupied by the chart.
    QPoint mappedPoint = point;
    mappedPoint.setX(point.x() - this->plotArea().x());
    mappedPoint.setY(point.y() - this->plotArea().y());

    // Get the x- and y axis to be able to convert the mapped
    // coordinate point to the charts scale.
    QAbstractAxis* axisx = axes(Qt::Horizontal).first();
    QValueAxis* haxis = 0;
    if (axisx->type() == QAbstractAxis::AxisTypeValue)
      haxis = qobject_cast<QValueAxis*>(axisx);

    QAbstractAxis* axisy = axes(Qt::Vertical).first();
    QValueAxis* vaxis = 0;
    if (axisy->type() == QAbstractAxis::AxisTypeValue)
      vaxis = qobject_cast<QValueAxis*>(axisy);

    if (haxis && vaxis)
    {
      // Calculate the "unit" between points on the x
      // y axis.
      double xUnit = this->plotArea().width() / haxis->max();
      double yUnit = this->plotArea().height() / vaxis->max();

      // Convert the mappedPoint to the actual chart scale.
      double x = mappedPoint.x() / xUnit;
      double y = vaxis->max() - mappedPoint.y() / yUnit;

      // Replace the old point with the new one.
      m_points->replace(m_movingPoint, QPointF(x, y));

      // Update the m_movingPoint so we are able to
      // do the replace also during mousemoveEvent.
      m_movingPoint.setX(x);
      m_movingPoint.setY(y);
    }
  }
}
