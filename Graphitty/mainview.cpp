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

string BASE_LETTER = "r";

MainView::MainView(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainView)
{
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

bool tryFindIntersectionX(QPointF a, QPointF b, float threshold, float& intersectionX)
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
  axisX->setRange(mFromX, mToX);
  chart->addAxis(axisX, Qt::AlignBottom);
  this->axisX = axisX;

  QValueAxis* axisY = new QValueAxis();
  axisY->setRange(mFromY, mToY);
  chart->addAxis(axisY, Qt::AlignLeft);
  this->axisY = axisY;

  this->ui->chartView->setChart(chart);
  this->chart = chart;

  double from = mFromCalc;
  double to = mToCalc;

  double delta = to - from;
  int resolution = 1000;
  double stepSize = (double)delta / (double)resolution;

  QList<map<string, double>> points;
  for (int i = 0; i <= resolution; i++)
  {
    double r = from + stepSize * i;
    points.append(map<string, double>{{BASE_LETTER, r}});
  }

  this->addFunctionToChart(&mCalculation, points, "x", "Produktionsfunktion x");
  this->addDerivationToChart(points, "x", "Grenzertrag x'");
  this->addDerivationToChart(points, "x'", "Grenzertragszuwachs x''");
  this->addFunctionToChart(&mOtherCalculation, points, "e", "Durschnittsertrag e");
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

QLineSeries* MainView::addDerivationToChart(QList<map<string, double>>& variablesList,
                                            const string& letterToDerivate, const QString& name)
{
  QLineSeries* series = new QLineSeries();
  series->setName(name);

  int length = variablesList.count();
  for (int i = 0; i < length; i++)
  {
    auto lastVars = &variablesList[max(i - 1, 0)];
    QPointF last(lastVars->at(BASE_LETTER), lastVars->at(letterToDerivate));
    auto nextVars = &variablesList[min(i + 1, length - 1)];
    QPointF next(nextVars->at(BASE_LETTER), nextVars->at(letterToDerivate));

    double value = (next.y() - last.y()) / (next.x() - last.x());
    if (value == NAN)
    {
      continue;
    }

    auto thisVars = &variablesList[i];
    thisVars->insert_or_assign(letterToDerivate + "'", value);
    series->append(thisVars->at(BASE_LETTER), value);
  }

  this->chart->addSeries(series);
  series->attachAxis(this->axisX);
  series->attachAxis(this->axisY);

  return series;
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
