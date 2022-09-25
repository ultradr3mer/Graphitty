/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#include "polyeditchartview.h"
#include "polyeditview.h"
#include <QtGui/QMouseEvent>

PolyEditChartView::PolyEditChartView(QWidget* parent) : QChartView(parent)
{
}

void PolyEditChartView::setView(PolyEditView* view)
{
  this->view = view;
}

void PolyEditChartView::mousePressEvent(QMouseEvent* event)
{
  mousePos = event->pos();
  view->handleMousePress(mousePos);
  QChartView::mousePressEvent(event);
}

void PolyEditChartView::mouseMoveEvent(QMouseEvent* event)
{
  view->handlePointMove(event->pos());
  QChartView::mouseMoveEvent(event);
}

void PolyEditChartView::mouseReleaseEvent(QMouseEvent* event)
{
  view->handlePointMove(event->pos());
  view->setPointClicked(false);

  QChartView::mouseReleaseEvent(event);
}
