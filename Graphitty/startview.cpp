#include "startview.h"
#include "mainview.h"
#include "ui_startview.h"
#include <QFileDialog>

StartView::StartView(QWidget* parent) : QDialog(parent), ui(new Ui::StartView)
{
  ui->setupUi(this);
}

StartView::~StartView()
{
  delete ui;
}

void StartView::on_newProject_clicked()
{
  auto* mainView = new MainView(this);
  this->close();
  //  mainView->show();
}

void StartView::on_openProject_clicked()
{
  this->fileName =
      QFileDialog::getOpenFileName(this, tr("Open Project"), "/home", tr("Json Files (*.json)"));

  this->close();
  //  MainView mainView;
  //  mainView.openProject(fileName);
  //  this->hide();
  //  mainView.show();
}
