#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QChart>
#include <QLineSeries>
#include <QMainWindow>
#include <QValueAxis>

#include "FunctionParser/functionnode.h"
#include "Models/mainviewmodel.h"
#include "sheetmanager.h"

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

private:
  Ui::MainView* ui;
  void initializeChart();
  void addRecentProject();
  bool mImported = false;
  FunctionNode mCalculation;
  FunctionNode mOtherCalculation;
  SheetManager mSheetManager;
  int mFromCalc = 0;
  int mToCalc = 7;
  void setSeries();
  QChart* chart;
  QValueAxis* axisX;
  QValueAxis* axisY;
  MainViewModel model;
  void readViewSettings();
  void writeViewSettings();
};

#endif // MAINVIEW_H
