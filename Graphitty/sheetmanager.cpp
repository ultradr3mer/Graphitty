#include "sheetmanager.h"

#include <QString>
#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>


// creates new sheet manager for new projects
SheetManager::SheetManager()
{}

// creates sheet manager from existing project path
SheetManager::SheetManager(const QString& filePath)
{
    readProject(filePath);
    projectPath = filePath;
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

    qWarning() << val;

    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject values = d.object();

    QJsonArray globals = values["global"].toArray();
    sheetGlobal = globals;

    QJsonArray sheets = values["sheets"].toArray();
    sheetData = sheets;

//    foreach (const QJsonValue & value, sheets) {
//        QJsonObject sheet = value.toObject();
//        //
//    }
}

// saves project to member path
void SheetManager::saveProject()
{
    // requires previous checkExistingProject() check
    // otherwise call saveNewProject()
    saveProjectToFile(projectPath);
}

// saves project to new (provided) path
void SheetManager::saveNewProject(const QString& filePath)
{
    // requires previous QFileDialog to select new save file
    saveProjectToFile(filePath);
}

// final save action to translate to external file
void SheetManager::saveProjectToFile(const QString& filePath)
{
    QFile file(filePath);
    QJsonDocument* document = new QJsonDocument(QJsonArray());

    auto objectData = QJsonObject(
    {
        qMakePair(QString("global"), sheetGlobal),
        qMakePair(QString("sheets"), sheetData)
    });

    document->array().append(objectData);

    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(document->toJson());
    file.close();
}

// saves individual sheet to temporary project
void SheetManager::saveSheet(const QJsonArray& sheet)
{
    sheetData[sheetIndex] = sheet;
}

// loads sheet from project by index
QJsonArray SheetManager::loadSheet(int index)
{
    sheetIndex = index;
    return sheetData[index].toArray();
}

// checks if project path exists (if project already has a file path || was saved before)
bool SheetManager::checkExistingProject()
{
    if(projectPath.isEmpty() || projectPath.isNull()) {
        return false;
    } else {
        return true;
    }
}

