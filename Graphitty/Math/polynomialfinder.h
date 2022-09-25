/***************************************************************************
 * SoSe2022 Praktikum C++ für JAVA Programmierer
 * Graphitty
 * Gruppe: Schulz-Theißen Clara, Ahrens Pascal, Jaenisch Lukas
 * Datum: 25.09.22
 **************************************************************************/

#ifndef POLYNOMIALFINDER_H
#define POLYNOMIALFINDER_H

#include <QString>
#include <vector>

extern std::vector<double> findPolynomial(std::vector<std::array<double, 2>> points);
extern double calculatePolynomial(std::vector<double>* coefficients, double x);
extern QString generateFormula(std::vector<double>* coefficients);

#endif
