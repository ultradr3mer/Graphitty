#ifndef POLYEDITVIEW_H
#define POLYEDITVIEW_H

#include <QDialog>

namespace Ui
{
class PolyEditView;
}

class PolyEditView : public QDialog
{
  Q_OBJECT

public:
  explicit PolyEditView(QWidget* parent = nullptr);
  ~PolyEditView();

private:
  Ui::PolyEditView* ui;
  void initialize();
};

#endif // POLYEDITVIEW_H
