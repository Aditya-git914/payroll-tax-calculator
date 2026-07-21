#include "Payroll.h"
#include "Csv.h"
#include <stdexcept>
#include <algorithm>

std::string PayrollRecord::toCsvRow() const {
    return csv::joinRow(
        std::to_string(employeeId),
        employeeName,
        department,
        period,
        std::to_string(grossPay),
        std::to_string(socialSecurity),
        std::to_string(medicare),
        std::to_string(incomeTax),
        std::to_string(otherDeductions),
        std::to_string(netPay)
    );
}

PayrollRecord PayrollRecord::fromCsvRow(const std::string& row) {
    auto f = csv::split(row);
    if (f.size() < 10) {
        throw std::runtime_error("Malformed payroll CSV row: " + row);
    }
    PayrollRecord r;
    r.employeeId = std::stoi(f[0]);
    r.employeeName = f[1];
    r.department = f[2];
    r.period = f[3];
    r.grossPay = std::stod(f[4]);
    r.socialSecurity = std::stod(f[5]);
    r.medicare = std::stod(f[6]);
    r.incomeTax = std::stod(f[7]);
    r.otherDeductions = std::stod(f[8]);
    r.netPay = std::stod(f[9]);
    return r;
}

PayrollProcessor::PayrollProcessor(const TaxCalculator& calc) : taxCalc(calc) {}

PayrollRecord PayrollProcessor::process(const Employee& emp, const std::string& period) const {
    PayrollRecord rec;
    rec.employeeId = emp.id;
    rec.employeeName = emp.name;
    rec.department = emp.department;
    rec.period = period;

    double grossPay = emp.baseSalary + emp.bonus;
    rec.grossPay = grossPay;

    // Payroll contributions (flat-rate, applied to gross pay)
    rec.socialSecurity = grossPay * taxCalc.socialSecurityRate();
    rec.medicare = grossPay * taxCalc.medicareRate();

    // Income tax: annualize gross, subtract dependent deductions, tax it,
    // then bring the result back to a monthly figure.
    double annualGross = grossPay * 12.0;
    double annualDeduction = emp.dependents * taxCalc.standardDeductionPerDependent();
    double annualTaxable = std::max(0.0, annualGross - annualDeduction);
    double annualTax = taxCalc.calculateAnnualTax(annualTaxable);
    rec.incomeTax = annualTax / 12.0;

    rec.otherDeductions = emp.otherDeductions;

    rec.netPay = grossPay - rec.socialSecurity - rec.medicare - rec.incomeTax - rec.otherDeductions;
    return rec;
}
