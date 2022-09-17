#include "mainview.h"
#include "polyeditview.h"
#include "ui_mainview.h"

#include "FunctionParser/functionnode.h"

#include <QFile>
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
#include <Models/functionstablemodel.h>

MainView::MainView(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainView), model(MainViewModel())
{

  ui->setupUi(this);

  MyModel* model = new MyModel;
  this->ui->testTable->setModel(model);
  this->readViewSettings();
  this->initializeChart();
}

MainView::~MainView()
{
  delete ui;
}

void MainView::openProject(const QString& fileName)
{
  QString val;
  QFile file;

  file.setFileName(fileName);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  val = file.readAll();
  file.close();

  qWarning() << val;

  QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
  QJsonObject values = d.object();
  QJsonValue value = values.value(QString("function"));
  QJsonObject item = value.toObject();

  /* in case of string value get value and convert into string*/
  auto viewArea = ViewArea(item["fromX"].toDouble(), item["toX"].toDouble(),
                           item["fromY"].toDouble(), item["toY"].toDouble());
  this->model.GetChartData()->SetViewArea(viewArea);

  mCalculation = FunctionNode(item["calculation"].toString().toStdString());
  mFromCalc = item["fromCalc"].toInt();
  mToCalc = item["toCalc"].toInt();

  mImported = true;

  this->initializeChart();
}

bool tryFindIntersectionX(QPointF a, QPointF b, double threshold, double& intersectionX)
{
  if ((a.y() > threshold) == (b.y() > threshold))
  {
    return false;
  }

  float lambda = (threshold - b.y()) / (a.y() - b.y());
  intersectionX = (lambda * a + (1 - lambda) * b).x();

  return true;
}

void MainView::initializeChart()
{
  QChart* chart = new QChart();

  auto viewArea = this->model.GetChartData()->GetViewArea();

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

// QLineSeries* MainView::addFunctionToChart(FunctionNode* func,
//                                           QList<map<string, double>>& variablesList,
//                                           const string& letter, const QString& name)
//{
//   QLineSeries* series = new QLineSeries();
//   series->setName(name);

//  for (map<string, double>& singleVariables : variablesList)
//  {
//    double value = func->getResult(singleVariables);
//    singleVariables.insert_or_assign(letter, value);
//    if (value == NAN)
//    {
//      continue;
//    }

//    this->addPointToSeries(series, singleVariables.at(BASE_LETTER), value);
//  }

//  this->chart->addSeries(series);
//  series->attachAxis(this->axisX);
//  series->attachAxis(this->axisY);

//  return series;
//}

// QLineSeries* MainView::addDerivationToChart(const string& letter,
//                                             QList<map<string, double>>& variablesList,
//                                             const string& letterToDerivate, const QString& name)
//{
//   QLineSeries* series = new QLineSeries();
//   series->setName(name);

//  variablesList[0][letter] = NAN;

//  int length = variablesList.count() - 1;
//  for (int i = 1; i < length; i++)
//  {
//    auto lastVars = &variablesList[max(i - 1, 0)];
//    QPointF last(lastVars->at(BASE_LETTER), lastVars->at(letterToDerivate));
//    auto nextVars = &variablesList[min(i + 1, length - 1)];
//    QPointF next(nextVars->at(BASE_LETTER), nextVars->at(letterToDerivate));

//    double value = (next.y() - last.y()) / (next.x() - last.x());
//    auto thisVars = &variablesList[i];
//    thisVars->insert_or_assign(letter, value);

//    if (value == NAN)
//    {
//      continue;
//    }

//    this->addPointToSeries(series, thisVars->at(BASE_LETTER), value);
//  }

//  variablesList[length][letter] = NAN;

//  this->chart->addSeries(series);
//  series->attachAxis(this->axisX);
//  series->attachAxis(this->axisY);

//  return series;
//}

// QList<QLineSeries*>* MainView::addYThresholdToChart(QList<map<string, double>>& variablesList,
//                                                     const string& letter, double threshold,
//                                                     const QString& name)
//{
//   auto listOfSeries = new QList<QLineSeries*>{};
//   auto viewArea = this->model.GetViewArea();

//  int length = variablesList.count() - 1;
//  for (int i = 0; i < length; i++)
//  {
//    QLineSeries* series = new QLineSeries();
//    series->setName(name);
//    series->setPen(Qt::DashLine);

//    auto lastVars = &variablesList[i];
//    QPointF last(lastVars->at(BASE_LETTER), lastVars->at(letter));
//    auto nextVars = &variablesList[i + 1];
//    QPointF next(nextVars->at(BASE_LETTER), nextVars->at(letter));

//    double intersectionX;
//    if (!tryFindIntersectionX(last, next, threshold, intersectionX))
//    {
//      continue;
//    }

//    bool inverted = this->ui->invert->isChecked();
//    this->addPointToSeries(series, intersectionX,
//                           inverted ? viewArea->getFromX() : viewArea->getFromY());
//    this->addPointToSeries(series, intersectionX,
//                           inverted ? viewArea->getToX() : viewArea->getToY());

//    this->chart->addSeries(series);
//    series->attachAxis(this->axisX);
//    series->attachAxis(this->axisY);

//    listOfSeries->append(series);
//  }

//  return listOfSeries;
//}

// void MainView::addPointToSeries(QXYSeries* series, double x, double y)
//{
//   if (!this->ui->invert->isChecked())
//   {
//     series->append(x, y);
//   }
//   else
//   {
//     series->append(y, x);
//   }
// }

void MainView::readViewSettings()
{
  auto viewArea = this->model.GetChartData()->GetViewArea();
  this->ui->fromX->setValue(viewArea->getFromX());
  this->ui->toX->setValue(viewArea->getToX());
  this->ui->fromY->setValue(viewArea->getFromY());
  this->ui->toY->setValue(viewArea->getToY());
  this->ui->invert->setChecked(this->model.GetChartData()->GetIsChartInverted());
  this->model.GetChartData()->SetViewArea(*viewArea);
}

void MainView::setSeries()
{
  this->readViewSettings();
  auto viewArea = this->model.GetChartData()->GetViewArea();
  this->axisX->setRange(viewArea->getFromX(), viewArea->getToX());
  this->axisY->setRange(viewArea->getFromY(), viewArea->getToY());

  this->chart->removeAllSeries();

  auto allSeries = this->model.GenerateAllSeries();

  for (auto singleSeries : *allSeries)
  {
    this->chart->addSeries(singleSeries);
    singleSeries->attachAxis(this->axisX);
    singleSeries->attachAxis(this->axisY);
  }
}

void MainView::on_polyEdit_clicked()
{
  int row = -1;
  for (QTableWidgetItem* item : this->ui->tableWidget->selectedItems())
  {
    row = item->row();
    break;
  }

  if (row == -1)
  {
    QMessageBox msgBox;
    msgBox.setText("No Row Selected.");
    msgBox.setInformativeText("Please select a row that you want to generate an equation for.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
    return;
  }

  auto polyView = new PolyEditView(this);
  polyView->initialize(*this->model.GetChartData()->GetViewArea());
  polyView->exec();

  auto formula = polyView->getFormula();
  this->ui->tableWidget->item(row, 2)->setText(formula);
}

void MainView::on_actionSpeichern_unter_triggered()
{
}

void MainView::on_actionSpeichern_triggered()
{
}

void MainView::on_actionProjektmappe_schlie_en_triggered()
{
}

void MainView::on_update_clicked()
{
  try
  {
    initializeChart();
    setSeries();
  }
  catch (FunctionParserException e)
  {
    // ui->textEdit->setPlainText(e.getMessage().c_str());
    QMessageBox::information(this, "The function contains an error.", e.getMessage().c_str(),
                             QMessageBox::Ok);
  }
}

void MainView::on_tableWidget_cellActivated(int row, int column)
{
}
