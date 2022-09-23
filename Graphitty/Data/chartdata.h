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

  inline bool GetIsChartInverted()
  {
    return isChartInverted;
  }
  inline void SetIsChartInverted(bool value)
  {
    isChartInverted = value;
  }

  inline ViewArea* GetViewArea()
  {
    return &this->viewArea;
  }
  inline void SetViewArea(const ViewArea& value)
  {
    this->viewArea = value;
  }

  inline QList<FunctionData>* GetFunctionData()
  {
    return &this->functionData;
  }
  inline void SetFunctionData(QList<FunctionData>& value)
  {
    this->functionData = value;
  }

  inline QList<ThresholdData>* GetThresholdData()
  {
    return &this->thresholdData;
  }
  inline void SetThresholdData(QList<ThresholdData>& value)
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
