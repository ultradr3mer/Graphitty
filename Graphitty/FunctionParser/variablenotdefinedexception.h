#ifndef VARIABLENOTDEFINEDEXCEPTION_H
#define VARIABLENOTDEFINEDEXCEPTION_H

#include "FunctionParserException.h"

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
    message = string("The variable '") + var + string("' is not defined.");
  }
};

#endif // VARIABLENOTDEFINEDEXCEPTION_H
