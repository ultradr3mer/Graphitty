#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QChart>
#include <QLineSeries>
#include <QMainWindow>
#include <QValueAxis>

#include "FunctionParser/functionnode.h"

namespace Ui
{
class MainView;
}

class MainView : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainView(QWidget* parent = nullptr);
  ~MainView();
  void openProject(const QString& fileName);

private slots:
  void on_polyEdit_clicked();
  void on_actionSpeichern_unter_triggered();
  void on_actionSpeichern_triggered();
  void on_actionProjektmappe_schlie_en_triggered();
  void on_update_clicked();

private:
  Ui::MainView* ui;
  void initializeChart();
  bool mImported = false;
  int mFromX = 0;
  int mToX = 7;
  int mFromY = 0;
  int mToY = 60;
  FunctionNode mCalculation;
  int mFromCalc = 0;
  int mToCalc = 7;
  QLineSeries* addFunctionToChart(FunctionNode* func, const QString& name);
  QLineSeries* addDerivationToChart(QLineSeries* series, const QString& name);
  QChart* chart;
  QValueAxis* axisX;
  QValueAxis* axisY;
};

#endif // MAINVIEW_H
