// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef CHART_H
#define CHART_H

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

#include <QScatterSeries>

QT_USE_NAMESPACE

class PolyEditChart : public QChart
{
  Q_OBJECT
public:
  explicit PolyEditChart(QGraphicsItem* parent = nullptr, Qt::WindowFlags wFlags = {},
                 QScatterSeries* points = nullptr, QLineSeries* poly = nullptr);
  ~PolyEditChart();

public slots:
  void clickPoint(const QPointF& point);

public:
  void handlePointMove(const QPoint& point);
  void setPointClicked(bool clicked);

private:
  qreal distance(const QPointF& p1, const QPointF& p2);
  QScatterSeries* m_points;
  QLineSeries* m_poly;
  QPointF m_movingPoint;

  // Boolean value to determine if an actual point in the
  // series is clicked.
  bool m_clicked;
};

#endif // CHART_H
