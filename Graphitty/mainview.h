#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>

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

private:
  Ui::MainView* ui;
  void initializeChart();
  bool mImported = false;
  int mFromX = 0;
  int mToX = 7;
  int mFromY = 0;
  int mToY = 60;
  QString mCalculation;
  int mFromCalc = 0;
  int mToCalc = 7;
};

#endif // MAINVIEW_H
