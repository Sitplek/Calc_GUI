#ifndef CPP3_SMARTCALC_SRC_CONTROLLER_CALC_CONTROLLER_H
#define CPP3_SMARTCALC_SRC_CONTROLLER_CALC_CONTROLLER_H

#include <string>

#include "../model/calc_model.h"

namespace s21 {

class CaclController {
public:

    CaclController() = default;
    void inputExpression(const std::string& infix, long double x) noexcept;
    void calculate();
    long double returnResult() const noexcept;
    enum Status returnStatus() const noexcept;

private:
    CalcModel expression_;
};

} // namespace s21

#endif // CPP3_SMARTCALC_SRC_CONTROLLER_CALC_CONTROLLER_H
