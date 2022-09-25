#include "recentviewmodel.h"

QStringList testData = {"Letter", "Name", "Definition", "Show"};

RecentViewModel::RecentViewModel(QObject* parent) : QStringListModel(parent)
{}

void RecentViewModel::setRecentFiles(QStringList projects)
{
    this->setStringList(projects);
}


