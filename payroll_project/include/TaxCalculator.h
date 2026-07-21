#pragma once
#include <vector>

// A single progressive tax bracket.
// upTo == -1 means "and above" (no upper bound).
struct TaxBracket {
    double upTo;
    double rate;
};

// Computes income tax using configurable progressive brackets,
// plus flat-rate payroll contributions (social security / medicare style).
// The default brackets are illustrative/example values and can be
// replaced with real jurisdiction-specific figures via setBrackets().
class TaxCalculator {
public:
    TaxCalculator();

    double calculateAnnualTax(double annualTaxableIncome) const;
    double calculateMonthlyTax(double monthlyTaxableIncome) const;

    void setBrackets(std::vector<TaxBracket> newBrackets);
    const std::vector<TaxBracket>& getBrackets() const { return brackets; }

    double standardDeductionPerDependent() const { return dependentDeductionAnnual; }
    double socialSecurityRate() const { return ssRate; }
    double medicareRate() const { return medicareRateValue; }

    void setDependentDeduction(double annualAmount) { dependentDeductionAnnual = annualAmount; }
    void setSocialSecurityRate(double rate) { ssRate = rate; }
    void setMedicareRate(double rate) { medicareRateValue = rate; }

private:
    std::vector<TaxBracket> brackets;
    double dependentDeductionAnnual = 2000.0;
    double ssRate = 0.062;
    double medicareRateValue = 0.0145;
};
