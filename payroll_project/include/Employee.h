#pragma once
#include <string>

struct Employee {
    int id = 0;
    std::string name;
    std::string department;
    double baseSalary = 0.0;     // monthly base salary (gross, before bonus/tax)
    double bonus = 0.0;          // monthly bonus, if any
    double otherDeductions = 0.0; // e.g. health insurance premium, monthly
    int dependents = 0;          // number of dependents (affects tax deduction)

    std::string toCsvRow() const;
    static Employee fromCsvRow(const std::string& row);
};
