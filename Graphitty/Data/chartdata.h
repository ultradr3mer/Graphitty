#ifndef VIEWDATA_H
#define VIEWDATA_H

#include "functiondata.h"
#include "thresholddata.h"

#include <QList>
#include <viewarea.h>

class ChartData
{
public:
  ChartData()
  {
    functionData = QList<FunctionData>();
  }

  inline bool getIsChartInverted()
  {
    return isChartInverted;
  }
  inline void setIsChartInverted(bool value)
  {
    isChartInverted = value;
  }

  inline ViewArea* getViewArea()
  {
    return &this->viewArea;
  }
  inline void setViewArea(const ViewArea& value)
  {
    this->viewArea = value;
  }

  inline QList<FunctionData>* getFunctionData()
  {
    return &this->functionData;
  }
  inline void setFunctionData(QList<FunctionData>& value)
  {
    this->functionData = value;
  }

  inline QList<ThresholdData>* getThresholdData()
  {
    return &this->thresholdData;
  }
  inline void setThresholdData(QList<ThresholdData>& value)
  {
    this->thresholdData = value;
  }

private:
  bool isChartInverted;
  ViewArea viewArea;
  QList<FunctionData> functionData;
  QList<ThresholdData> thresholdData;
};

#endif // VIEWDATA_H
