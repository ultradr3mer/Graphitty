#ifndef STARTVIEW_H
#define STARTVIEW_H

#include <QDialog>
#include <QMainWindow>

#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui
{
class StartView;
}
QT_END_NAMESPACE

class StartView : public QDialog
{
  Q_OBJECT

public:
  StartView(QWidget* parent = nullptr);
  ~StartView();
  QJsonArray collectRecentProjects();
  void newProject();

  inline QString getFileName()
  {
    return fileName;
  }

private slots:
  void on_newProject_clicked();

  void on_openProject_clicked();

private:
  Ui::StartView* ui;
  QString fileName;
};
#endif // STARTVIEW_H
