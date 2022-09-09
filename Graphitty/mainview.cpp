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
#include <fstream>
#include <qlineseries.h>
#include <qvalueaxis.h>

const string BASE_LETTER = "r";

QRegularExpression derivateRegex("^derivate\\((.*?)\\)$",
                                 QRegularExpression::DotMatchesEverythingOption);

MainView::MainView(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainView)
{
  this->viewArea = ViewArea(0.0, 7.0, 0.0, 60.0);

  ui->setupUi(this);

  this->mCalculation = FunctionNode("6(r+r^2)-r^3");
  this->mOtherCalculation = FunctionNode("x/r");
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
  this->viewArea = ViewArea(item["fromX"].toDouble(), item["toX"].toDouble(),
                            item["fromY"].toDouble(), item["toY"].toDouble());

  mCalculation = FunctionNode(item["calculation"].toString().toStdString());
  mFromCalc = item["fromCalc"].toInt();
  mToCalc = item["toCalc"].toInt();

  mImported = true;

  /* in case of array get array and convert into string*/
  // qWarning() << tr("QJsonObject[function] of value: ") << item["content"];
  // QJsonArray test = item["content"].toArray();
  // qWarning() << test[1].toString();

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

  QValueAxis* axisX = new QValueAxis();
  axisX->setRange(viewArea.getFromX(), viewArea.getToX());
  chart->addAxis(axisX, Qt::AlignBottom);
  this->axisX = axisX;

  QValueAxis* axisY = new QValueAxis();
  axisY->setRange(viewArea.getFromY(), viewArea.getToY());
  chart->addAxis(axisY, Qt::AlignLeft);
  this->axisY = axisY;

  this->ui->chartView->setChart(chart);
  this->chart = chart;

  this->ui->fromX->setValue(viewArea.getFromX());
  this->ui->toX->setValue(viewArea.getToX());
  this->ui->fromY->setValue(viewArea.getFromY());
  this->ui->toY->setValue(viewArea.getToY());

  this->setSeries();
}

QLineSeries* MainView::addFunctionToChart(FunctionNode* func,
                                          QList<map<string, double>>& variablesList,
                                          const string& letter, const QString& name)
{
  QLineSeries* series = new QLineSeries();
  series->setName(name);

  for (map<string, double>& singleVariables : variablesList)
  {
    double value = func->getResult(singleVariables);
    singleVariables.insert_or_assign(letter, value);
    if (value == NAN)
    {
      continue;
    }

    series->append(singleVariables.at(BASE_LETTER), value);
  }

  this->chart->addSeries(series);
  series->attachAxis(this->axisX);
  series->attachAxis(this->axisY);

  return series;
}

QLineSeries* MainView::addDerivationToChart(const string& letter,
                                            QList<map<string, double>>& variablesList,
                                            const string& letterToDerivate, const QString& name)
{
  QLineSeries* series = new QLineSeries();
  series->setName(name);

  variablesList[0][letter] = NAN;

  int length = variablesList.count() - 1;
  for (int i = 1; i < length; i++)
  {
    auto lastVars = &variablesList[max(i - 1, 0)];
    QPointF last(lastVars->at(BASE_LETTER), lastVars->at(letterToDerivate));
    auto nextVars = &variablesList[min(i + 1, length - 1)];
    QPointF next(nextVars->at(BASE_LETTER), nextVars->at(letterToDerivate));

    double value = (next.y() - last.y()) / (next.x() - last.x());
    auto thisVars = &variablesList[i];
    thisVars->insert_or_assign(letter, value);

    if (value == NAN)
    {
      continue;
    }

    series->append(thisVars->at(BASE_LETTER), value);
  }

  variablesList[length][letter] = NAN;

  this->chart->addSeries(series);
  series->attachAxis(this->axisX);
  series->attachAxis(this->axisY);

  return series;
}

QList<QLineSeries*>* MainView::addYThresholdToChart(QList<map<string, double>>& variablesList,
                                                    const string& letter, double threshold,
                                                    const QString& name)
{
  auto listOfSeries = new QList<QLineSeries*>{};

  int length = variablesList.count() - 1;
  for (int i = 0; i < length; i++)
  {
    QLineSeries* series = new QLineSeries();
    series->setName(name);
    series->setPen(Qt::DashLine);

    auto lastVars = &variablesList[i];
    QPointF last(lastVars->at(BASE_LETTER), lastVars->at(letter));
    auto nextVars = &variablesList[i + 1];
    QPointF next(nextVars->at(BASE_LETTER), nextVars->at(letter));

    double intersectionX;
    if (!tryFindIntersectionX(last, next, threshold, intersectionX))
    {
      continue;
    }

    series->append(intersectionX, this->viewArea.getFromY());
    series->append(intersectionX, this->viewArea.getToY());

    this->chart->addSeries(series);
    series->attachAxis(this->axisX);
    series->attachAxis(this->axisY);

    listOfSeries->append(series);
  }

  return listOfSeries;
}

void MainView::setSeries()
{
  this->viewArea = ViewArea(this->ui->fromX->value(), this->ui->toX->value(),
                            this->ui->fromY->value(), this->ui->toY->value());

  this->axisX->setRange(this->viewArea.getFromX(), this->viewArea.getToX());
  this->axisY->setRange(this->viewArea.getFromY(), this->viewArea.getToY());

  this->chart->removeAllSeries();

  double delta = this->viewArea.getWidth();
  int resolution = 300;
  double stepSize = (double)delta / (double)resolution;

  QList<map<string, double>> variablesList;
  for (int i = 0; i <= resolution; i++)
  {
    double r = this->ui->fromX->value() + stepSize * i;
    variablesList.append(map<string, double>{{BASE_LETTER, r}});
  }

  QList<FunctionNode*> functions;
  for (int i = 0; i < this->ui->tableWidget->rowCount(); i++)
  {
    auto definition = this->ui->tableWidget->item(i, 2)->text();
    auto match = derivateRegex.match(definition);
    if (match.hasMatch())
    {
      auto letter = match.captured(1).trimmed();
      this->addDerivationToChart(this->ui->tableWidget->item(i, 0)->text().toStdString(),
                                 variablesList, letter.toStdString(),
                                 this->ui->tableWidget->item(i, 1)->text());
    }
    else
    {
      auto func = new FunctionNode(definition.toStdString());
      this->addFunctionToChart(func, variablesList,
                               this->ui->tableWidget->item(i, 0)->text().toStdString(),
                               this->ui->tableWidget->item(i, 1)->text());
      functions.append(func);
    }
  }

  for (int i = 0; i < this->ui->yThresholds->rowCount(); i++)
  {
    this->addYThresholdToChart(variablesList,
                               this->ui->yThresholds->item(i, 1)->text().toStdString(),
                               this->ui->yThresholds->item(i, 2)->text().toDouble(),
                               this->ui->yThresholds->item(i, 0)->text());
  }

  for (auto singleFunction : functions)
  {
    delete singleFunction;
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
  polyView->initialize(this->viewArea);
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
