#ifndef CPP3_SMARTCALC_SRC_MODEL_CALC_MODEL_H
#define CPP3_SMARTCALC_SRC_MODEL_CALC_MODEL_H

#include <stack>
#include <string>
#include <cmath>
#include <cstring>
#include <unordered_map>
#include <vector>

namespace s21 {
enum Status { OK, ERROR };

class CalcModel {
 public:
  CalcModel() = default;
  ~CalcModel() = default;

  void сalculate();
  enum Status returnStatus() const;
  long double returnResult() const;
  void inputObjectX(const long double x) noexcept;
  void inputExpression(const std::string &infix) noexcept;

 private:
  enum Status expressionInPotsfix();

  void NumberToStack(std::string::iterator &iterator);
  void Calc(std::string::iterator &iterator) ;

  void postNumber(std::string::iterator &iterator) noexcept;
  void unaryOrpostPow(char op) noexcept;
  void postUnar(std::string::iterator &iterator) noexcept;
  void postBrackets(std::string::iterator &iterator) noexcept;
  void postFunctions(std::string::iterator &iterator) noexcept;
  void postPow(std::string::iterator &iterator) noexcept;
  inline void handleError();
  void processOperators(std::string::iterator& iterator);



  inline bool isDigit(std::string::iterator &iterator) noexcept;
  inline bool isUnar(std::string::iterator &iterator) noexcept;
  inline bool isBrackets(std::string::iterator &iterator) noexcept;
  inline bool isFunction(std::string::iterator &iterator) noexcept;
  inline bool isOperators(std::string::iterator &iterator) noexcept;
  inline bool isPow(std::string::iterator &iterator) noexcept;
  bool check() noexcept;

  inline int priority(char ch) noexcept;
  inline void objectToPost() noexcept;
  inline char findFuntion(std::string::iterator &iterator, bool iter = false) noexcept;

  enum Status status_ = OK;
  long double x_ = 0;
  long double resultExpression_ = 0;
  std::string expression_ = "";
  std::string postExpression_;
  std::stack<long double> stack_;

};  

}  

#endif  // CPP3_SMARTCALC_SRC_MODEL_CALC_MODEL_H
