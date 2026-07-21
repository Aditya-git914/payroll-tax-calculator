#pragma once
#include "Payroll.h"
#include <vector>
#include <string>
#include <map>

struct DepartmentSummary {
    std::string department;
    int payslipCount = 0;
    double totalGross = 0.0;
    double totalTax = 0.0;
    double totalNet = 0.0;
    double averageNet = 0.0;
};

// Computes reporting/analytics figures over the full payroll history.
class Analytics {
public:
    explicit Analytics(const std::vector<PayrollRecord>& history);

    double totalGrossPayAllTime() const;
    double totalTaxCollectedAllTime() const; // income tax + SS + medicare
    double totalNetPayAllTime() const;
    double averageNetPay() const;
    double effectiveTaxRate() const; // totalTax / totalGross

    std::vector<DepartmentSummary> byDepartment() const;
    std::vector<PayrollRecord> topEarners(int n) const;   // by net pay, desc
    std::map<std::string, double> monthlyNetPayTrend() const; // period -> total net pay
    std::map<std::string, double> monthlyTaxTrend() const;    // period -> total tax

    void printReport() const;

private:
    const std::vector<PayrollRecord>& history_;
};
