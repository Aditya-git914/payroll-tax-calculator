#pragma once
#include "Employee.h"
#include "TaxCalculator.h"
#include <string>

// One computed payslip for one employee for one period (e.g. "2026-07").
struct PayrollRecord {
    int employeeId = 0;
    std::string employeeName;
    std::string department;
    std::string period;
    double grossPay = 0.0;
    double socialSecurity = 0.0;
    double medicare = 0.0;
    double incomeTax = 0.0;
    double otherDeductions = 0.0;
    double netPay = 0.0;

    std::string toCsvRow() const;
    static PayrollRecord fromCsvRow(const std::string& row);
};

class PayrollProcessor {
public:
    explicit PayrollProcessor(const TaxCalculator& calc);

    // Computes a full payslip for the given employee/period.
    PayrollRecord process(const Employee& emp, const std::string& period) const;

private:
    const TaxCalculator& taxCalc;
};
