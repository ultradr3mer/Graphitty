/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef VARIABLENOTDEFINEDEXCEPTION_H
#define VARIABLENOTDEFINEDEXCEPTION_H

#include "functionparserexception.h"

class VariableNotDefinedException : public FunctionParserException
{
  // private:
  // string message;
public:
  inline VariableNotDefinedException()
  {
    message = "A variable used int this FunctionNode was not defined in the hashmap.";
  }
  inline VariableNotDefinedException(string var)
  {
    message = string("The variable '") + var + string("' is not a defined variable.");
  }
};

#endif // VARIABLENOTDEFINEDEXCEPTION_H
