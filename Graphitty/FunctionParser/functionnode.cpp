/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#include "functionnode.h"

#include <QString>
#include <algorithm>

#include <Exceptions/variablenotdefinedexception.h>
using namespace std;

FunctionNode::FunctionNode()
    : operand1(NULL), operand2(NULL), inBrackets(false), varName(""), value(0.0)
{
}

FunctionNode::FunctionNode(const FunctionNode& obj)
    : expr(""), operand1(NULL), operation(obj.operation), operand2(NULL),
      inBrackets(obj.inBrackets), varName(obj.varName), value(obj.value)
{
  if (obj.operand1 && obj.operand2)
  {
    operand1 = new FunctionNode(*obj.operand1);
    operand2 = new FunctionNode(*obj.operand2);
  }
}

FunctionNode::FunctionNode(string expression)
    : expr(expression), operand1(NULL), operand2(NULL), inBrackets(false), varName(""), value(0.0)
{
  removeSpaces();

  removeOuterBrackets();

  // do the next operation, if it exists
  if (!doNextOperation())
  {
    auto qStr = QString::fromStdString(expr);
    // if false is returned, this is not a math operation, so it's a constant or
    // variable.
    bool ok;
    auto tmp = qStr.toDouble(&ok);
    if (!ok)
    {
      // Variable
      varName = expr;
    }
    else
    {
      // Constant Value
      value = tmp;
    }
  }
}

FunctionNode::~FunctionNode()
{
  if (operand1)
  {
    delete operand1;
  }
  if (operand2)
  {
    delete operand2;
  }
}

FunctionNode& FunctionNode::operator=(FunctionNode obj)
{
  if (&obj != this)
  {
    if (operand1)
    {
      delete operand1;
    }
    if (operand2)
    {
      delete operand2;
    }

    operation = obj.operation;
    inBrackets = obj.inBrackets;
    varName = obj.varName;
    value = obj.value;

    if (obj.operand1 && obj.operand2)
    {
      operand1 = new FunctionNode(*obj.operand1);
      operand2 = new FunctionNode(*obj.operand2);
    }
  }
  return *this;
}

double FunctionNode::getResult(double r)
{
  map<string, double> var;
  var.insert_or_assign("r", r);
  return getResult(var);
}

double FunctionNode::getResult(map<string, double> vars)
{
  double result;

  if (operand1 && operand2)
  {
    // Math Operation
    result = operation.calculate(operand1->getResult(vars), operand2->getResult(vars));
  }
  else if (!varName.empty())
  {
    // Variable
    // try:
    auto it = vars.find(varName);
    if (it != vars.end())
    {
      result = it->second;
    }
    else
    {
      // catch:
      // var doesn't exist
      throw VariableNotDefinedException(varName);
    }
  }
  else
  {
    // Constant
    result = value;
  }

  return result;
}

string FunctionNode::toStrnig()
{
  string result;

  if (operand1 && operand2)
  {
    // Math Operation
    result = operand1->toStrnig() + operation.toString() + operand2->toStrnig();
    if (inBrackets)
    {
      result = "(" + result + ")";
    }
  }
  else if (!varName.empty())
  {
    // Variable
    result = varName;
  }
  else
  {
    // Constant
    result = to_string(value);
    result.erase(result.find_last_not_of("0") + 1, string::npos);
    result.erase(result.find_last_not_of(".") + 1, string::npos);
  }

  return result;
}

void FunctionNode::removeSpaces()
{
  string str = "";
  for (int ch : expr)
  {
    if (!isspace(ch))
    {
      str.append(string(1, ch));
    }
  }
  expr = str;
}

void FunctionNode::removeOuterBrackets()
{
  while (hasExcessveBrackets())
  {
    expr.pop_back();   // delete last bracket
    expr.erase(0, 1);  // delete first bracket
    inBrackets = true; // needed in the toString method
  }
}

bool FunctionNode::hasExcessveBrackets()
{
  int bracketDepth = 0;
  bool excessiveBrackets = true;
  if (expr.length() < 2)
    excessiveBrackets = false;
  else if (expr[0] != '(')
  {
    excessiveBrackets = false;
  }
  for (unsigned int i = 0; i < expr.length(); ++i)
  {
    // count up if a new bracket was opened
    if (expr[i] == '(')
    {
      bracketDepth++;
    }
    // count down if the last bracket was closed
    if (expr[i] == ')')
    {
      bracketDepth--;
    }
    // throw an exception, if more brackets were closed than opened.
    if (bracketDepth < 0)
    {
      // FEHLERHAFTE FORMEL!
      throw FunctionParserException("Number of '(' doesn't match ')'! Maybe too many ')'?");
      // raise or throw??
    }
    // if we have a part where the bracketing level is 0, we can't remove the
    // outermost bracket.
    if (bracketDepth == 0)
    {
      if (i < expr.length() - 1)
      {
        excessiveBrackets = false;
      }
    }
  }
  // opening and closing bracket count must match.
  if (bracketDepth != 0)
  {
    throw FunctionParserException("Number of '(' doesn't match ')'!");
  }

  return excessiveBrackets;
}

bool FunctionNode::doNextOperation()
{
  // suchen nach dem Rechenzeichen,dass zuletzt ausgeführt werden muss.
  // Strich vor Punkt vor Exponent. Bei Strich und Punkt muss von rechts nach
  // links gesucht werden, bei Exponent von links nach rechts. a-b-c = (a-b)-c #
  // Hier wird das letzt Minus am Schluss berechnete. a/b/c = (a/b)/c # Hier
  // wird das letzt Geteilt am Schluss berechnet. a^b^c = a^(b^c) # Hier wird
  // der ERSTE Exponent zum schluss berechnet.

  int opCharPos = -1;
  bool operationDone = false;
  bool implicitMultiply = false; // wenn implizit multipliziert wird, z.B. 5(1+2)
  int bracketLevel;

  // Suche letztes + oder -
  bracketLevel = 0;
  for (int i = expr.length() - 1; !operationDone && i >= 0; --i)
  {
    if (expr[i] == ')')
    {
      bracketLevel++;
    }
    if (expr[i] == '(')
    {
      bracketLevel--;
    }
    if (bracketLevel == 0 && (expr[i] == '+' || expr[i] == '-'))
    {
      operationDone = true;
      opCharPos = i;
    }
  }

  // Wenn noch nichts gefunden, suche letztes * oder / oder letztes impliziertes
  // *
  bracketLevel = 0;
  for (int i = expr.length() - 1; !operationDone && i >= 0; --i)
  {
    if (expr[i] == ')')
    {
      bracketLevel++;
    }
    if (expr[i] == '(')
    {
      bracketLevel--;
    }
    if (bracketLevel == 0)
    {
      if (expr[i] == '*' || expr[i] == '/')
      {
        operationDone = true;
        opCharPos = i;
      }
      if (i > 0)
        if (isImplicitMultiply(expr[i - 1], expr[i]))
        {
          operationDone = true;
          implicitMultiply = true;
          opCharPos = i;
        }
    }
  }
  // Wenn immer noch nichts gefunden, suche ERSTES ^
  bracketLevel = 0;
  for (unsigned int i = 0; !operationDone && i < expr.length(); ++i)
  {
    if (expr[i] == '(')
    {
      bracketLevel++;
    }
    if (expr[i] == ')')
    {
      bracketLevel--;
    }
    if (bracketLevel == 0 && (expr[i] == '^'))
    {
      operationDone = true;
      opCharPos = i;
    }
  }

  // Wenn gefunden, speichere Rechenzeichen und erstelle 2 weitere Teilbäume.
  if (operationDone)
  {
    if (implicitMultiply)
    { // Eine Implizierte Multiplikation
      // speichere Rechenzeichen
      operation = '*';
      // Teile string und mache neue Objekte
      operand1 = new FunctionNode(expr.substr(0, opCharPos));
      operand2 = new FunctionNode(expr.substr(opCharPos, expr.length() - opCharPos));
    }
    else
    { // eine Operation mit rechenzeichen
      // speichere Rechenzeichen
      operation = expr[opCharPos];
      // Teile string und mache neue Objekte
      operand1 = new FunctionNode(expr.substr(0, opCharPos));
      operand2 = new FunctionNode(expr.substr(opCharPos + 1, expr.length() - opCharPos - 1));
    }
  }
  return operationDone;
}

bool FunctionNode::isImplicitMultiply(char c1, char c2)
{
  // )1  1(
  // )r  r(
  // )(  1r
  // r1 nur wenn var namen keine zahlen enthalten dürfen

  return (c1 == ')' && (isdigit(c2) || isalpha(c2) || c2 == '(')) ||
         (c2 == '(' && (isdigit(c1) || isalpha(c1))) || (isdigit(c1) && isalpha(c2));
}
