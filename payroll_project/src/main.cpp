#include "Employee.h"
#include "TaxCalculator.h"
#include "Payroll.h"
#include "Persistence.h"
#include "Analytics.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

namespace {

void clearInputError() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInputError();
            return value;
        }
        std::cout << "Please enter a valid integer.\n";
        clearInputError();
    }
}

double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInputError();
            return value;
        }
        std::cout << "Please enter a valid number.\n";
        clearInputError();
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int nextEmployeeId(const std::vector<Employee>& employees) {
    int maxId = 0;
    for (const auto& e : employees) maxId = std::max(maxId, e.id);
    return maxId + 1;
}

void printEmployeeTable(const std::vector<Employee>& employees) {
    if (employees.empty()) {
        std::cout << "No employees on file.\n";
        return;
    }
    std::cout << std::left
               << std::setw(5) << "ID"
               << std::setw(20) << "Name"
               << std::setw(15) << "Department"
               << std::right << std::setw(12) << "Base"
               << std::setw(10) << "Bonus"
               << std::setw(12) << "Deductions"
               << std::setw(8) << "Deps" << "\n";
    std::cout << std::string(82, '-') << "\n";
    std::cout << std::fixed << std::setprecision(2);
    for (const auto& e : employees) {
        std::cout << std::left
                   << std::setw(5) << e.id
                   << std::setw(20) << e.name
                   << std::setw(15) << e.department
                   << std::right << std::setw(12) << e.baseSalary
                   << std::setw(10) << e.bonus
                   << std::setw(12) << e.otherDeductions
                   << std::setw(8) << e.dependents << "\n";
    }
}

void printPayslip(const PayrollRecord& r) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  " << std::left << std::setw(20) << r.employeeName
               << " [" << r.department << "] period " << r.period << "\n";
    std::cout << "    Gross: " << r.grossPay
               << "  SS: " << r.socialSecurity
               << "  Medicare: " << r.medicare
               << "  IncomeTax: " << r.incomeTax
               << "  OtherDed: " << r.otherDeductions
               << "  -> Net: " << r.netPay << "\n";
}

void addEmployee(std::vector<Employee>& employees, const Persistence& store) {
    Employee e;
    e.id = nextEmployeeId(employees);
    e.name = readLine("Name: ");
    e.department = readLine("Department: ");
    e.baseSalary = readDouble("Monthly base salary: ");
    e.bonus = readDouble("Monthly bonus (0 if none): ");
    e.otherDeductions = readDouble("Other monthly deductions (insurance, etc, 0 if none): ");
    e.dependents = readInt("Number of dependents: ");

    employees.push_back(e);
    store.saveEmployees(employees);
    std::cout << "Added employee #" << e.id << " (" << e.name << ").\n";
}

void removeEmployee(std::vector<Employee>& employees, const Persistence& store) {
    int id = readInt("Employee ID to remove: ");
    auto it = std::remove_if(employees.begin(), employees.end(),
                              [id](const Employee& e) { return e.id == id; });
    if (it == employees.end()) {
        std::cout << "No employee with ID " << id << ".\n";
        return;
    }
    employees.erase(it, employees.end());
    store.saveEmployees(employees);
    std::cout << "Removed employee #" << id << ".\n";
}

void runPayroll(const std::vector<Employee>& employees,
                const PayrollProcessor& processor,
                std::vector<PayrollRecord>& history,
                const Persistence& store) {
    if (employees.empty()) {
        std::cout << "No employees to run payroll for.\n";
        return;
    }
    std::string period = readLine("Period to run (e.g. 2026-07): ");

    std::vector<PayrollRecord> batch;
    batch.reserve(employees.size());
    for (const auto& e : employees) {
        batch.push_back(processor.process(e, period));
    }

    std::cout << "\n-- Payroll run for " << period << " --\n";
    for (const auto& r : batch) printPayslip(r);

    store.appendPayrollRecords(batch);
    history.insert(history.end(), batch.begin(), batch.end());

    double totalNet = 0;
    for (const auto& r : batch) totalNet += r.netPay;
    std::cout << "\nRun complete. " << batch.size() << " payslips generated. "
               << "Total net paid out: " << std::fixed << std::setprecision(2)
               << totalNet << "\n";
}

void viewHistory(const std::vector<PayrollRecord>& history) {
    if (history.empty()) {
        std::cout << "No payroll history yet.\n";
        return;
    }
    for (const auto& r : history) printPayslip(r);
}

void printMenu() {
    std::cout << "\n===== C++ Payroll & Tax Calculator =====\n"
               << "1. Add employee\n"
               << "2. List employees\n"
               << "3. Remove employee\n"
               << "4. Run payroll for a period\n"
               << "5. View payroll history\n"
               << "6. Analytics report\n"
               << "7. Exit\n"
               << "Choose an option: ";
}

} // namespace

int main() {
    const std::string employeesFile = "data/employees.csv";
    const std::string payrollFile = "data/payroll_history.csv";

    Persistence store(employeesFile, payrollFile);
    TaxCalculator taxCalc;
    PayrollProcessor processor(taxCalc);

    std::vector<Employee> employees = store.loadEmployees();
    std::vector<PayrollRecord> history = store.loadPayrollHistory();

    std::cout << "Loaded " << employees.size() << " employee(s) and "
               << history.size() << " payroll record(s) from disk.\n";

    bool running = true;
    while (running) {
        printMenu();
        int choice = readInt("");
        switch (choice) {
            case 1: addEmployee(employees, store); break;
            case 2: printEmployeeTable(employees); break;
            case 3: removeEmployee(employees, store); break;
            case 4: runPayroll(employees, processor, history, store); break;
            case 5: viewHistory(history); break;
            case 6: { Analytics analytics(history); analytics.printReport(); break; }
            case 7: running = false; break;
            default: std::cout << "Invalid option.\n"; break;
        }
    }

    std::cout << "Data saved to " << employeesFile << " and " << payrollFile << ". Goodbye!\n";
    return 0;
}
