#include "mainview.h"
#include "Exceptions/thresholdexception.h"
#include "ui_mainview.h"

#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLegendMarker>
#include <QMessageBox>
#include <QPointF>
#include <QRegExp>
#include <QTableView>
#include <fstream>
#include <qlineseries.h>
#include <qvalueaxis.h>

#include <Data/functiondata.h>
#include <Exceptions/DerivationException.h>
#include <Exceptions/invalichartdataexception.h>
#include <IO/ioparser.h>

MainView::MainView(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainView), model(MainViewModel())
{
  ui->setupUi(this);

  this->chart = nullptr;

  this->model = MainViewModel();
  this->sheets = new SheetViewModel();
  this->sheets->setSheetViews(this->model);
  this->mIOParser = IOParser();

  functionstableModel = new FunctionsTableModel();
  functionstableModel->setFunctionData(this->model.getChartData()->getFunctionData());
  this->ui->functions->setModel(functionstableModel);

  thresholdsModel = new ThresholdsTableModel();
  thresholdsModel->setThresholdData(this->model.getChartData()->getThresholdData());
  this->ui->thesholds->setModel(thresholdsModel);

  this->ui->sheetViews->setModel(this->sheets);
  this->ui->sheetViews->setCurrentIndex(this->sheets->index(0));

  this->readViewSettings();
  this->safeInitializeChart();
}

MainView::~MainView()
{
  delete ui;
}

void MainView::openProject(const QString& fileName)
{
  QList<ChartData> loadedData = this->mIOParser.readProject(fileName);

    try {
      if(loadedData.isEmpty()) {
            this->on_actionNeu_triggered();
      } else {
          this->model.setChartList(loadedData);
          this->model.setActiveChart(0);
          this->sheets->setSheetViews(this->model);
          this->ui->sheetViews->setCurrentIndex(this->sheets->index(0));
          this->addRecentProject();
          this->readViewSettings();
          this->safeInitializeChart();
      }

    }
    catch(InvalidChartDataException e)
    {
        QMessageBox::information(this, "The program cannot load file data.", e.getMessage(),
                                 QMessageBox::Ok);
        this->on_actionNeu_triggered();
    }
}

void MainView::initializeChart()
{
  if (this->chart == nullptr)
  {
    this->chart = new QChart();
    this->axisX = new QValueAxis();
    this->axisY = new QValueAxis();
  }

  auto viewArea = this->model.getChartData()->getViewArea();

  this->axisX->setRange(viewArea->getFromX(), viewArea->getToX());
  chart->addAxis(this->axisX, Qt::AlignBottom);

  this->axisY->setRange(viewArea->getFromY(), viewArea->getToY());
  chart->addAxis(this->axisY, Qt::AlignLeft);

  chart->legend()->setInteractive(true);
  this->ui->chartView->setChart(chart);

  this->setSeries();
}

void MainView::safeInitializeChart()
{
  try
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    initializeChart();
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

  QList<QAbstractSeries*> list = this->ui->chartView->chart()->series();
  for (QAbstractSeries* singleSeries : list)
  {
    delete singleSeries;
  }

  this->chart->removeAllSeries();

  QList<QLineSeries*> seriesInLegend;
  QList<QLineSeries*> series;
  this->model.generateAllSeries(seriesInLegend, series);

  int index = 0;
  for (auto singleSeries : seriesInLegend)
  {
    this->chart->addSeries(singleSeries);
    singleSeries->attachAxis(this->axisX);
    singleSeries->attachAxis(this->axisY);

    chart->legend()->markers()[index++]->setVisible(true);
  }

  for (auto singleSeries : series)
  {
    this->chart->addSeries(singleSeries);
    singleSeries->attachAxis(this->axisX);
    singleSeries->attachAxis(this->axisY);

    chart->legend()->markers()[index++]->setVisible(false);
  }
}

void MainView::addRecentProject()
{
  QString val;
  QFile file;
  QString fileName = "recent.json";
  QJsonArray newProjectList;

  file.setFileName(fileName);

  if (QFileInfo::exists(fileName))
  {
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject values = doc.object();

    QJsonArray projects = values["projects"].toArray();

    foreach (const QJsonValue& project, projects)
    {
        if (project.toString() != this->mIOParser.getProjectPath()) {
            newProjectList.append(project.toString());
        }
    }

    newProjectList.append(this->mIOParser.getProjectPath());

    auto recentProjects = QJsonObject({
        qMakePair(QString("projects"), newProjectList),
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

// switches active chart view to selected one
void MainView::switchCurrentChartData(int index)
{
  this->model.setActiveChart(index);
  this->readViewSettings();

  emit functionstableModel->layoutAboutToBeChanged();
  functionstableModel->setFunctionData(this->model.getChartData()->getFunctionData());
  emit functionstableModel->layoutChanged();

  emit thresholdsModel->layoutAboutToBeChanged();
  thresholdsModel->setThresholdData(this->model.getChartData()->getThresholdData());
  emit thresholdsModel->layoutChanged();

  this->safeInitializeChart();
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

// saves current project to new selected file
void MainView::on_actionSpeichern_unter_triggered()
{
  this->saveCurrentChartData();
  auto fileName =
      QFileDialog::getSaveFileName(this, tr("Save Project to"), "/home", tr("Json Files (*.json)"));
  this->mIOParser.saveProjectToFile(this->model.getChartList(), fileName);
  this->addRecentProject();
}

// saves current project
void MainView::on_actionSpeichern_triggered()
{
  this->saveCurrentChartData();
  if (this->mIOParser.checkForExistingProject())
  {
    this->mIOParser.saveProjectToFile(this->model.getChartList(),
                                          this->mIOParser.getProjectPath());
  }
  else
  {
    this->on_actionSpeichern_unter_triggered();
  }
}

void MainView::on_actionProjektmappe_schlie_en_triggered()
{
    exit(0);
}

// updates view output
void MainView::on_update_clicked()
{
  this->safeInitializeChart();
}

// adds new view to project
void MainView::on_viewAdd_clicked()
{
  int row = this->sheets->rowCount();
  this->sheets->insertRows(row, 1);
  this->model.appendNewDefaultData();
  this->saveCurrentChartData();
  this->switchCurrentChartData(row);

  QModelIndex index = this->sheets->index(row);
  this->ui->sheetViews->setCurrentIndex(index);
  this->ui->sheetViews->edit(index);
}

// enables renaming for selected view
void MainView::on_viewRename_clicked()
{
  int row = this->ui->sheetViews->currentIndex().row();
  QModelIndex index = this->sheets->index(row);
  this->ui->sheetViews->edit(index);
}

// deletes seletected view
void MainView::on_viewDelete_clicked()
{
  int rows = this->sheets->rowCount();

  if (rows > 1)
  {
    int row = this->ui->sheetViews->currentIndex().row();
    this->sheets->removeRows(row, 1);
    this->model.removeChart(row);
    this->switchCurrentChartData(0);
  } else {
      QMessageBox msgBox;
      msgBox.setText("Last view cannot be deleted.");
      msgBox.setInformativeText("Either create a new one first or edit the last one to your needs.");
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      return;
  }
}

// onclick action per view entry by index
void MainView::on_sheetViews_clicked(const QModelIndex& index)
{
  this->saveCurrentChartData();
  int sheetIndex = index.row();
  this->switchCurrentChartData(sheetIndex);
}

void MainView::on_actionNeu_triggered()
{
    this->mIOParser.setProjectPath(NULL);
    this->model.initializeDefaultData();
    this->sheets->setSheetViews(this->model);
    this->ui->sheetViews->setCurrentIndex(this->sheets->index(0));
    this->switchCurrentChartData(0);
}


void MainView::on_actionOpen_triggered()
{
    QString openFile = QFileDialog::getOpenFileName(this, tr("Open Project"), "/home", tr("Json Files (*.json)"));
    this->openProject(openFile);
}

