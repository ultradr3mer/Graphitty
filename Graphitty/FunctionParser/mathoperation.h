#ifndef MATHOPERATION_H
#define MATHOPERATION_H

#include <QtMath>
#include <string>

#include <Exceptions/mathoperationexception.h>
using namespace std;

class MathOperation
{
private:
  char opSymbol;

public:
  inline MathOperation()
  {
    this->opSymbol = '?';
  }

  inline MathOperation(char opSymbol)
  {
    this->opSymbol = opSymbol;
  }

  inline string toString()
  {
    return string(1, opSymbol);
  }

  inline double calculate(double operand1, double operand2)
  {
    double result;
    switch (opSymbol)
    {
    case '+':
      result = operand1 + operand2;
      break;
    case '-':
      result = operand1 - operand2;
      break;
    case '*':
      result = operand1 * operand2;
      break;
    case '/':
      result = operand1 / operand2;
      break;
    case '^':
      result = qPow(operand1, operand2);
      break;
    default:
      throw MathOperationException(opSymbol);
      result = 0.0;
      break;
    }
    return result;
  }
};

#endif // MATHOPERATION_H
