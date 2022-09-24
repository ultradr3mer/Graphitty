#include "thresholdstablemodel.h"

QStringList thresholdsHeader = {"Name", "Letter", "Threshold", "Show"};

ThresholdsTableModel::ThresholdsTableModel(QObject* parent) : QAbstractTableModel(parent)
{
}

int ThresholdsTableModel::rowCount(const QModelIndex& parent) const
{
  return this->entries->count() + 1;
}

int ThresholdsTableModel::columnCount(const QModelIndex& parent) const
{
  return 4;
}

QVariant ThresholdsTableModel::data(const QModelIndex& index, int role) const
{
  ThresholdData data;
  if (index.row() < this->entries->count())
  {
    data = this->entries->at(index.row());
  }
  else
  {
    data = ThresholdData("", "", 0.0, false);
  }

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
  ThresholdData data;
  if (index.row() < this->entries->count())
  {
    data = this->entries->at(index.row());
  }
  else
  {
    data = ThresholdData("", "", 0.0, false);
  }

  bool isEdit = false;
  switch (role)
  {
  case Qt::EditRole:
    if (index.column() == 0)
    {
      data.setName(value.toString());
      isEdit = true;
    }
    else if (index.column() == 1)
    {
      data.setLetter(value.toString());
      isEdit = true;
    }
    else if (index.column() == 2)
    {
      data.setThreshold(value.toDouble());
      isEdit = true;
    }

  case Qt::CheckStateRole:
    if (index.column() == 3)
    {
      bool checked = value == Qt::Checked;
      data.setIsShown(checked);
      isEdit = true;
    }
  }

  if (isEdit)
  {
    if (index.row() < this->entries->count())
    {
      if (!data.isEmpty())
      {
        this->entries->replace(index.row(), data);
      }
      else
      {
        beginRemoveRows(QModelIndex(), index.row(), index.row());
        this->entries->remove(index.row());
        endRemoveRows();
      }
    }
    else
    {
      if (!data.isEmpty())
      {
        beginInsertRows(QModelIndex(), this->entries->size() + 1, this->entries->size() + 1);
        this->entries->append(data);
        endInsertRows();
      }
    }

    return true;
  }
  return false;
}

void ThresholdsTableModel::setThresholdData(QList<ThresholdData>* value)
{
  this->entries = value;
}
