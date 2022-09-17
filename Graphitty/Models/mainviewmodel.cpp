#include "mainviewmodel.h"
#include "qregularexpression.h"
#include <Data/functiondata.h>

const string BASE_LETTER = "r";

QRegularExpression derivateRegex("^derivate\\((.*?)\\)$",
                                 QRegularExpression::DotMatchesEverythingOption);

ChartData initializeDefaultChartData()
{
  ChartData result;
  result.SetIsChartInverted(false);

  ViewArea va(0.0, 7.0, 0.0, 60.0);
  result.SetViewArea(va);

  FunctionData defaultFunktionen[] = {
      FunctionData("x", "Produktionsfunktion", "6(r+r^2)-r^3", true),
      FunctionData("x'", "Grenzertrag", "derivate(x)", true),
      FunctionData("x''", "Grenzertragszuwachs", "derivate(x')", true),
      FunctionData("e", "Durschnittsertrag", "x/r", true),
      FunctionData("e'", "Ableitung", "derivate(e)", false),
  };
  auto copy = QList<FunctionData>();
  for (auto singleEntry : defaultFunktionen)
  {
    FunctionData singleCopy(singleEntry);
    copy.append(singleCopy);
  }

  return result;
}

MainViewModel::MainViewModel()
{
  auto defaultData = initializeDefaultChartData();
  this->SetChartData(defaultData);
}

QList<QLineSeries*>* MainViewModel::GenerateAllSeries()
{
  double delta = this->chartData.GetIsChartInverted() ? this->chartData.GetViewArea()->getHeight()
                                                      : this->chartData.GetViewArea()->getWidth();
  double from = this->chartData.GetIsChartInverted() ? this->chartData.GetViewArea()->getFromY()
                                                     : this->chartData.GetViewArea()->getFromX();
  int resolution = 300;
  double stepSize = (double)delta / (double)resolution;

  QList<map<string, double>> variablesList;
  for (int i = 0; i <= resolution; i++)
  {
    double r = from + stepSize * i;
    variablesList.append(map<string, double>{{BASE_LETTER, r}});
  }

  auto result = new QList<QLineSeries*>();
  auto functionData = this->chartData.GetFunctionData();
  for (auto& singleEnty : functionData)
  {
    auto definition = *singleEnty.GetDefinition();
    auto match = derivateRegex.match(definition);

    if (match.hasMatch())
    {
      auto letter = match.captured(1).trimmed();
      this->CalculateDerivation(letter.toStdString(), variablesList,
                                singleEnty.GetLetter()->toStdString(), singleEnty.GetName(),
                                result);
    }
    else
    {
      auto func = new FunctionNode(definition.toStdString());
      this->CalculateFunction(func, variablesList, singleEnty.GetLetter()->toStdString(),
                              singleEnty.GetDefinition(), result);
      delete func;
    }
  }

  //  for (int i = 0; i < this->ui->yThresholds->rowCount(); i++)
  //  {
  //    this->CalculateYThresshold(variablesList,
  //                               this->ui->yThresholds->item(i, 1)->text().toStdString(),
  //                               this->ui->yThresholds->item(i, 2)->text().toDouble(),
  //                               this->ui->yThresholds->item(i, 0)->text());
  //  }

  return result;
}

void MainViewModel::CalculateFunction(FunctionNode* func, QList<map<string, double>>& variablesList,
                                      const string& letter, QString* name, QList<QLineSeries*>* out)
{
  QLineSeries* series = new QLineSeries();
  series->setName(*name);

  for (map<string, double>& singleVariables : variablesList)
  {
    double value = func->getResult(singleVariables);
    singleVariables.insert_or_assign(letter, value);
    if (value == NAN)
    {
      continue;
    }

    this->AddPointToSeries(series, singleVariables.at(BASE_LETTER), value);
  }

  out->append(series);
}

void MainViewModel::CalculateDerivation(const string& letter,
                                        QList<map<string, double>>& variablesList,
                                        const string& letterToDerivate, QString* name,
                                        QList<QLineSeries*>* out)
{
  QLineSeries* series = new QLineSeries();
  series->setName(*name);

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

    this->AddPointToSeries(series, thisVars->at(BASE_LETTER), value);
  }

  variablesList[length][letter] = NAN;

  out->append(series);
}

void MainViewModel::AddPointToSeries(QXYSeries* series, double x, double y)
{
  if (!this->chartData.GetIsChartInverted())
  {
    series->append(x, y);
  }
  else
  {
    series->append(y, x);
  }
}
