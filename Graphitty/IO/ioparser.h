/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef IOPARSER_H
#define IOPARSER_H

#include <Data/chartdata.h>
#include <Data/functiondata.h>
#include <QChart>
#include <QJsonArray>
#include <QList>
#include <viewarea.h>

class IOParser
{

public:
  IOParser();
  ~IOParser();
  QList<ChartData> readProject(const QString& filePath);
  void saveProjectToFile(QList<ChartData> charts, const QString& filePath);
  bool checkForExistingProject();

  inline QString getProjectPath()
  {
    return mProjectPath;
  }

  inline void setProjectPath(QString path)
  {
    this->mProjectPath = path;
  }

private:
  QString mProjectPath;
  QList<ChartData> convertSheetsToChart(QJsonValue sheets);
  QJsonValue convertSheetsToJson(QList<ChartData>);
};

#endif // IOPARSER_H
