#include "mainview.h"
#include "polyeditview.h"
#include "ui_mainview.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <fstream>
#include <qlineseries.h>
#include <qvalueaxis.h>

MainView::MainView(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainView)
{
  ui->setupUi(this);
  this->initializeChart(7);
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
  QJsonValue subobj = item["toX"];
  qWarning() << subobj.toInt();

  /* in case of array get array and convert into string*/
  // qWarning() << tr("QJsonObject[function] of value: ") << item["content"];
  // QJsonArray test = item["content"].toArray();
  // qWarning() << test[1].toString();

  this->initializeChart(item["fromX"].toInt(), item["toX"].toInt(), item["fromY"].toInt(),
                        item["toY"].toInt());
}

double produktionsFunktion(double r)
{
  return 6.0 * r + 6.0 * pow(r, 2.0) - pow(r, 3.0);
}

void MainView::initializeChart(double fromX, double toX, double fromY, double toY)
{
  QLineSeries* produktionsWerte = new QLineSeries();
  produktionsWerte->setName("Produktionsfunktion x");
  QLineSeries* grenzertrag = new QLineSeries();
  grenzertrag->setName("Grenzertrag x'");
  QLineSeries* grenzertragZuwachs = new QLineSeries();
  grenzertragZuwachs->setName("Grenzertragszuwachs x''");
  QLineSeries* durchschnittsErtrag = new QLineSeries();
  durchschnittsErtrag->setName("Durschnittsertrag e");

  double from = 0;
  double to = 7;

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
  axisX->setRange(fromX, toX);
  chart->addAxis(axisX, Qt::AlignBottom);
  produktionsWerte->attachAxis(axisX);
  grenzertrag->attachAxis(axisX);
  grenzertragZuwachs->attachAxis(axisX);
  durchschnittsErtrag->attachAxis(axisX);

  QValueAxis* axisY = new QValueAxis();
  axisY->setRange(fromY, toY);
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
