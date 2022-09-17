#ifndef FUNCTIONDATA_H
#define FUNCTIONDATA_H

#include <QString>

class FunctionData
{
private:
  QString mLetter;
  QString mName;
  QString mDefinition;
  bool mIsShown;

public:
  FunctionData(const QString& letter, const QString& name, const QString& definition, bool isShown)
      : mLetter(letter), mName(name), mDefinition(definition), mIsShown(isShown)
  {
  }

  FunctionData(const FunctionData& old_obj)
  {
    this->mLetter = old_obj.mLetter;
    this->mName = old_obj.mName;
    this->mDefinition = old_obj.mDefinition;
    this->mIsShown = old_obj.mIsShown;
  }

  FunctionData();

  inline QString* GetLetter()
  {
    return &mLetter;
  }
  inline void SetLetter(const QString& value)
  {
    mLetter = value;
  }

  inline QString* GetName()
  {
    return &mName;
  }
  inline void SetName(const QString& value)
  {
    mName = value;
  }

  inline QString* GetDefinition()
  {
    return &mDefinition;
  }
  inline void SetDefinition(const QString& value)
  {
    mDefinition = value;
  }

  inline bool GetIsShown()
  {
    return this->mIsShown;
  }
  inline void SetIsShown(bool value)
  {
    this->mIsShown = value;
  }
};

#endif // FUNCTIONDATA_H
