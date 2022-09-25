#include "sheetmanager.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QString>
#include <iostream>

#include <Data/chartdata.h>
#include <Data/functiondata.h>
#include <Data/thresholddata.h>
#include <viewarea.h>

// creates new sheet manager for projects
SheetManager::SheetManager()
{
}

SheetManager::~SheetManager()
{
}

// reads project file into view model
QList<ChartData> SheetManager::readProject(const QString& filePath)
{
  this->mProjectPath = filePath;
  QString val;
  QFile file;

  file.setFileName(filePath);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  val = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
  QJsonObject values = doc.object();

  QJsonValue sheets = values.value(QString("sheets"));
  QList<ChartData> charts = this->convertSheetsToChart(sheets);

  return charts;
}

// final save action to translate to external file
void SheetManager::saveProjectToFile(QList<ChartData> charts, const QString& filePath)
{
  QJsonValue sheets = this->convertSheetsToJson(charts);

  QFile file(filePath);
  QJsonDocument* document = new QJsonDocument(QJsonArray());

  auto objectData = QJsonObject({
      qMakePair(QString("sheets"), sheets),
  });

  document->setObject(objectData);

  file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
  file.write(document->toJson());
  file.close();

  this->mProjectPath = filePath;
  delete(document);
}

QList<ChartData> SheetManager::convertSheetsToChart(QJsonValue sheets)
{
    QList<ChartData> charts;


    auto sheetList = sheets.toArray();

    foreach (const QJsonValue& sheet, sheetList)
    {
        ChartData chart;
        auto view = sheet["view"].toObject();
        auto functions = sheet["function"].toArray();
        auto thresholds = sheet["threshold"].toArray();

        // sets Inverted
        chart.setIsChartInverted(sheet["inverted"].toBool());

        // sets Name
        chart.setName(sheet["name"].toString().toStdString().c_str());

        // sets View
        ViewArea va(view["fromX"].toDouble(), view["toX"].toDouble(), view["fromY"].toDouble(),
                    view["toY"].toDouble());
        chart.setViewArea(va);



        // sets Functions
        auto functionData = QList<FunctionData>();
        foreach (const QJsonValue& value, functions)
        {
            auto function = value.toObject();
            FunctionData fc(function["letter"].toString().toStdString().c_str(),
                            function["name"].toString().toStdString().c_str(),
                            function["definition"].toString().toStdString().c_str(),
                            function["shown"].toBool());

            functionData.append(fc);
        }

        // sets Thresholds
        auto thresholdData = QList<ThresholdData>();
        foreach (const QJsonValue& value, thresholds)
        {
            auto threshold = value.toObject();
            ThresholdData th(threshold["name"].toString().toStdString().c_str(),
                            threshold["letter"].toString().toStdString().c_str(),
                             threshold["threshold"].toDouble(),
                            threshold["shown"].toBool());

            thresholdData.append(th);
        }
        /* TODO */

        chart.setFunctionData(functionData);
        chart.setThresholdData(thresholdData);
        charts.append(chart);
    }

    return charts;
}

QJsonValue SheetManager::convertSheetsToJson(QList<ChartData> charts)
{
    QJsonArray sheets;

    for (auto chart : charts)
    {
        auto viewArea = chart.getViewArea();
        auto functionData = chart.getFunctionData();
        auto thresholdData = chart.getThresholdData();
        auto functions = QJsonArray();
        auto thresholds = QJsonArray();

        for (auto& singleEnty : *functionData)
        {
            auto function = QJsonObject({qMakePair(QString("letter"), *singleEnty.getLetter()),
                                         qMakePair(QString("name"), *singleEnty.getName()),
                                         qMakePair(QString("definition"), *singleEnty.getDefinition()),
                                         qMakePair(QString("shown"), singleEnty.getIsShown())});
            functions.append(function);
        }

        for (auto& singleEnty : *thresholdData)
        {
            auto threshold = QJsonObject({qMakePair(QString("letter"), *singleEnty.getLetter()),
                                         qMakePair(QString("name"), *singleEnty.getName()),
                                         qMakePair(QString("threshold"), singleEnty.getThreshold()),
                                         qMakePair(QString("shown"), singleEnty.getIsShown())});
            thresholds.append(threshold);
        }

        auto view = QJsonObject({qMakePair(QString("fromX"), viewArea->getFromX()),
                                 qMakePair(QString("toX"), viewArea->getToX()),
                                 qMakePair(QString("fromY"), viewArea->getFromY()),
                                 qMakePair(QString("toY"), viewArea->getToY())});

        auto sheet =
            QJsonObject({
                         qMakePair(QString("function"), functions),
                         qMakePair(QString("threshold"), thresholds),
                         qMakePair(QString("view"), view),
                         qMakePair(QString("name"), chart.getName()),
                         qMakePair(QString("inverted"), chart.getIsChartInverted()),
            });
        sheets.append(sheet);
    }

    return QJsonValue(sheets);
}

// checks if project path exists (if project already has a file path || was saved before)
bool SheetManager::checkForExistingProject()
{
  if (this->mProjectPath.isEmpty() || this->mProjectPath.isNull())
  {
    return false;
  }
  else
  {
    return true;
  }
}
