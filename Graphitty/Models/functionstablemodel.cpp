#include "functionstablemodel.h"

#include <QBrush>
#include <QFont>

QStringList header = {"Letter", "Name", "Definition", "Show"};

FunctionsTableModel::FunctionsTableModel(QObject* parent) : QAbstractTableModel(parent)
{
}

int FunctionsTableModel::rowCount(const QModelIndex& /*parent*/) const
{
  return this->entries->count();
}

int FunctionsTableModel::columnCount(const QModelIndex& /*parent*/) const
{
  return 4;
}

Qt::ItemFlags FunctionsTableModel::flags(const QModelIndex& index) const
{
  if (index.column() == 3)
    return Qt::ItemIsUserCheckable | QAbstractTableModel::flags(index);
  return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
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

bool FunctionsTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  switch (role)
  {
  case Qt::EditRole:
    if (index.column() == 0)
    {
      auto data = this->entries->at(index.row());
      data.setLetter(value.toString());
      this->entries->replace(index.row(), data);
      return true;
    }
    else if (index.column() == 1)
    {
      auto data = this->entries->at(index.row());
      data.setName(value.toString());
      this->entries->replace(index.row(), data);
      return true;
    }
    else if (index.column() == 2)
    {
      auto data = this->entries->at(index.row());
      data.setDefinition(value.toString());
      this->entries->replace(index.row(), data);
      return true;
    }

  case Qt::CheckStateRole:
    if (index.column() == 3)
    {
      auto data = this->entries->at(index.row());
      bool checked = value == Qt::Checked;
      data.setIsShown(checked);
      this->entries->replace(index.row(), data);
      return true;
    }
  }
  return false;
}

void FunctionsTableModel::SetFunctionData(QList<FunctionData>* value)
{
  this->entries = value;
}

QVariant FunctionsTableModel::data(const QModelIndex& index, int role) const
{
  FunctionData data = this->entries->at(index.row());
  switch (role)
  {
  case Qt::DisplayRole:
  case Qt::EditRole:

    if (index.column() == 0)
      return QVariant(*data.getLetter());
    if (index.column() == 1)
      return QVariant(*data.getName());
    if (index.column() == 2)
      return QVariant(*data.getDefinition());

    return QVariant("");

  case Qt::CheckStateRole:

    if (index.column() == 3)
      return data.getIsShown() ? Qt::Checked : Qt::Unchecked;

    break;
  }
  return QVariant();
}
