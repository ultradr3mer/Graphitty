// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>

class PolyEditChart;

QT_USE_NAMESPACE

class PolyEditChartView : public QChartView
{
public:
  PolyEditChartView(QWidget* parent = 0);

protected:
  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

private:
  PolyEditChart* m_chart;
  QPoint m_mousePos;
};

#endif
