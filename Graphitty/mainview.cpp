#include "mainview.h"
#include "polyeditview.h"
#include "ui_mainview.h"

#include "FunctionParser/functionnode.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
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

double MainView::produktionsFunktion(double r) // QString mCalculation
{
  // ToDo QString input into CalcNode return
  // return 6.0 * r + 6.0 * pow(r, 2.0) - pow(r, 3.0);
  return mCalculation.getResult(r);
}

void MainView::initializeChart()
{
  QLineSeries* produktionsWerte = new QLineSeries();
  produktionsWerte->setName("Produktionsfunktion x");
  QLineSeries* grenzertrag = new QLineSeries();
  grenzertrag->setName("Grenzertrag x'");
  QLineSeries* grenzertragZuwachs = new QLineSeries();
  grenzertragZuwachs->setName("Grenzertragszuwachs x''");
  QLineSeries* durchschnittsErtrag = new QLineSeries();
  durchschnittsErtrag->setName("Durschnittsertrag e");

  double from = mFromCalc;
  double to = mToCalc;

  double delta = to - from;
  int resolution = 1000;
  double stepSize = (double)delta / (double)resolution;

  double* xValues = new double[resolution + 1];
  double* xDerivativeValues = new double[resolution - 1];

  double* currentXValue = xValues;
  for (int i = 0; i <= resolution; i++)
  {
    double r = from + stepSize * i;
    double x = produktionsFunktion(r);
    produktionsWerte->append(r, x);
    double e = x / r;
    durchschnittsErtrag->append(r, e);

    *currentXValue = x;
    currentXValue++;
  }

  double* currentValue = xValues + 1;
  double* currentXDerivativeValue = xDerivativeValues;
  for (int i = 1; i <= (resolution - 1); i++)
  {
    double r = from + stepSize * i;

    double lastX = *(currentValue - 1);
    double nextX = *(currentValue + 1);
    double xDerivative = (nextX - lastX) / stepSize / 2.0;
    grenzertrag->append(r, xDerivative);

    *currentXDerivativeValue = xDerivative;

    currentValue++;
    currentXDerivativeValue++;
  }

  currentXDerivativeValue = xDerivativeValues + 1;
  for (int i = 2; i <= (resolution - 2); i++)
  {
    double r = from + stepSize * i;

    double lastXDreivative = *(currentXDerivativeValue - 1);
    double nextXDerivative = *(currentXDerivativeValue + 1);
    double xDerivativeTwo = (nextXDerivative - lastXDreivative) / stepSize / 2.0;
    grenzertragZuwachs->append(r, xDerivativeTwo);
    currentXDerivativeValue++;
  }

  delete[] xValues;
  delete[] xDerivativeValues;

  QChart* chart = new QChart();
  //    chart->legend()->hide();
  chart->addSeries(produktionsWerte);
  chart->addSeries(grenzertrag);
  chart->addSeries(grenzertragZuwachs);
  chart->addSeries(durchschnittsErtrag);

  QValueAxis* axisX = new QValueAxis();
  axisX->setRange(mFromX, mToX);
  chart->addAxis(axisX, Qt::AlignBottom);
  produktionsWerte->attachAxis(axisX);
  grenzertrag->attachAxis(axisX);
  grenzertragZuwachs->attachAxis(axisX);
  durchschnittsErtrag->attachAxis(axisX);

  QValueAxis* axisY = new QValueAxis();
  axisY->setRange(mFromY, mToY);
  chart->addAxis(axisY, Qt::AlignLeft);
  produktionsWerte->attachAxis(axisY);
  grenzertrag->attachAxis(axisY);
  grenzertragZuwachs->attachAxis(axisY);
  durchschnittsErtrag->attachAxis(axisY);

  chart->setTitle("Simple line chart example");

  this->ui->chartView->setChart(chart);
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
