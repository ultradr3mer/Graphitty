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
    void openProject(const QString& filePath);
    void readProject(const QString& filePath);
    void saveProjectToFile(ChartData* chart, const QString& filePath);
    void saveSheet(ChartData* chart);
    ChartData loadSheet();
    bool checkForExistingProject();
    QJsonArray getSheetTree();


    inline QString GetProjectPath()
    {
        return mProjectPath;
    }

private:
    QString mProjectPath;
    QJsonValue mSheetGlobal;
    QJsonValue mSheetData;
    int mSheetIndex = 0;
};

#endif // SHEETMANAGER_H
