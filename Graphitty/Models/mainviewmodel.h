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
  inline QList<ChartData> getChartList()
  {
    return this->chartList;
  }
  inline void setChartList(QList<ChartData> list)
  {
    this->chartList = list;
  }
  inline void addChart(ChartData& value)
  {
    this->chartList.append(value);
  }
  inline void removeChart(int index)
  {
    this->chartList.removeAt(index);
  }
  inline void setActiveChart(int index)
  {
    this->chartData = this->chartList[index];
    this->activeChartIndex = index;
  }
  inline void saveActiveChart()
  {
    this->chartList[this->activeChartIndex] = this->chartData;
  }
  inline int getActiveIndex()
  {
    return this->activeChartIndex;
  }
  void generateAllSeries(QList<QLineSeries *> &seriesInLegend, QList<QLineSeries *> &series);
  void openPolyEdit(int row, QWidget* parent);
  void initializeDefaultData();
  void appendNewDefaultData();

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
  int activeChartIndex = 0;
  QList<ChartData> chartList;
};

#endif // MAINVIEWMODEL_H
