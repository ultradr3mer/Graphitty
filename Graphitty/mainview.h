#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QChart>
#include <QLineSeries>
#include <QMainWindow>
#include <QValueAxis>

#include <Models/functionstablemodel.h>
#include <Models/thresholdstablemodel.h>

#include "FunctionParser/functionnode.h"
#include "Models/mainviewmodel.h"
#include "Models/sheetviewmodel.h"
#include "IO/ioparser.h"

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
  void on_tableWidget_cellActivated(int row, int column);
  void on_viewAdd_clicked();
  void on_viewDelete_clicked();
  void on_viewRename_clicked();
  void on_sheetViews_clicked(const QModelIndex& index);

  void on_actionNeu_triggered();

  void on_actionOpen_triggered();

  private:
  Ui::MainView* ui;
  void initializeChart();
  void safeInitializeChart();
  void addRecentProject();
  void saveCurrentChartData();
  void switchCurrentChartData(int index);
  bool mImported = false;
  FunctionNode mCalculation;
  FunctionNode mOtherCalculation;
  IOParser mIOParser;
  int mFromCalc = 0;
  int mToCalc = 7;
  void setSeries();
  QChart* chart;
  QValueAxis* axisX;
  QValueAxis* axisY;
  MainViewModel model;
  SheetViewModel* sheets;
  void readViewSettings();
  void writeViewSettings();
  FunctionsTableModel* functionstableModel;
  ThresholdsTableModel* thresholdsModel;
};

#endif // MAINVIEW_H
