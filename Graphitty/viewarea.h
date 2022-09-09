#ifndef VIEWAREA_H
#define VIEWAREA_H

#include <QPointF>

class ViewArea
{
public:
  ViewArea();
  ViewArea(double fromX, double toX, double fromY, double toY);

  inline double getFromX()
  {
    return mFromX;
  }

  inline double getToX()
  {
    return mToX;
  }

  inline double getFromY()
  {
    return mFromY;
  }

  inline double getToY()
  {
    return mToY;
  }

  inline double getWidth()
  {
    return mToX - mFromX;
  }

  inline double getHeight()
  {
    return mToY - mFromY;
  }

  bool isInside(QPointF point);
  QPointF mapPointToView(QPointF relativePoint);

private:
  double mFromX;
  double mToX;
  double mFromY;
  double mToY;
};

#endif // VIEWAREA_H
