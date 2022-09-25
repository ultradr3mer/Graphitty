/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef FUNCTIONSTABLEMODEL_H
#define FUNCTIONSTABLEMODEL_H

#include <QAbstractTableModel>

#include <Data/functiondata.h>

class FunctionsTableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit FunctionsTableModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;
  void setFunctionData(QList<FunctionData>* value);

private:
  QList<FunctionData>* entries;
};

#endif
