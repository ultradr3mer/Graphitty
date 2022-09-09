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
