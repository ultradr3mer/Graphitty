/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef THRESHOLDDATA_H
#define THRESHOLDDATA_H

#include <QString>

class ThresholdData
{
public:
  ThresholdData(const QString& name, const QString& letter, const double value, bool isShown)
      : mName(name), mLetter(letter), mThreshold(value), mIsShown(isShown)
  {
  }

  ThresholdData(const ThresholdData& old_obj)
  {
    mName = old_obj.mName;
    mLetter = old_obj.mLetter;
    mThreshold = old_obj.mThreshold;
    mIsShown = old_obj.mIsShown;
  }

  ThresholdData()
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

  inline double getThreshold()
  {
    return mThreshold;
  }
  inline void setThreshold(double value)
  {
    mThreshold = value;
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
    return this->mName.isEmpty() && this->mLetter.isEmpty() && !this->mIsShown;
  }

private:
  QString mName;
  QString mLetter;
  double mThreshold;
  bool mIsShown;
};

#endif // THRESHOLDDATA_H
