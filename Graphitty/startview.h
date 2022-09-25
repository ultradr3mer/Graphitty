#ifndef STARTVIEW_H
#define STARTVIEW_H

#include <Models/recentviewmodel.h>
#include <QDialog>
#include <QMainWindow>

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
  void collectRecentProjects();
  void newProject();

  inline QString getFileName()
  {
    return fileName;
  }

private slots:
  void on_newProject_clicked();

  void on_openProject_clicked();

  void on_recentList_clicked(const QModelIndex &index);

  private:
  Ui::StartView* ui;
  QString fileName;
  RecentViewModel* projects;
};
#endif // STARTVIEW_H
