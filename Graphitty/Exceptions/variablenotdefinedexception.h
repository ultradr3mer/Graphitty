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
