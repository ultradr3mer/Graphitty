#ifndef POLYEDITVIEW_H
#define POLYEDITVIEW_H

#include <QDialog>
#include <QLineSeries>
#include <QScatterSeries>

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
  void setPointClicked(bool clicked);
  ~PolyEditView();

public slots:
  void clickPoint(const QPointF& point);

private:
  QPoint mousePos;
  Ui::PolyEditView* ui;
  void initialize();
  qreal distance(const QPointF& p1, const QPointF& p2);
  QScatterSeries* pointsSeries;
  QLineSeries* polySeries;
  QPointF movingPoint;
  QChart* chart;

  // Boolean value to determine if an actual point in the
  // series is clicked.
  bool m_clicked;

  void updatePoly();
};

#endif // POLYEDITVIEW_H
