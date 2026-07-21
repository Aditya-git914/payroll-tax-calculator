#include "Employee.h"
#include "Csv.h"
#include <stdexcept>

std::string Employee::toCsvRow() const {
    return csv::joinRow(
        std::to_string(id),
        name,
        department,
        std::to_string(baseSalary),
        std::to_string(bonus),
        std::to_string(otherDeductions),
        std::to_string(dependents)
    );
}

Employee Employee::fromCsvRow(const std::string& row) {
    auto f = csv::split(row);
    if (f.size() < 7) {
        throw std::runtime_error("Malformed employee CSV row: " + row);
    }
    Employee e;
    e.id = std::stoi(f[0]);
    e.name = f[1];
    e.department = f[2];
    e.baseSalary = std::stod(f[3]);
    e.bonus = std::stod(f[4]);
    e.otherDeductions = std::stod(f[5]);
    e.dependents = std::stoi(f[6]);
    return e;
}
