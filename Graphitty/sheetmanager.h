#ifndef SHEETMANAGER_H
#define SHEETMANAGER_H

#include <QChart>
#include <QJsonArray>
#include <QList>
#include <Data/functiondata.h>
#include <viewarea.h>
#include <Data/chartdata.h>



class SheetManager
{

public:
    SheetManager();
    ~SheetManager();
    QList<ChartData> readProject(const QString& filePath);
    void saveProjectToFile(QList<ChartData> charts, const QString& filePath);
    bool checkForExistingProject();

    inline QString GetProjectPath()
    {
        return mProjectPath;
    }

private:
    QString mProjectPath;
    QList<ChartData> convertSheetsToChart(QJsonValue sheets);
    QJsonValue convertSheetsToJson(QList<ChartData>);
};

#endif // SHEETMANAGER_H
