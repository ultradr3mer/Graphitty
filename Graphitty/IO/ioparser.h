#ifndef IOPARSER_H
#define IOPARSER_H

#include <QChart>
#include <QJsonArray>
#include <QList>
#include <Data/functiondata.h>
#include <viewarea.h>
#include <Data/chartdata.h>



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
