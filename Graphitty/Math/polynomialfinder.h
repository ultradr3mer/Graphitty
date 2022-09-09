#ifndef POLYNOMIALFINDER_H
#define POLYNOMIALFINDER_H

#include <QString>
#include <vector>

extern std::vector<double> findPolynomial(std::vector<std::array<double, 2>> points);
extern double calculatePolynomial(std::vector<double>* coefficients, double x);
extern QString generateFormula(std::vector<double>* coefficients);

#endif
