#ifndef SHEETMANAGER_H
#define SHEETMANAGER_H

#include <QChart>
#include <QJsonArray>

namespace std
{
    class SheetManager;
}

class SheetManager
{

public:
    SheetManager();
    SheetManager(const QString& filePath);
    ~SheetManager();
    void readProject(const QString& filePath);
    void saveProject();
    void saveNewProject(const QString& filePath);
    void saveProjectToFile(const QString& filePath);
    void saveSheet(const QJsonArray& sheet);
    QJsonArray loadSheet(int sheetIndex);
    bool checkExistingProject();

private:
    int sheetIndex = 0;
    QString projectPath;
    QJsonArray sheetGlobal;
    QJsonArray sheetData;
};

#endif // SHEETMANAGER_H
