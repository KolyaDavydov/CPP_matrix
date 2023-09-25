CC = g++ -std=c++17 -Wall -Werror -Wextra -Wpedantic
SOURCE = s21_matrix_oop.cc
OBJECT = $(SOURCE:.cc=.o)
TEST_FLAGS =-lgtest -lgcov --coverage

all: clean s21_matrix_oop.a

s21_matrix_oop.a:$(SOURCE)
	$(CC) -c $(SOURCE)
	@ar rcs s21_matrix_oop.a $(OBJECT)

test: clean test.cc s21_matrix_oop.a
	$(CC) test.cc s21_matrix_oop.a $(TEST_FLAGS) -o test
	./test

# В условиях нет необходимости делать эту цель
# gcov_report: test
# 	lcov -t test -o report.info -c -d .
# 	genhtml -o report report.info
# 	open report/index.html

clang:
	clang-format -style=Google -n *.cc  *.h

leaks_test: clean test
ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	valgrind --leak-check=full --show-reachable=yes --show-leak-kinds=all --track-origins=yes --log-file=RESULT_VALGRIND.txt ./test
endif

clean:
	@rm -rf *.o *.a report.info *.gcda *.gcno report RESULT_VALGRIND.txt test