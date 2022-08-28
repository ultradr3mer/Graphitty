#include "mainview.h"
#include "polyeditview.h"
#include "ui_mainview.h"

#include "FunctionParser/functionnode.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPointF>
#include <fstream>
#include <qlineseries.h>
#include <qvalueaxis.h>

MainView::MainView(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainView)
{
  ui->setupUi(this);

  this->mCalculation = FunctionNode("6(r+r^2)-r^3");
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
  mFromX = item["fromX"].toInt();
  mToX = item["toX"].toInt();
  mFromY = item["fromY"].toInt();
  mToY = item["toY"].toInt();

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

bool tryFindIntersection(QPointF a, QPointF b, float threshold, QPointF& pointOfIntersection)
{
  if ((a.y() > 0) == (b.y() > 0))
  {
    return false;
  }

  float lambda = (threshold - b.y()) / (a.y() - b.y());
  pointOfIntersection = lambda * a + (1 - lambda) * b;

  return true;
}

void MainView::initializeChart()
{
  QChart* chart = new QChart();

  QValueAxis* axisX = new QValueAxis();
  axisX->setRange(mFromX, mToX);
  chart->addAxis(axisX, Qt::AlignBottom);
  this->axisX = axisX;

  QValueAxis* axisY = new QValueAxis();
  axisY->setRange(mFromY, mToY);
  chart->addAxis(axisY, Qt::AlignLeft);
  this->axisY = axisY;

  this->ui->chartView->setChart(chart);
  this->chart = chart;

  auto prodSeries = this->addFunctionToChart(&mCalculation, "Produktionsfunktion x");
  auto grenzSeries = this->addDerivationToChart(prodSeries, "Grenzertrag x'");
  this->addDerivationToChart(grenzSeries, "Grenzertragszuwachs x''");
}

QLineSeries* MainView::addFunctionToChart(FunctionNode* func, const QString& name)
{
  QLineSeries* series = new QLineSeries();
  series->setName(name);

  double from = mFromCalc;
  double to = mToCalc;

  double delta = to - from;
  int resolution = 1000;
  double stepSize = (double)delta / (double)resolution;
  double* xValues = new double[resolution + 1];
  double* currentXValue = xValues;
  for (int i = 0; i <= resolution; i++)
  {
    double r = from + stepSize * i;
    double x = func->getResult(r);
    series->append(r, x);
    *currentXValue = x;
    currentXValue++;
  }

  this->chart->addSeries(series);
  series->attachAxis(this->axisX);
  series->attachAxis(this->axisY);

  return series;
}

QLineSeries* MainView::addDerivationToChart(QLineSeries* series, const QString& name)
{
  QLineSeries* derivation = new QLineSeries();
  derivation->setName(name);

  for (int i = 0; i < (series->count() - 1); i++)
  {
    QPointF last = series->at(i);
    QPointF next = series->at(i + 1);

    double x = (last.x() + next.x()) / 2.0;
    double y = (next.y() - last.y()) / (next.x() - last.x());

    derivation->append(x, y);
  }

  this->chart->addSeries(derivation);
  derivation->attachAxis(this->axisX);
  derivation->attachAxis(this->axisY);

  return derivation;
}

void MainView::on_polyEdit_clicked()
{
  auto* polyView = new PolyEditView(this);
  polyView->show();
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
    mCalculation = FunctionNode(ui->lineEditFunction->text().toStdString());
    ui->lineEditFunction->setText(QString(mCalculation.toStrnig().c_str()));
    initializeChart();
  }
  catch (FunctionParserException e)
  {
    // ui->textEdit->setPlainText(e.getMessage().c_str());
    QMessageBox::information(this, "The function contains an error.", e.getMessage().c_str(),
                             QMessageBox::Ok);
  }
}
