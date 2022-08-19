#include "startview.h"
#include "mainview.h"
#include "ui_startview.h"
#include <QFileDialog>

StartView::StartView(QWidget* parent) : QMainWindow(parent), ui(new Ui::StartView)
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
  this->hide();
  mainView->show();
}

void StartView::on_openProject_clicked()
{
  auto fileName =
      QFileDialog::getOpenFileName(this, tr("Open Project"), "/home", tr("Json Files (*.json)"));

  auto* mainView = new MainView(this);
  mainView->openProject(fileName);
  this->hide();
  mainView->show();
}
