/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef THRESHOLDSTABLEMODEL_H
#define THRESHOLDSTABLEMODEL_H

#include <QAbstractTableModel>

#include <Data/thresholddata.h>

class ThresholdsTableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit ThresholdsTableModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;
  void setThresholdData(QList<ThresholdData>* value);

private:
  QList<ThresholdData>* entries;
};

#endif // THRESHOLDSTABLEMODEL_H
