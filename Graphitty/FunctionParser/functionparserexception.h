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
    message = "Error while parsnig the Expression.";
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
