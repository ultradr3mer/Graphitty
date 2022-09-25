/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef FUNCTIONPARSEREXCEPTION_H
#define FUNCTIONPARSEREXCEPTION_H

#include <string>
using namespace std;

class FunctionParserException : public exception
{
protected:
  string message;

public:
  inline FunctionParserException()
  {
    message = "Error while parsing the Expression.";
  }
  inline FunctionParserException(string msg)
  {
    message = msg;
  }
  inline string getMessage()
  {
    return message;
  }
};

#endif // FUNCTIONPARSEREXCEPTION_H
