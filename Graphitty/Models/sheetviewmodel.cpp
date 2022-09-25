/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#include "sheetviewmodel.h"
#include "Data/chartdata.h"
#include "Models/mainviewmodel.h"

SheetViewModel::SheetViewModel(QObject* parent) : QStringListModel(parent)
{
}

void SheetViewModel::setSheetViews(MainViewModel model)
{
  QStringList views;
  QList<ChartData> chartList = model.getChartList();

  for (auto chart : chartList)
  {
    views.append(chart.getName());
  }

  this->setStringList(views);
}
