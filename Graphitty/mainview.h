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
  void initializeChart(double fromX = 0, double toX = 7, double fromY = 0, double toY = 60);
};

#endif // MAINVIEW_H
