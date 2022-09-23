#include "sheetmanager.h"

#include <QString>
#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>

#include <Data/functiondata.h>
#include <viewarea.h>
#include <Data/chartdata.h>

// creates new sheet manager for projects
SheetManager::SheetManager()
{}

SheetManager::~SheetManager()
{}

// opens given file and saves filepath to manager
void SheetManager::openProject(const QString& filePath)
{
    readProject(filePath);
    this->mProjectPath = filePath;
}

// reads project files into members
void SheetManager::readProject(const QString& filePath)
{
    QString val;
    QFile file;

    file.setFileName(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject values = doc.object();

    QJsonValue globals = values.value(QString("global"));
    this->mSheetGlobal = globals;

    QJsonValue sheets = values.value(QString("sheets"));
    this->mSheetData = sheets;
}

// final save action to translate to external file
void SheetManager::saveProjectToFile(ChartData* chart, const QString& filePath)
{
    this->saveSheet(chart);

    QFile file(filePath);
    QJsonDocument* document = new QJsonDocument(QJsonArray());

    auto objectData = QJsonObject(
    {
        qMakePair(QString("global"), this->mSheetGlobal),
        qMakePair(QString("sheets"), this->mSheetData),
    });

    document->setObject(objectData);

    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(document->toJson());
    file.close();

    this->mProjectPath = filePath;
}

// saves current sheet to manager
void SheetManager::saveSheet(ChartData* chart)
{
    auto viewArea = chart->GetViewArea();
    auto functionData = chart->GetFunctionData();
    auto functions = QJsonArray();

    for (auto& singleEnty : functionData)
    {
        auto function = QJsonObject(
            {
                qMakePair(QString("letter"), *singleEnty.getLetter()),
                qMakePair(QString("name"), *singleEnty.getName()),
                qMakePair(QString("definition"), *singleEnty.getDefinition()),
                qMakePair(QString("shown"), singleEnty.getIsShown())
             });

        functions.append(function);
    }

    auto view = QJsonObject(
        {
            qMakePair(QString("fromX"), viewArea->getFromX()),
            qMakePair(QString("toX"), viewArea->getToX()),
            qMakePair(QString("fromY"), viewArea->getFromY()),
            qMakePair(QString("toY"), viewArea->getToY())
        });

    auto sheet = QJsonObject(
        {
            qMakePair(QString("function"), functions),
            qMakePair(QString("view"), view),
            // Sheet name not changeable / defined yet
            qMakePair(QString("name"), "sheetNew"),
            qMakePair(QString("inverted"), chart->GetIsChartInverted())
        });

    QJsonArray sheets = this->mSheetData.toArray();
    sheets[this->mSheetIndex] = sheet;
    this->mSheetData = QJsonValue(sheets);

}

// loads sheet from project by index
ChartData SheetManager::loadSheet()
{
    ChartData result;
    auto sheets = this->mSheetData.toArray();
    auto sheet = sheets[this->mSheetIndex].toObject();
    auto view = sheet["view"].toObject();
    auto functions = sheet["function"].toArray();

    result.SetIsChartInverted(sheet["inverted"].toBool());

    ViewArea va(view["fromX"].toDouble(), view["toX"].toDouble(), view["fromY"].toDouble(), view["toY"].toDouble());
    result.SetViewArea(va);

    auto functionData = QList<FunctionData>();

    foreach (const QJsonValue & value, functions) {
        auto function = value.toObject();
        FunctionData fc(
            function["letter"].toString().toStdString().c_str(),
            function["name"].toString().toStdString().c_str(),
            function["definition"].toString().toStdString().c_str(),
            function["shown"].toBool()
            );

        functionData.append(fc);
    }

    result.SetFunctionData(functionData);

    return result;
}

// checks if project path exists (if project already has a file path || was saved before)
bool SheetManager::checkForExistingProject()
{
    if(this->mProjectPath.isEmpty() || this->mProjectPath.isNull()) {
        return false;
    } else {
        return true;
    }
}

QJsonArray SheetManager::getSheetTree()
{
    QJsonArray tree;
    auto sheets = this->mSheetData.toArray();
    int index = 0;

    foreach (const QJsonValue & value, sheets) {
        auto sheet = value.toObject();
        auto branch = QJsonObject(
            {
                qMakePair(QString("name"), sheet["name"].toString().toStdString().c_str()),
                qMakePair(QString("index"), index),
                });
        tree.append(branch);
        index++;
    }

    return tree;
}

