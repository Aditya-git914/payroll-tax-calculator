cmake_minimum_required(VERSION 3.10)
project(PayrollTaxCalculator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(payroll
    src/main.cpp
    src/Employee.cpp
    src/TaxCalculator.cpp
    src/Payroll.cpp
    src/Persistence.cpp
    src/Analytics.cpp
)
