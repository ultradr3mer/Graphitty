/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef VIEWAREA_H
#define VIEWAREA_H

#include <QJsonArray>
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

  inline QJsonArray getViewpoints()
  {
    return {mFromX, mToX, mFromY, mToY};
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
