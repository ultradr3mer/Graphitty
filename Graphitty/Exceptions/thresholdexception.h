/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef THRESHOLDEXCEPTION_H
#define THRESHOLDEXCEPTION_H

#include <QString>
#include <string>
using namespace std;

class ThresholdException : public exception
{
protected:
  QString message;

public:
  inline ThresholdException()
  {
    message = "Error while parsing the Threshold.";
  }
  inline ThresholdException(QString msg)
  {
    message = msg;
  }
  inline QString getMessage()
  {
    return message;
  }
};

#endif // THRESHOLDEXCEPTION_H
