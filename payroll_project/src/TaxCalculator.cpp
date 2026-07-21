#include "TaxCalculator.h"
#include <algorithm>

TaxCalculator::TaxCalculator() {
    // Example progressive annual brackets (edit to match your jurisdiction).
    brackets = {
        {11000.0,   0.10},
        {44725.0,   0.12},
        {95375.0,   0.22},
        {182100.0,  0.24},
        {231250.0,  0.32},
        {578125.0,  0.35},
        {-1.0,      0.37}  // -1 => everything above the previous bracket
    };
}

void TaxCalculator::setBrackets(std::vector<TaxBracket> newBrackets) {
    brackets = std::move(newBrackets);
}

double TaxCalculator::calculateAnnualTax(double annualTaxableIncome) const {
    if (annualTaxableIncome <= 0) return 0.0;

    double tax = 0.0;
    double lowerBound = 0.0;

    for (const auto& bracket : brackets) {
        double upper = bracket.upTo;
        double taxableInThisBracket;

        if (upper < 0.0 || annualTaxableIncome <= upper) {
            // Final applicable bracket
            taxableInThisBracket = annualTaxableIncome - lowerBound;
            tax += std::max(0.0, taxableInThisBracket) * bracket.rate;
            break;
        } else {
            taxableInThisBracket = upper - lowerBound;
            tax += taxableInThisBracket * bracket.rate;
            lowerBound = upper;
        }
    }
    return tax;
}

double TaxCalculator::calculateMonthlyTax(double monthlyTaxableIncome) const {
    return calculateAnnualTax(monthlyTaxableIncome * 12.0) / 12.0;
}
