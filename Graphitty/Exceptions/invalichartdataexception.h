/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef INVALICHARTDATAEXCEPTION_H
#define INVALICHARTDATAEXCEPTION_H

#include <QString>
#include <string>
using namespace std;

class InvalidChartDataException : public exception
{
protected:
  QString message;

public:
  inline InvalidChartDataException()
  {
    message = "Error while importing project file. Invalid project data.";
  }
  inline InvalidChartDataException(QString msg)
  {
    message = msg;
  }
  inline QString getMessage()
  {
    return message;
  }
};

#endif // INVALICHARTDATAEXCEPTION_H
