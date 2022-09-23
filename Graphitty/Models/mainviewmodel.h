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

  inline ChartData* getChartData()
  {
    return &this->chartData;
  }
  inline void setChartData(ChartData& value)
  {
    this->chartData = value;
  }

  QList<QLineSeries*>* generateAllSeries();
  void openPolyEdit(int row, QWidget* parent);

private:
  void calculateFunction(FunctionNode* func, QList<map<string, double>>& variablesList,
                         const string& letter, QString* name, bool isVisible,
                         QList<QLineSeries*>* out);
  void calculateDerivation(const string& letter, QList<map<string, double>>& variablesList,
                           const string& letterToDerivate, QString* name, bool isVisible,
                           QList<QLineSeries*>* out);
  void calculateYThresshold(const string& letter, QList<map<string, double>>& variablesList,
                            QString* name, bool isVisible, double threshold,
                            QList<QLineSeries*>* out);
  void addPointToSeries(QXYSeries* series, double x, double y);
  bool tryFindIntersectionX(QPointF a, QPointF b, double threshold, double& intersectionX);

  ChartData chartData;
};

#endif // MAINVIEWMODEL_H
