#include "polynomialfinder.h"
#include "Math/lineqsolver.h"
#include "mainview.h"

std::vector<double> findPolynomial(std::vector<std::array<double, 2>> points)
{
  int degree = points.size();
  std::vector<std::vector<double>> les;
  for (auto point : points)
  {
    std::vector<double> line;

    for (int i = degree - 1; i >= 0; i--)
    {
      line.push_back(pow(point[0], i));
    }

    line.push_back(point[1]);

    les.push_back(std::move(line));
  }

  return solveLes(std::move(les));
}

double calculatePolynomial(std::vector<double>* coefficients, double x)
{
  int degree = coefficients->size();
  double value = 0;
  for (int i = 0; i < degree; i++)
  {
    value += coefficients->at(i) * pow(x, degree - 1 - i);
  }
  return value;
}

QString generateFormula(std::vector<double>* coefficients)
{
  QStringList result;
  auto degree = coefficients->size();
  for (int i = 0; i < degree; i++)
  {
    if (coefficients->at(i) == 0.0)
    {
      continue;
    }

    int power = degree - 1 - i;
    if (power > 0)
    {
      result.append(QString("(%1*%2^%3)")
                        .arg(QString::number(coefficients->at(i)),
                             QString::fromStdString(BASE_LETTER), QString::number(degree - 1 - i)));
    }
    else
    {
      result.append(QString::number(coefficients->at(i)));
    }
  }

  return result.join(" + ");
}
