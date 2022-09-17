#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include "Data/chartdata.h"
#include <FunctionParser/functionnode.h>
#include <QLineSeries>
#include <qstring.h>
#include <viewarea.h>

extern const string BASE_LETTER;

class MainViewModel
{
public:
  MainViewModel();

  inline ChartData* GetChartData()
  {
    return &this->chartData;
  }
  inline void SetChartData(ChartData& value)
  {
    this->chartData = value;
  }

  QList<QLineSeries*>* GenerateAllSeries();

private:
  void CalculateFunction(FunctionNode *func, QList<map<string, double>>& variablesList,
                         const string& letter, QString* name, QList<QLineSeries*>* out);
  void CalculateDerivation(const string& letter, QList<map<string, double>>& variablesList,
                           const string& letterToDerivate, QString* name, QList<QLineSeries*>* out);
  //  void CalculateYThresshold(FunctionNode* func, QList<map<string, double>>& variablesList,
  //                            string& letter, const QString& name, QList<QLineSeries*>& out);
  void AddPointToSeries(QXYSeries* series, double x, double y);

  ChartData chartData;
};

#endif // MAINVIEWMODEL_H
