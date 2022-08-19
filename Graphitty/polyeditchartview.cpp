// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "polyeditchartview.h"
#include "polyeditchart.h"
#include <QtGui/QMouseEvent>

PolyEditChartView::PolyEditChartView(QWidget* parent) : QChartView(parent)
{
}

void PolyEditChartView::mousePressEvent(QMouseEvent* event)
{
  m_mousePos = event->pos();
  QChartView::mousePressEvent(event);
}

void PolyEditChartView::mouseMoveEvent(QMouseEvent* event)
{
  m_chart->handlePointMove(event->pos());
  QChartView::mouseMoveEvent(event);
}

void PolyEditChartView::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->pos() != m_mousePos)
  {
    m_chart->handlePointMove(event->pos());
    m_chart->setPointClicked(false);
  }
  QChartView::mouseReleaseEvent(event);
}
