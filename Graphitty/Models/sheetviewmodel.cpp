#include "sheetviewmodel.h"
#include "Models/mainviewmodel.h"
#include "Data/chartdata.h"

SheetViewModel::SheetViewModel(QObject* parent) : QStringListModel(parent)
{}

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


