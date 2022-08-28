#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H

#include "mathoperation.h"
#include <map>
#include <string>

using namespace std;

class FunctionNode
{
private:
  // needed for building the tree
  string expr;

  // Needed when this is a math operation
  FunctionNode* operand1;
  MathOperation operation;
  FunctionNode* operand2;
  bool inBrackets;

  // Needed when this is a variable
  string varName;

  // Needed when this is a constant value
  double value;

  void removeSpaces();
  void removeOuterBrackets();
  bool hasExcessveBrackets();
  bool doNextOperation();
  bool isImplicitMultiply(char c1, char c2);

public:
  FunctionNode();
  FunctionNode(const FunctionNode& obj);
  FunctionNode(string expression);
  ~FunctionNode();
  FunctionNode& operator=(FunctionNode obj);
  double getResult(double r);
  double getResult(map<string, double>);
  string toStrnig();
};

#endif // FUNCTIONNODE_H
