#include "calc_model.h"

namespace s21 {

void CalcModel::сalculate() {
  if (expressionInPotsfix() != ERROR) {
    for (auto iterator = postExpression_.begin();
         iterator != postExpression_.end(); ++iterator) {
      if (*iterator == ' ') {
        continue;
      } else if (isDigit(iterator)) {
        NumberToStack(iterator);
      } else {
        Calc(iterator);
      }
      if (status_ == OK && !stack_.empty()) resultExpression_ = stack_.top();
    }
  }
  if (status_ == OK && !stack_.empty()) {
    stack_.pop();
  }

  if (!stack_.empty()) handleError();
}

enum Status CalcModel::expressionInPotsfix() {
  if (!check()) return ERROR;
  for (auto iterator = expression_.begin(); iterator != expression_.end();
       ++iterator) {
    if (*iterator == ' ') {
      continue;
    } else if (isDigit(iterator)) {
      postNumber(iterator);
    } else if (isUnar(iterator)) {
      postUnar(iterator);
    } else if (isBrackets(iterator)) {
      postBrackets(iterator);
    } else if (isFunction(iterator)) {
      postFunctions(iterator);
    } else if (isPow(iterator)) {
      postPow(iterator);
    } else if (isOperators(iterator)) {
      processOperators(iterator);
    } else {
      handleError();
    }
  }

  while (!stack_.empty()) {
    objectToPost();
  }

  if (status_ != OK) {
    postExpression_ = "";
  }

  return status_;
}

void CalcModel::processOperators(std::string::iterator &iterator) {
  while (!stack_.empty() && priority(*iterator) <= priority(stack_.top())) {
    objectToPost();
  }

  stack_.push(*iterator);
}

void CalcModel::NumberToStack(std::string::iterator &iterator) {
  if (*iterator == 'x') {
    stack_.push(x_);
    return;
  }

  std::string num;
  while (isDigit(iterator)) num.push_back(*iterator++);

  try {
    stack_.push(std::stod(num));
  } catch (...) {
    handleError();
  }
}

void CalcModel::handleError() { status_ = ERROR; }

void CalcModel::Calc(std::string::iterator &iterator) {
  if (stack_.empty()) {
    handleError();
    return;
  }

  long double operand = stack_.top();
  stack_.pop();

  if (strchr("+-*^%/", *iterator)) {
    if (stack_.empty()) {
      handleError();
      return;
    }

    long double operand2 = stack_.top();
    stack_.pop();

    switch (*iterator) {
      case '+':
        stack_.push(operand2 + operand);
        break;
      case '-':
        stack_.push(operand2 - operand);
        break;
      case '*':
        stack_.push(operand2 * operand);
        break;
      case '^':
        stack_.push(std::pow(operand2, operand));
        break;
      case '/':
        stack_.push((operand != 0) ? operand2 / operand : (handleError(), 0));
        break;
      case '%':
        stack_.push(std::fmod(operand2, operand));
        break;
    }
  } else if (*iterator >= 'A' && *iterator <= 'I') {
    switch (*iterator) {
      case 'A':
        stack_.push(std::cos(operand));
        break;
      case 'B':
        stack_.push(std::sin(operand));
        break;
      case 'C':
        stack_.push(std::tan(operand));
        break;
      case 'D':
        stack_.push(std::acos(operand));
        break;
      case 'E':
        stack_.push(std::asin(operand));
        break;
      case 'F':
        stack_.push(std::atan(operand));
        break;
      case 'G':
        stack_.push((operand >= 0) ? std::sqrt(operand) : (handleError(), 0));
        break;
      case 'H':
        stack_.push((operand > 0) ? std::log10(operand) : (handleError(), 0));
        break;
      case 'I':
        stack_.push((operand > 0) ? std::log(operand) : (handleError(), 0));
        break;
    }
  } else if (*iterator == '~') {
    stack_.push(operand * -1);
  } else if (*iterator == '&') {
    stack_.push(operand);
  } 
}

bool CalcModel::isDigit(std::string::iterator &iterator) noexcept {
  if (iterator == expression_.end()) {
    return false;
  }
  return isdigit(*iterator) || strchr(".,xe", *iterator) ||
         (strchr("+-", *iterator) && iterator != expression_.begin() &&
          *(iterator - 1) == 'e');
}

bool CalcModel::isUnar(std::string::iterator &iterator) noexcept {
  return strchr("+-", *iterator) &&
         (iterator == expression_.begin() || *(iterator - 1) == '(');
}

bool CalcModel::isBrackets(std::string::iterator &iterator) noexcept {
  return strchr("()", *iterator);
}

bool CalcModel::isOperators(std::string::iterator &iterator) noexcept {
  return strchr("+-*/%^", *iterator);
}

bool CalcModel::isFunction(std::string::iterator &iterator) noexcept {
  char ch = findFuntion(iterator);
  return ch >= 'A' && ch <= 'I' && *iterator != ch;
}

bool CalcModel::isPow(std::string::iterator &iterator) noexcept {
  return *iterator == '^';
}

bool CalcModel::check() noexcept{
  int balance = 0;
  if (expression_.empty()) {
    handleError();
    return false;
  }
  for (auto iterator = expression_.begin(); iterator != expression_.end();
       ++iterator) {
    balance += (*iterator == '(') ? 1 : (*iterator == ')') ? -1 : 0;
  }
  status_ = (balance == 0) ? OK : ERROR;
  return balance == 0;
}

void CalcModel::postNumber(std::string::iterator &iterator) noexcept {
  while (isDigit(iterator)) postExpression_.push_back(*iterator++);
  *iterator--;
  postExpression_.push_back(' ');  //-----------------------
}

void CalcModel::unaryOrpostPow(char op) noexcept {
  while (!stack_.empty() && priority(op) <= priority(stack_.top()))
    objectToPost();
  stack_.push(op);
}

void CalcModel::postUnar(std::string::iterator &iterator) noexcept {
  unaryOrpostPow((*iterator == '+') ? '&' : '~');
}

void CalcModel::postBrackets(std::string::iterator &iterator) noexcept {
  if (*iterator == '(')
    stack_.push(*iterator);  
  else {
    while (!stack_.empty() && stack_.top() != '(') objectToPost();
    stack_.empty() || stack_.top() == '(' ? stack_.pop() : handleError();
  }
}

void CalcModel::postPow(std::string::iterator &iterator) noexcept {
  unaryOrpostPow(*iterator);
}

void CalcModel::postFunctions(std::string::iterator &iterator) noexcept {
  unaryOrpostPow(findFuntion(iterator, true));
}

char CalcModel::findFuntion(std::string::iterator &iterator,
                                  bool iter) noexcept {
  const std::unordered_map<std::string, char> kfunc = {
      //
      {"cos", 'A'},  {"sin", 'B'},  {"tan", 'C'}, {"acos", 'D'}, {"asin", 'E'},
      {"atan", 'F'}, {"sqrt", 'G'}, {"log", 'H'}, {"ln", 'I'}};

  for (const auto &entry : kfunc) {
    const std::string &funcName = entry.first;
    if (std::equal(iterator, iterator + funcName.size(), funcName.begin(),
                   funcName.end())) {
      if (iter) iterator += funcName.size() - 1;
      return entry.second;
    }
  }
  return *iterator;
}



int CalcModel::priority(char ch) noexcept {
  switch (ch) {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
      return 2;
    case '^':
      return 3;
    case '%':
    case 'A' ... 'J':
      return 4;
    case '~':
    case '&':
      return 5;
    case '(':
      return -1;
    default:
      return 0;
  }
}

void CalcModel::objectToPost() noexcept {
  if (!stack_.empty()) postExpression_ += stack_.top(), stack_.pop();
}

void CalcModel::inputExpression(const std::string &infix) noexcept {
  expression_ = infix;
}

void CalcModel::inputObjectX(const long double x) noexcept { x_ = x; }

enum Status CalcModel::returnStatus() const { return status_; }

long double CalcModel::returnResult() const { return resultExpression_; }

}  // namespace s21
