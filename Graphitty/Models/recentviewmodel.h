/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef RECENTVIEWMODEL_H
#define RECENTVIEWMODEL_H

#include <QStringListModel>

class RecentViewModel : public QStringListModel
{
  Q_OBJECT
public:
  explicit RecentViewModel(QObject* parent = nullptr);
  void setRecentFiles(QStringList projects);

private:
  QStringList* files;
};

#endif
