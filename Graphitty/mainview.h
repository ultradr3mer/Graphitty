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

private slots:
  void on_polyEdit_clicked();

private:
  Ui::MainView* ui;
  void initializeChart();
};

#endif // MAINVIEW_H
