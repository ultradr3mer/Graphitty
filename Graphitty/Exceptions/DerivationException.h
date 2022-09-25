/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef DERIVATIONEXCEPTION_H
#define DERIVATIONEXCEPTION_H

#include <QString>
#include <string>
using namespace std;

class DerivationException : public exception
{
protected:
  QString message;

public:
  inline DerivationException()
  {
    message = "Error while parsing the Derivation.";
  }
  inline DerivationException(QString msg)
  {
    message = msg;
  }
  inline QString getMessage()
  {
    return message;
  }
};

#endif // DERIVATIONEXCEPTION_H
