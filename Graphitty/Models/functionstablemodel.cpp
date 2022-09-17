#include "functionstablemodel.h"

#include <QBrush>
#include <QFont>

QStringList header = {"Letter", "Name", "Definition", "Show"};

MyModel::MyModel(QObject* parent) : QAbstractTableModel(parent)
{
}

int MyModel::rowCount(const QModelIndex& /*parent*/) const
{
  return 2;
}

int MyModel::columnCount(const QModelIndex& /*parent*/) const
{
  return 4;
}

Qt::ItemFlags MyModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags baseFlags = QAbstractTableModel::flags(index);
  if (index.column() != 3)
  {
    return baseFlags | Qt::ItemIsEditable;
  }
  return baseFlags | Qt::ItemIsEnabled;
}

QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
  {
    return QVariant();
  }

  if (orientation == Qt::Orientation::Vertical)
  {
    return QVariant(QString::number(section + 1));
  }

  return QVariant(header[section]);
}

QVariant MyModel::data(const QModelIndex& index, int role) const
{
  if (index.column() == 3 && role == Qt::CheckStateRole)
  {
    bool b = true;
    Qt::CheckState cs = b ? Qt::Checked : Qt::Unchecked;
    return QVariant(static_cast<int>(cs));
  }

  if (index.column() != 3 && role == Qt::DisplayRole)
  {
    return QVariant("a");
  }

  return QVariant();
}
