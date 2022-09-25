/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#include "viewarea.h"

ViewArea::ViewArea() : mFromX(0), mToX(0), mFromY(0), mToY(0)
{
}

ViewArea::ViewArea(double fromX, double toX, double fromY, double toY)
    : mFromX(fromX), mToX(toX), mFromY(fromY), mToY(toY)
{
}

bool ViewArea::isInside(QPointF point)
{
  return (point.x() >= this->getFromX() && point.x() <= this->getToX()) &&
         point.y() >= this->getFromY() && point.y() <= this->getToY();
}

QPointF ViewArea::mapPointToView(QPointF relativePoint)
{
  return QPointF(mFromX + getWidth() * relativePoint.x(), mFromY + getHeight() * relativePoint.y());
}
