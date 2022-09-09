#ifndef POLYEDITVIEW_H
#define POLYEDITVIEW_H

#include <QDialog>
#include <QLineSeries>
#include <QScatterSeries>
#include <QValueAxis>

#include "viewarea.h"

namespace Ui
{
class PolyEditView;
}

class PolyEditView : public QDialog
{
  Q_OBJECT

public:
  explicit PolyEditView(QWidget* parent = nullptr);
  void handlePointMove(const QPoint& point);
  void handleMousePress(const QPoint& point);
  void setPointClicked(bool clicked);
  void initialize(const ViewArea& viewArea);
  ~PolyEditView();

  inline QString getFormula()
  {
    return formula;
  }

public slots:
  //  void clickPoint(const QPointF& point);

private slots:
  void on_add_clicked();
  void on_remove_clicked();
  void on_update_clicked();

  void on_buttonBox_accepted();

private:
  QPoint mousePos;
  Ui::PolyEditView* ui;
  void initializeChart();
  qreal distance(const QPointF& p1, const QPointF& p2);
  QScatterSeries* pointsSeries;
  QLineSeries* polySeries;
  QPointF movingPoint;
  QChart* chart;
  QValueAxis* axisX;
  QValueAxis* axisY;
  QPointF pointOnGraph(const QPoint& point);
  QPointF pointOnWidget(const QPointF& point);
  QString formula;

  ViewArea viewArea;

  // Boolean value to determine if an actual point in the
  // series is clicked.
  bool m_clicked;

  void updatePoly();
};

#endif // POLYEDITVIEW_H
