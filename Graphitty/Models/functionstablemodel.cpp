#include "functionstablemodel.h"

#include <QBrush>
#include <QFont>

QStringList header = {"Letter", "Name", "Definition", "Show"};

FunctionsTableModel::FunctionsTableModel(QObject* parent) : QAbstractTableModel(parent)
{
}

int FunctionsTableModel::rowCount(const QModelIndex& /*parent*/) const
{
  return 2;
}

int FunctionsTableModel::columnCount(const QModelIndex& /*parent*/) const
{
  return 4;
}

Qt::ItemFlags FunctionsTableModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags baseFlags = QAbstractTableModel::flags(index);
  if (index.column() != 3)
  {
    return baseFlags | Qt::ItemIsEditable;
  }
  return baseFlags | Qt::ItemIsEnabled;
}

QVariant FunctionsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant FunctionsTableModel::data(const QModelIndex& index, int role) const
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
