#include "mainview.h"
#include "ui_mainview.h"

#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPointF>
#include <QRegExp>
#include <QTableView>
#include <fstream>
#include <qlineseries.h>
#include <qvalueaxis.h>

#include <Data/functiondata.h>
#include <Exceptions/derivationexception.h>
#include <Models/functionstablemodel.h>
#include <Models/thresholdstablemodel.h>
#include <sheetmanager.h>

MainView::MainView(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainView), model(MainViewModel())
{
  ui->setupUi(this);

  this->model = MainViewModel();
  this->sheets = new SheetViewModel();
  this->sheets->setSheetViews(this->model);
  this->mSheetManager = SheetManager();

  FunctionsTableModel* functionstableModel = new FunctionsTableModel();
  functionstableModel->setFunctionData(this->model.getChartData()->getFunctionData());
  this->ui->functions->setModel(functionstableModel);

  auto thresholdsModel = new ThresholdsTableModel();
  thresholdsModel->setThresholdData(this->model.getChartData()->getThresholdData());
  this->ui->thesholds->setModel(thresholdsModel);

  this->ui->sheetViews->setModel(this->sheets);

  this->readViewSettings();
  this->initializeChart();
}

MainView::~MainView()
{
  delete ui;
}

void MainView::openProject(const QString& fileName)
{
  QList<ChartData> loadedData = this->mSheetManager.readProject(fileName);
  this->model.setChartList(loadedData);
  this->model.setActiveChart(0);
  this->sheets->setSheetViews(this->model);
  this->readViewSettings();
  this->initializeChart();
    /*
  this->mSheetManager.openProject(fileName);
  ChartData loadedData = this->mSheetManager.loadSheet();
  QJsonArray tree = this->mSheetManager.getSheetTree(); // Debug
  this->addRecentProject();
  this->model.setChartData(loadedData);
  this->readViewSettings();
  this->initializeChart();
*/
}

void MainView::initializeChart()
{
  QChart* chart = new QChart();

  auto viewArea = this->model.getChartData()->getViewArea();

  QValueAxis* axisX = new QValueAxis();
  axisX->setRange(viewArea->getFromX(), viewArea->getToX());
  chart->addAxis(axisX, Qt::AlignBottom);
  this->axisX = axisX;

  QValueAxis* axisY = new QValueAxis();
  axisY->setRange(viewArea->getFromY(), viewArea->getToY());
  chart->addAxis(axisY, Qt::AlignLeft);
  this->axisY = axisY;

  this->ui->chartView->setChart(chart);
  this->chart = chart;

  this->setSeries();
}

void MainView::readViewSettings()
{
  auto viewArea = this->model.getChartData()->getViewArea();
  this->ui->fromX->setValue(viewArea->getFromX());
  this->ui->toX->setValue(viewArea->getToX());
  this->ui->fromY->setValue(viewArea->getFromY());
  this->ui->toY->setValue(viewArea->getToY());
  this->ui->invert->setChecked(this->model.getChartData()->getIsChartInverted());
  this->model.getChartData()->setViewArea(*viewArea);
}

void MainView::writeViewSettings()
{
  auto viewArea = ViewArea(this->ui->fromX->value(), this->ui->toX->value(),
                           this->ui->fromY->value(), this->ui->toY->value());
  this->model.getChartData()->setViewArea(viewArea);
  this->model.getChartData()->setIsChartInverted(this->ui->invert->isChecked());
}

void MainView::setSeries()
{
  this->writeViewSettings();
  auto viewArea = this->model.getChartData()->getViewArea();
  this->axisX->setRange(viewArea->getFromX(), viewArea->getToX());
  this->axisY->setRange(viewArea->getFromY(), viewArea->getToY());

  this->chart->removeAllSeries();

  auto allSeries = this->model.generateAllSeries();

  for (auto singleSeries : *allSeries)
  {
    this->chart->addSeries(singleSeries);
    singleSeries->attachAxis(this->axisX);
    singleSeries->attachAxis(this->axisY);
  }
}

void MainView::addRecentProject()
{
  QString val;
  QFile file;
  // QString fileName = QFileDialog::getOpenFileName(this, tr("Open Recent"), "/home", tr("Json
  // Files (*.json)"));
  QString fileName = "recent.json";
  QJsonArray lastProjects = {};

  file.setFileName(fileName);

  if (QFileInfo::exists(fileName))
  {
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject values = doc.object();

    QJsonArray projects = values["projects"].toArray();
    projects.append(this->mSheetManager.GetProjectPath());

    auto recentProjects = QJsonObject({
        qMakePair(QString("projects"), projects),
    });

    doc.setObject(recentProjects);

    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(doc.toJson());
    file.close();
  }
}

// saves name and changed data of currently active chart
void MainView::saveCurrentChartData()
{
    QModelIndex row = this->sheets->index(this->model.getActiveIndex());
    QString sheetName = row.data(Qt::DisplayRole).toString();
    this->model.getChartData()->setName(sheetName);

    // saves current active chart
    this->model.saveActiveChart();
}

void MainView::switchCurrentChartData(int index)
{
    this->model.setActiveChart(index);
    this->readViewSettings();
    this->initializeChart();
}

void MainView::on_polyEdit_clicked()
{
  auto selection = this->ui->functions->selectionModel();
  if (!selection->hasSelection())
  {
    QMessageBox msgBox;
    msgBox.setText("No Row Selected.");
    msgBox.setInformativeText("Please select a row that you want to generate an equation for.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
    return;
  }

  this->model.openPolyEdit(selection->currentIndex().row(), this);
}

void MainView::on_actionSpeichern_unter_triggered()
{
  this->saveCurrentChartData();
  auto fileName =
      QFileDialog::getSaveFileName(this, tr("Save Project to"), "/home", tr("Json Files (*.json)"));
  this->mSheetManager.saveProjectToFile(this->model.getChartList(), fileName);
  this->addRecentProject();
}

void MainView::on_actionSpeichern_triggered()
{
  this->saveCurrentChartData();
  if (this->mSheetManager.checkForExistingProject())
  {
    this->mSheetManager.saveProjectToFile(this->model.getChartList(),
                                          this->mSheetManager.GetProjectPath());
  }
  else
  {
    this->on_actionSpeichern_unter_triggered();
  }
}

void MainView::on_actionProjektmappe_schlie_en_triggered()
{
}

void MainView::on_update_clicked()
{
  try
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    initializeChart();
    setSeries();
    QApplication::restoreOverrideCursor();
  }
  catch (FunctionParserException e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, "The function contains an error.", e.getMessage().c_str(),
                             QMessageBox::Ok);
  }
  catch (DerivationException e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, "The derivation contains an error.", e.getMessage(),
                             QMessageBox::Ok);
  }
}

void MainView::on_tableWidget_cellActivated(int row, int column)
{
}

void MainView::on_viewAdd_clicked()
{
    int row = this->sheets->rowCount();
    this->sheets->insertRows(row,1);
    this->model.appendNewDefaultData();
    this->saveCurrentChartData();
    this->switchCurrentChartData(row);

    QModelIndex index = this->sheets->index(row);
    this->ui->sheetViews->setCurrentIndex(index);
    this->ui->sheetViews->edit(index);
}

void MainView::on_viewRename_clicked()
{
    int row = this->ui->sheetViews->currentIndex().row();
    QModelIndex index = this->sheets->index(row);
    this->ui->sheetViews->edit(index);
}


void MainView::on_viewDelete_clicked()
{
    int rows = this->sheets->rowCount();

    if (rows > 1) {
        int row = this->ui->sheetViews->currentIndex().row();
        this->sheets->removeRows(row,1);
        this->model.removeChart(row);
        this->switchCurrentChartData(0);
    }
}

// onclick action per view entry by index
void MainView::on_sheetViews_clicked(const QModelIndex &index)
{
    this->saveCurrentChartData();
    int sheetIndex = index.row();
    this->switchCurrentChartData(sheetIndex);
}

