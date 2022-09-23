#include "mainviewmodel.h"
#include "qregularexpression.h"
#include <Data/functiondata.h>
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
    result.setFunctionData()->append(singleCopy);
  }

  ThresholdData defaultThresholds[] = {ThresholdData("Übergang Phase 1 zu 2", "x''", 0.0, true),
                                       ThresholdData("Übergang Phase 2 zu 3", "e'", 0.0, true),
                                       ThresholdData("Übergang Phase 3 zu 4", "x'", 0.0, true)};

  for (auto singleEntry : defaultThresholds)
  {
    ThresholdData singleCopy(singleEntry);
    result.getThresholdData()->append(singleCopy);
  }
  return result;
}

MainViewModel::MainViewModel()
{
  auto defaultData = initializeDefaultChartData();
  this->setChartData(defaultData);
}

QList<QLineSeries*>* MainViewModel::generateAllSeries()
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

  auto result = new QList<QLineSeries*>();
  int length = this->chartData.setFunctionData()->count();
  for (int i = 0; i < length; ++i)
  {
    FunctionData singleEnty = this->chartData.setFunctionData()->at(i);

    QString definition = *singleEnty.getDefinition();
    QRegularExpressionMatch match = derivateRegex.match(definition);

    if (match.hasMatch())
    {
      QString letter = match.captured(1).trimmed();
      this->calculateDerivation(singleEnty.getLetter()->toStdString(), variablesList,
                                letter.toStdString(), singleEnty.getName(), singleEnty.getIsShown(),
                                result);
    }
    else
    {
      FunctionNode* func = new FunctionNode(definition.toStdString());
      this->calculateFunction(func, variablesList, singleEnty.getLetter()->toStdString(),
                              singleEnty.getName(), singleEnty.getIsShown(), result);
      delete func;
    }
  }

  length = this->chartData.getThresholdData()->count();
  for (int i = 0; i < length; i++)
  {
    ThresholdData singleEnty = this->chartData.getThresholdData()->at(i);

    this->calculateYThresshold(singleEnty.getLetter()->toStdString(), variablesList,
                               singleEnty.getName(), singleEnty.getIsShown(),
                               singleEnty.getThreshold(), result);
  }

  return result;
}

void MainViewModel::openPolyEdit(int row, QWidget* parent)
{
  PolyEditView* polyView = new PolyEditView(parent);
  polyView->initialize(*this->getChartData()->getViewArea());
  polyView->exec();

  QString formula = polyView->getFormula();

  QList<FunctionData>* entries = this->getChartData()->setFunctionData();

  FunctionData data = entries->at(row);
  data.setDefinition(formula);
  entries->replace(row, data);
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

void MainViewModel::calculateDerivation(const string& letter,
                                        QList<map<string, double>>& variablesList,
                                        const string& letterToDerivate, QString* name,
                                        bool isVisible, QList<QLineSeries*>* out)
{
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
  ViewArea* viewArea = this->getChartData()->getViewArea();

  int length = variablesList.count() - 1;
  for (int i = 0; i < length; i++)
  {
    QLineSeries* series;
    if (isVisible)
    {
      series = new QLineSeries();
      series->setName(*name);
      series->setPen(Qt::DashLine);
    }

    auto lastVars = &variablesList[i];
    QPointF last(lastVars->at(BASE_LETTER), lastVars->at(letter));
    auto nextVars = &variablesList[i + 1];
    QPointF next(nextVars->at(BASE_LETTER), nextVars->at(letter));

    double intersectionX;
    if (!tryFindIntersectionX(last, next, threshold, intersectionX))
    {
      continue;
    }

    bool inverted = this->getChartData()->getIsChartInverted();
    if (isVisible)
    {
      this->addPointToSeries(series, intersectionX,
                             inverted ? viewArea->getFromX() : viewArea->getFromY());
      this->addPointToSeries(series, intersectionX,
                             inverted ? viewArea->getToX() : viewArea->getToY());
    }

    out->append(series);
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
  if ((a.y() > threshold) == (b.y() > threshold))
  {
    return false;
  }

  float lambda = (threshold - b.y()) / (a.y() - b.y());
  intersectionX = (lambda * a + (1 - lambda) * b).x();

  return true;
}
