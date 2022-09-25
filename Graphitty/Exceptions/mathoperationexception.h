/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef MATHOPERATIONEXCEPTION_H
#define MATHOPERATIONEXCEPTION_H

#include "functionparserexception.h"

class MathOperationException : public FunctionParserException
{
private:
  string message;

public:
  inline MathOperationException()
  {
    message = "Error while using a math operation";
  }
  inline MathOperationException(string msg)
  {
    message = msg;
  }
  inline MathOperationException(char c)
  {
    message = string("Error while using math operation ") + c +
              string("\nIs this maybe the wrong operation?");
  }
};

#endif // MATHOPERATIONEXCEPTION_H
