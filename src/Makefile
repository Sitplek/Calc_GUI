CC := g++
CPPFLAGS := -lgtest -lgtest_main -std=c++17 -pthread
CHECKFLAGS := -Wall -Werror -Wextra
SRC := model/*.cc test/*.cc controller/*.cc
OS := $(shell uname)
OPEN_CMD=open

all: test gcov_report style install 

clean:
	@rm -f my_test *.gcda *.gcno *.info *.filtered gcov_report ./../*.log
	@rm -rf ./build-view-* ./report ../.vscode ../dist

test: clean
	@$(CC) $(CHECKFLAGS) $(SRC) -o my_test $(CPPFLAGS) && ./my_test

dvi:
	@$(OPEN_CMD) ../misc/info.html

gcov_report: clean
	@$(CC) --coverage $(SRC) ${CPPFLAGS} ${CHECKFLAGS} ${LIBS} -o gcov_report
	@./gcov_report
	@lcov -c -d . -o coverage.info --rc branch_coverage=1 --ignore-errors inconsistent -t "all_check"
	@lcov --rc branch_coverage=1 -e coverage.info 'model/*.cc'  -o coverage.info.filtered
	@genhtml coverage.info.filtered --output-directory report
	@$(OPEN_CMD) report/index.html;

clang:
	@clang-format -i -style=GOOGLE $(SRC) model/*.h controller/*.h view/*.cc view/*.h view/*.ui

style:
	@clang-format -n -style=GOOGLE $(SRC) model/*.h controller/*.h view/*.cc view/*.h view/*.ui


valgrind: clean test
	@CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test

.PHONY:  clean test dvi gcov_report clang style
