#pragma once
#include "Employee.h"
#include "Payroll.h"
#include <vector>
#include <string>

// Handles reading/writing employees and payroll history to CSV files
// on disk, so data survives between program runs.
class Persistence {
public:
    Persistence(std::string employeesFile, std::string payrollFile);

    std::vector<Employee> loadEmployees() const;
    void saveEmployees(const std::vector<Employee>& employees) const;

    std::vector<PayrollRecord> loadPayrollHistory() const;
    void appendPayrollRecords(const std::vector<PayrollRecord>& records) const;
    void saveAllPayrollHistory(const std::vector<PayrollRecord>& records) const;

private:
    std::string employeesFile_;
    std::string payrollFile_;

    static const std::string employeeHeader;
    static const std::string payrollHeader;
};
