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
