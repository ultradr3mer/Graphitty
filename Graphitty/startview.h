#ifndef STARTVIEW_H
#define STARTVIEW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class StartView;
}
QT_END_NAMESPACE

class StartView : public QMainWindow
{
  Q_OBJECT

public:
  StartView(QWidget* parent = nullptr);
  ~StartView();
  void newProject();

private slots:
  void on_newProject_clicked();

private:
  Ui::StartView* ui;
};
#endif // STARTVIEW_H
