/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#include "mainviewmodel.h"
#include "qregularexpression.h"
#include <Data/functiondata.h>
#include <Exceptions/DerivationException.h>
#include <QString>
#include <polyeditview.h>

const string BASE_LETTER = "r";

QRegularExpression derivateRegex("^derivate\\((.*?)\\)$",
                                 QRegularExpression::DotMatchesEverythingOption);

ChartData initializeDefaultChartData()
{
  ChartData result;
  result.setIsChartInverted(false);

  ViewArea va(0.0, 7.0, 0.0, 60.0);
  result.setViewArea(va);

  FunctionData defaultFunktionen[] = {
      FunctionData("x", "Produktionsfunktion", "6(r+r^2)-r^3", true),
      FunctionData("x'", "Grenzertrag", "derivate(x)", true),
      FunctionData("x''", "Grenzertragszuwachs", "derivate(x')", true),
      FunctionData("e", "Durschnittsertrag", "x/r", true),
      FunctionData("e'", "Ableitung", "derivate(e)", false),
  };

  for (auto singleEntry : defaultFunktionen)
  {
    FunctionData singleCopy(singleEntry);
    result.getFunctionData()->append(singleCopy);
  }

  ThresholdData defaultThresholds[] = {ThresholdData("Übergang Phase 1 zu 2", "x''", 0.0, true),
                                       ThresholdData("Übergang Phase 2 zu 3", "e'", 0.0, true),
                                       ThresholdData("Übergang Phase 3 zu 4", "x'", 0.0, true)};

  for (auto singleEntry : defaultThresholds)
  {
    ThresholdData singleCopy(singleEntry);
    result.getThresholdData()->append(singleCopy);
  }

  result.setName("4 Phasen Schema Produktion");
  return result;
}

ChartData initializeDefaultChartData2()
{
  ChartData result;
  result.setIsChartInverted(false);

  ViewArea va(0.0, 7.0, 0.0, 60.0);
  result.setViewArea(va);

  FunctionData defaultFunktionen[] = {
      FunctionData("k", "variable Kosten K", "(0.4390*r^3) + (-4.6492*r^2) + (19.6220*r^1)", true),
      FunctionData("k'", "Grenzkosten k'", "derivate(k)", true),
      FunctionData("k''", "Grenzkostenwachstum", "derivate(k')", false),
      FunctionData("v", "variable Stückkosten", "k/r", true),
      FunctionData("v'", "v. S. Wachstum", "derivate(v)", false),
      FunctionData("f", "fixkosten", "20", false),
      FunctionData("kf", "Gesamtkosten", "k+f", true),
      FunctionData("s", "Stückkosten", "kf/r", true),
      FunctionData("s'", "S. wachstum", "derivate(s)", false),
  };

  for (auto singleEntry : defaultFunktionen)
  {
    FunctionData singleCopy(singleEntry);
    result.getFunctionData()->append(singleCopy);
  }

  ThresholdData defaultThresholds[] = {ThresholdData("Übergang Phase 1 zu 2", "k''", 0.0, true),
                                       ThresholdData("Übergang Phase 2 zu 3", "v'", 0.0, true),
                                       ThresholdData("Übergang Phase 3 zu 4", "s'", 0.0, true)};

  for (auto singleEntry : defaultThresholds)
  {
    ThresholdData singleCopy(singleEntry);
    result.getThresholdData()->append(singleCopy);
  }

  result.setName("4 Phasen Schema Kosten");
  return result;
}

MainViewModel::MainViewModel()
{
  this->initializeDefaultData();
}

void MainViewModel::initializeDefaultData()
{
  QList<ChartData> defaultList;
  auto defaultData = initializeDefaultChartData();
  defaultList.append(defaultData);
  auto defaultData2 = initializeDefaultChartData2();
  defaultList.append(defaultData2);
  this->setChartList(defaultList);
  this->setChartData(defaultData);
}

void MainViewModel::appendNewDefaultData()
{
  auto defaultData = initializeDefaultChartData();
  this->addChart(defaultData);
}

void MainViewModel::generateAllSeries(QList<QLineSeries*>& seriesInLegend,
                                      QList<QLineSeries*>& series)
{
  double delta = this->chartData.getIsChartInverted() ? this->chartData.getViewArea()->getHeight()
                                                      : this->chartData.getViewArea()->getWidth();
  double from = this->chartData.getIsChartInverted() ? this->chartData.getViewArea()->getFromY()
                                                     : this->chartData.getViewArea()->getFromX();
  int resolution = 300;
  double stepSize = (double)delta / (double)resolution;

  QList<map<string, double>> variablesList;
  for (int i = 0; i <= resolution; i++)
  {
    double r = from + stepSize * i;
    variablesList.append(map<string, double>{{BASE_LETTER, r}});
  }

  seriesInLegend = QList<QLineSeries*>();
  series = QList<QLineSeries*>();
  int length = this->chartData.getFunctionData()->count();
  for (int i = 0; i < length; ++i)
  {
    FunctionData singleEnty = this->chartData.getFunctionData()->at(i);

    QString definition = *singleEnty.getDefinition();
    QRegularExpressionMatch match = derivateRegex.match(definition);

    if (match.hasMatch())
    {
      QString letter = match.captured(1).trimmed();
      this->calculateDerivation(singleEnty.getLetter()->toStdString(), variablesList,
                                letter.toStdString(), singleEnty.getName(), singleEnty.getIsShown(),
                                &seriesInLegend);
    }
    else
    {
      FunctionNode* func = new FunctionNode(definition.toStdString());
      this->calculateFunction(func, variablesList, singleEnty.getLetter()->toStdString(),
                              singleEnty.getName(), singleEnty.getIsShown(), &seriesInLegend);
      delete func;
    }
  }

  length = this->chartData.getThresholdData()->count();
  for (int i = 0; i < length; i++)
  {
    ThresholdData singleEnty = this->chartData.getThresholdData()->at(i);

    this->calculateYThresshold(singleEnty.getLetter()->toStdString(), variablesList,
                               singleEnty.getName(), singleEnty.getIsShown(),
                               singleEnty.getThreshold(), &series);
  }
}

void MainViewModel::openPolyEdit(int row, QWidget* parent)
{
  PolyEditView* polyView = new PolyEditView(parent);
  polyView->initialize(*this->getChartData()->getViewArea());
  polyView->exec();

  QString formula = polyView->getFormula();

  QList<FunctionData>* entries = this->getChartData()->getFunctionData();

  FunctionData data = entries->at(row);
  data.setDefinition(formula);
  entries->replace(row, data);

  delete polyView;
}

void MainViewModel::calculateFunction(FunctionNode* func, QList<map<string, double>>& variablesList,
                                      const string& letter, QString* name, bool isVisible,
                                      QList<QLineSeries*>* out)
{
  QLineSeries* series;
  if (isVisible)
  {
    series = new QLineSeries();
    series->setName(*name);
  }

  for (map<string, double>& singleVariables : variablesList)
  {
    double value = func->getResult(singleVariables);
    singleVariables.insert_or_assign(letter, value);
    if (value == NAN)
    {
      continue;
    }

    if (isVisible)
    {
      this->addPointToSeries(series, singleVariables.at(BASE_LETTER), value);
    }
  }

  if (isVisible)
  {
    out->append(series);
  }
}

bool isNan(const QPointF& value)
{
  return value.x() != value.x() || value.y() != value.y();
}

void MainViewModel::calculateDerivation(const string& letter,
                                        QList<map<string, double>>& variablesList,
                                        const string& letterToDerivate, QString* name,
                                        bool isVisible, QList<QLineSeries*>* out)
{
  if (variablesList.first().count(letterToDerivate) == 0)
  {
    throw DerivationException(QString("Unable to derivate %1. Letter not found.")
                                  .arg(QString::fromStdString(letterToDerivate)));
  }

  QLineSeries* series;
  if (isVisible)
  {
    series = new QLineSeries();
    series->setName(*name);
  }

  variablesList[0][letter] = NAN;

  int length = variablesList.count() - 1;
  for (int i = 1; i < length; i++)
  {
    auto lastVars = &variablesList[i - 1];
    QPointF last(lastVars->at(BASE_LETTER), lastVars->at(letterToDerivate));
    auto nextVars = &variablesList[i + 1];
    QPointF next(nextVars->at(BASE_LETTER), nextVars->at(letterToDerivate));

    auto thisVars = &variablesList[i];

    if (isNan(last) || isNan(next))
    {
      thisVars->insert_or_assign(letter, NAN);
      continue;
    }

    double value = (next.y() - last.y()) / (next.x() - last.x());
    thisVars->insert_or_assign(letter, value);

    if (isVisible)
    {
      this->addPointToSeries(series, thisVars->at(BASE_LETTER), value);
    }
  }

  variablesList[length][letter] = NAN;

  if (isVisible)
  {
    out->append(series);
  }
}

void MainViewModel::calculateYThresshold(const string& letter,
                                         QList<map<string, double>>& variablesList, QString* name,
                                         bool isVisible, double threshold, QList<QLineSeries*>* out)
{
  if (variablesList.first().count(letter) == 0)
  {
    throw DerivationException(QString("Unable to find threshold for %1. Letter not found.")
                                  .arg(QString::fromStdString(letter)));
  }

  ViewArea* viewArea = this->getChartData()->getViewArea();

  int length = variablesList.count() - 1;
  for (int i = 0; i < length; i++)
  {

    auto lastVars = &variablesList[i];
    QPointF last(lastVars->at(BASE_LETTER), lastVars->at(letter));
    auto nextVars = &variablesList[i + 1];
    QPointF next(nextVars->at(BASE_LETTER), nextVars->at(letter));

    double intersectionX;
    if (!tryFindIntersectionX(last, next, threshold, intersectionX))
    {
      continue;
    }

    QLineSeries* seriesVerical;
    QLineSeries* seriesHorizontal;
    if (isVisible)
    {
      seriesVerical = new QLineSeries();
      seriesVerical->setName(*name);
      seriesVerical->setPen(Qt::DashLine);
      seriesHorizontal = new QLineSeries();
      seriesHorizontal->setName(QString::number(threshold));
      seriesHorizontal->setPen(Qt::DashLine);
    }

    if (isVisible)
    {
      seriesVerical->append(intersectionX, viewArea->getFromY());
      seriesVerical->append(intersectionX, viewArea->getToY());
      seriesHorizontal->append(viewArea->getFromX(), threshold);
      seriesHorizontal->append(viewArea->getToX(), threshold);
    }

    if (isVisible)
    {
      out->append(seriesVerical);
      out->append(seriesHorizontal);
    }
  }
}

void MainViewModel::addPointToSeries(QXYSeries* series, double x, double y)
{
  if (!this->chartData.getIsChartInverted())
  {
    series->append(x, y);
  }
  else
  {
    series->append(y, x);
  }
}

bool MainViewModel::tryFindIntersectionX(QPointF a, QPointF b, double threshold,
                                         double& intersectionX)
{
  if (isNan(a) || isNan(b))
  {
    return false;
  }

  if (this->chartData.getIsChartInverted())
  {
    a = a.transposed();
    b = b.transposed();
  }

  if ((a.y() > threshold) == (b.y() > threshold))
  {
    return false;
  }

  float lambda = (threshold - b.y()) / (a.y() - b.y());
  intersectionX = (lambda * a + (1 - lambda) * b).x();

  return true;
}
