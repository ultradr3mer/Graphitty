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
  void OpenPolyEdit(int row, QWidget* parent);

private:
  void CalculateFunction(FunctionNode* func, QList<map<string, double>>& variablesList,
                         const string& letter, QString* name, bool isVisible,
                         QList<QLineSeries*>* out);
  void CalculateDerivation(const string& letter, QList<map<string, double>>& variablesList,
                           const string& letterToDerivate, QString* name, bool isVisible,
                           QList<QLineSeries*>* out);
  void CalculateYThresshold(const string& letter, QList<map<string, double>>& variablesList,
                            QString* name, bool isVisible, double threshold,
                            QList<QLineSeries*>* out);
  void addPointToSeries(QXYSeries* series, double x, double y);
  bool tryFindIntersectionX(QPointF a, QPointF b, double threshold, double& intersectionX);

  ChartData chartData;
};

#endif // MAINVIEWMODEL_H
