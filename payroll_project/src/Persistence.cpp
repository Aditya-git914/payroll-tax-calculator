#include "Persistence.h"
#include <fstream>
#include <iostream>

const std::string Persistence::employeeHeader =
    "id,name,department,base_salary,bonus,other_deductions,dependents";
const std::string Persistence::payrollHeader =
    "employee_id,employee_name,department,period,gross_pay,social_security,"
    "medicare,income_tax,other_deductions,net_pay";

Persistence::Persistence(std::string employeesFile, std::string payrollFile)
    : employeesFile_(std::move(employeesFile)), payrollFile_(std::move(payrollFile)) {}

std::vector<Employee> Persistence::loadEmployees() const {
    std::vector<Employee> employees;
    std::ifstream in(employeesFile_);
    if (!in.is_open()) return employees; // no file yet -> empty list

    std::string line;
    bool first = true;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (first) { first = false; continue; } // skip header
        try {
            employees.push_back(Employee::fromCsvRow(line));
        } catch (const std::exception& e) {
            std::cerr << "Warning: skipping bad employee row: " << e.what() << "\n";
        }
    }
    return employees;
}

void Persistence::saveEmployees(const std::vector<Employee>& employees) const {
    std::ofstream out(employeesFile_, std::ios::trunc);
    out << employeeHeader << "\n";
    for (const auto& e : employees) {
        out << e.toCsvRow() << "\n";
    }
}

std::vector<PayrollRecord> Persistence::loadPayrollHistory() const {
    std::vector<PayrollRecord> records;
    std::ifstream in(payrollFile_);
    if (!in.is_open()) return records;

    std::string line;
    bool first = true;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (first) { first = false; continue; }
        try {
            records.push_back(PayrollRecord::fromCsvRow(line));
        } catch (const std::exception& e) {
            std::cerr << "Warning: skipping bad payroll row: " << e.what() << "\n";
        }
    }
    return records;
}

void Persistence::appendPayrollRecords(const std::vector<PayrollRecord>& records) const {
    std::ifstream test(payrollFile_);
    bool needsHeader = !test.good() || test.peek() == std::ifstream::traits_type::eof();
    test.close();

    std::ofstream out(payrollFile_, std::ios::app);
    if (needsHeader) out << payrollHeader << "\n";
    for (const auto& r : records) {
        out << r.toCsvRow() << "\n";
    }
}

void Persistence::saveAllPayrollHistory(const std::vector<PayrollRecord>& records) const {
    std::ofstream out(payrollFile_, std::ios::trunc);
    out << payrollHeader << "\n";
    for (const auto& r : records) {
        out << r.toCsvRow() << "\n";
    }
}
