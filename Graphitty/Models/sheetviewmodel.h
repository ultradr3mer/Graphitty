/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef SHEETVIEWMODEL_H
#define SHEETVIEWMODEL_H

#include <Models/mainviewmodel.h>
#include <QStringListModel>

class SheetViewModel : public QStringListModel
{
  Q_OBJECT
public:
  explicit SheetViewModel(QObject* parent = nullptr);
  void setSheetViews(MainViewModel model);

private:
  QStringList* views;
};

#endif
