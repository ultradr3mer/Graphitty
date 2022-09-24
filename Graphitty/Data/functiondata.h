#ifndef FUNCTIONDATA_H
#define FUNCTIONDATA_H

#include <QString>

class FunctionData
{
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

  FunctionData()
  {
  }

  inline QString* getLetter()
  {
    return &mLetter;
  }
  inline void setLetter(const QString& value)
  {
    mLetter = value;
  }

  inline QString* getName()
  {
    return &mName;
  }
  inline void setName(const QString& value)
  {
    mName = value;
  }

  inline QString* getDefinition()
  {
    return &mDefinition;
  }
  inline void setDefinition(const QString& value)
  {
    mDefinition = value;
  }

  inline bool getIsShown()
  {
    return this->mIsShown;
  }
  inline void setIsShown(bool value)
  {
    this->mIsShown = value;
  }

  inline bool isEmpty()
  {
    return this->mName.isEmpty() && this->mLetter.isEmpty() && this->mDefinition.isEmpty() &&
           !this->mIsShown;
  }

private:
  QString mLetter;
  QString mName;
  QString mDefinition;
  bool mIsShown;
};

#endif // FUNCTIONDATA_H
