#include "calc_controller.h"

namespace s21 {
    
void CaclController::inputExpression(const std::string& infix, long double x) noexcept {
    expression_.inputExpression(infix);
    expression_.inputObjectX(x);
}

void CaclController::calculate() {
    expression_.сalculate();
}

long double CaclController::returnResult() const noexcept {
        return expression_.returnResult();
}

enum Status CaclController::returnStatus() const noexcept {
    return expression_.returnStatus();
}

} // namespace s21
