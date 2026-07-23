# C++ Payroll & Tax Calculator

A console-based payroll system in modern C++ (C++17) with progressive
tax calculation, CSV-based persistence, and a built-in analytics report.

## Features

- **Employee management** — add, list, and remove employees (name,
  department, base salary, bonus, other deductions, dependents).
- **Progressive tax calculation** — configurable tax brackets, plus
  flat-rate social security and medicare-style contributions, with a
  per-dependent deduction. See `TaxCalculator.h/.cpp`.
- **Payroll processing** — computes a full payslip (gross, contributions,
  income tax, deductions, net pay) for every employee for a given period.
- **Persistence** — employees and full payroll history are stored as CSV
  files under `data/` and automatically reloaded on the next run. CSV
  fields are properly quoted/escaped, so names with commas or quotes work.
- **Analytics** — total gross/tax/net across all history, effective tax
  rate, per-department breakdown, top earners, and a monthly net-pay/tax
  trend.

## Project layout


payroll_project/
├── include/          Header files (interfaces)
│   ├── Csv.h              CSV quoting/parsing helpers
│   ├── Employee.h
│   ├── TaxCalculator.h
│   ├── Payroll.h
│   ├── Persistence.h
│   └── Analytics.h
├── src/               Implementations + entry point
│   ├── Employee.cpp
│   ├── TaxCalculator.cpp
│   ├── Payroll.cpp
│   ├── Persistence.cpp
│   ├── Analytics.cpp
│   └── main.cpp        Interactive CLI menu
├── data/              CSV data files (created on first run)
├── Makefile
└── CMakeLists.txt

## Building

With make (no other dependencies beyond a C++17 compiler):

bash
make          # builds ./payroll
make run      # builds and runs
make clean    # removes build artifacts


Or with CMake:

bash
mkdir build && cd build
cmake ..
make
./payroll


## Running

bash
./payroll


You'll get an interactive menu:


1. Add employee
2. List employees
3. Remove employee
4. Run payroll for a period
5. View payroll history
6. Analytics report
7. Exit


Run payroll for a period (e.g. 2026-07) after adding employees — it
generates a payslip per employee, prints it, and appends it to
data/payroll_history.csv`. Employees are saved to data/employees.csv
immediately whenever you add or remove one, so nothing is lost even if
you exit without going through the menu's "save" step (there isn't
one — it's always saved).

## Customizing the tax model

TaxCalculator's default brackets are illustrative example values
(loosely modeled on a simple progressive schedule), not a specific
country's real tax code. To adjust them for your own use case, edit the
brackets vector in TaxCalculator::TaxCalculator() (in
src/TaxCalculator.cpp), or call setBrackets(...),
setSocialSecurityRate(...), setMedicareRate(...), and
setDependentDeduction(...) before constructing PayrollProcessor.

## Data format

data/employees.csv
id,name,department,base_salary,bonus,other_deductions,dependents


data/payroll_history.csv`:

employee_id,employee_name,department,period,gross_pay,social_security,medicare,income_tax,other_deductions,net_pay


Both files are plain CSV and can be opened directly in Excel/Sheets or
processed by other tools.
