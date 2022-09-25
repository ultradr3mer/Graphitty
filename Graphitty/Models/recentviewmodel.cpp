/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#include "recentviewmodel.h"

QStringList testData = {"Letter", "Name", "Definition", "Show"};

RecentViewModel::RecentViewModel(QObject* parent) : QStringListModel(parent)
{
}

void RecentViewModel::setRecentFiles(QStringList projects)
{
  this->setStringList(projects);
}
