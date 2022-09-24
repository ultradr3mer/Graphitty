#ifndef THRESHOLDEXCEPTION_H
#define THRESHOLDEXCEPTION_H

#include <QString>
#include <string>
using namespace std;

class ThresholdException : public exception
{
protected:
  QString message;

public:
  inline ThresholdException()
  {
    message = "Error while parsing the Threshold.";
  }
  inline ThresholdException(QString msg)
  {
    message = msg;
  }
  inline QString getMessage()
  {
    return message;
  }
};

#endif // THRESHOLDEXCEPTION_H
