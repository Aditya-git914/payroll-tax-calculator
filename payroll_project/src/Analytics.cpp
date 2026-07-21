#include "Analytics.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

Analytics::Analytics(const std::vector<PayrollRecord>& history) : history_(history) {}

double Analytics::totalGrossPayAllTime() const {
    double total = 0.0;
    for (const auto& r : history_) total += r.grossPay;
    return total;
}

double Analytics::totalTaxCollectedAllTime() const {
    double total = 0.0;
    for (const auto& r : history_) total += r.incomeTax + r.socialSecurity + r.medicare;
    return total;
}

double Analytics::totalNetPayAllTime() const {
    double total = 0.0;
    for (const auto& r : history_) total += r.netPay;
    return total;
}

double Analytics::averageNetPay() const {
    if (history_.empty()) return 0.0;
    return totalNetPayAllTime() / static_cast<double>(history_.size());
}

double Analytics::effectiveTaxRate() const {
    double gross = totalGrossPayAllTime();
    if (gross <= 0.0) return 0.0;
    return totalTaxCollectedAllTime() / gross;
}

std::vector<DepartmentSummary> Analytics::byDepartment() const {
    std::map<std::string, DepartmentSummary> agg;
    for (const auto& r : history_) {
        auto& d = agg[r.department];
        d.department = r.department;
        d.payslipCount += 1;
        d.totalGross += r.grossPay;
        d.totalTax += (r.incomeTax + r.socialSecurity + r.medicare);
        d.totalNet += r.netPay;
    }
    std::vector<DepartmentSummary> result;
    result.reserve(agg.size());
    for (auto& [name, summary] : agg) {
        summary.averageNet = summary.payslipCount > 0
            ? summary.totalNet / summary.payslipCount
            : 0.0;
        result.push_back(summary);
    }
    std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
        return a.totalGross > b.totalGross;
    });
    return result;
}

std::vector<PayrollRecord> Analytics::topEarners(int n) const {
    std::vector<PayrollRecord> sorted = history_;
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.netPay > b.netPay;
    });
    if (static_cast<int>(sorted.size()) > n) sorted.resize(n);
    return sorted;
}

std::map<std::string, double> Analytics::monthlyNetPayTrend() const {
    std::map<std::string, double> trend;
    for (const auto& r : history_) trend[r.period] += r.netPay;
    return trend;
}

std::map<std::string, double> Analytics::monthlyTaxTrend() const {
    std::map<std::string, double> trend;
    for (const auto& r : history_) trend[r.period] += (r.incomeTax + r.socialSecurity + r.medicare);
    return trend;
}

void Analytics::printReport() const {
    std::cout << std::fixed << std::setprecision(2);

    if (history_.empty()) {
        std::cout << "No payroll history yet. Run a payroll cycle first.\n";
        return;
    }

    std::cout << "\n==================== PAYROLL ANALYTICS ====================\n";
    std::cout << "Payslips on record : " << history_.size() << "\n";
    std::cout << "Total gross pay     : " << totalGrossPayAllTime() << "\n";
    std::cout << "Total tax collected : " << totalTaxCollectedAllTime()
               << "  (effective rate " << (effectiveTaxRate() * 100.0) << "%)\n";
    std::cout << "Total net pay       : " << totalNetPayAllTime() << "\n";
    std::cout << "Average net payslip : " << averageNetPay() << "\n";

    std::cout << "\n-- By Department --\n";
    std::cout << std::left << std::setw(15) << "Department"
               << std::right << std::setw(10) << "Slips"
               << std::setw(15) << "Gross"
               << std::setw(15) << "Tax"
               << std::setw(15) << "Net"
               << std::setw(15) << "Avg Net" << "\n";
    for (const auto& d : byDepartment()) {
        std::cout << std::left << std::setw(15) << d.department
                   << std::right << std::setw(10) << d.payslipCount
                   << std::setw(15) << d.totalGross
                   << std::setw(15) << d.totalTax
                   << std::setw(15) << d.totalNet
                   << std::setw(15) << d.averageNet << "\n";
    }

    std::cout << "\n-- Top 5 Earners (by net pay, any period) --\n";
    for (const auto& r : topEarners(5)) {
        std::cout << "  " << std::left << std::setw(20) << r.employeeName
                   << " [" << r.period << "] net: " << r.netPay << "\n";
    }

    std::cout << "\n-- Monthly Net Pay Trend --\n";
    for (const auto& [period, total] : monthlyNetPayTrend()) {
        std::cout << "  " << period << " : " << total << "\n";
    }
    std::cout << "=============================================================\n";
}
