/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#include "functionstablemodel.h"

#include <QBrush>
#include <QFont>

QStringList functionsHeader = {"Letter", "Name", "Definition", "Show"};

FunctionsTableModel::FunctionsTableModel(QObject* parent) : QAbstractTableModel(parent)
{
}

int FunctionsTableModel::rowCount(const QModelIndex& /*parent*/) const
{
  return this->entries->count() + 1;
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

  return QVariant(functionsHeader[section]);
}

bool FunctionsTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  FunctionData data;
  if (index.row() < this->entries->count())
  {
    data = this->entries->at(index.row());
  }
  else
  {
    data = FunctionData("", "", "", false);
  }

  bool edit = false;
  switch (role)
  {
  case Qt::EditRole:
    if (index.column() == 0)
    {
      data.setLetter(value.toString());
      edit = true;
    }
    else if (index.column() == 1)
    {
      data.setName(value.toString());
      edit = true;
    }
    else if (index.column() == 2)
    {
      data.setDefinition(value.toString());
      edit = true;
    }

  case Qt::CheckStateRole:
    if (index.column() == 3)
    {
      bool checked = value == Qt::Checked;
      data.setIsShown(checked);
      edit = true;
    }
  }

  if (edit)
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

void FunctionsTableModel::setFunctionData(QList<FunctionData>* value)
{
  this->entries = value;
}

QVariant FunctionsTableModel::data(const QModelIndex& index, int role) const
{
  FunctionData data;
  if (index.row() < this->entries->count())
  {
    data = this->entries->at(index.row());
  }
  else
  {
    data = FunctionData("", "", "", false);
  }

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
