#include "thresholdstablemodel.h"

QStringList thresholdsHeader = {"Name", "Letter", "Threshold", "Show"};

ThresholdsTableModel::ThresholdsTableModel(QObject* parent) : QAbstractTableModel(parent)
{
}

int ThresholdsTableModel::rowCount(const QModelIndex& parent) const
{
  return this->entries->count();
}

int ThresholdsTableModel::columnCount(const QModelIndex& parent) const
{
  return 4;
}

QVariant ThresholdsTableModel::data(const QModelIndex& index, int role) const
{
  ThresholdData data = this->entries->at(index.row());
  switch (role)
  {
  case Qt::DisplayRole:
  case Qt::EditRole:

    if (index.column() == 0)
      return QVariant(*data.getName());
    if (index.column() == 1)
      return QVariant(*data.getLetter());
    if (index.column() == 2)
      return QVariant(data.getThreshold());

    return QVariant("");

  case Qt::CheckStateRole:

    if (index.column() == 3)
      return data.getIsShown() ? Qt::Checked : Qt::Unchecked;

    break;
  }
  return QVariant();
}

Qt::ItemFlags ThresholdsTableModel::flags(const QModelIndex& index) const
{
  if (index.column() == 3)
    return Qt::ItemIsUserCheckable | QAbstractTableModel::flags(index);
  return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

QVariant ThresholdsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
  {
    return QVariant();
  }

  if (orientation == Qt::Orientation::Vertical)
  {
    return QVariant(QString::number(section + 1));
  }

  return QVariant(thresholdsHeader[section]);
}

bool ThresholdsTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  return false;
}

void ThresholdsTableModel::setThresholdData(QList<ThresholdData>* value)
{
  this->entries = value;
}
