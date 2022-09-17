#ifndef VIEWDATA_H
#define VIEWDATA_H

#include "functiondata.h"

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

  inline QList<FunctionData> GetFunctionData()
  {
    return this->functionData;
  }
  inline void SetFunctionData(QList<FunctionData> value)
  {
    this->functionData = value;
  }

private:
  bool isChartInverted;
  ViewArea viewArea;
  QList<FunctionData> functionData;
};

#endif // VIEWDATA_H
