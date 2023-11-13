/*
 * compile with the -g option to include debugging symbols:
 *   clang++ lldbtest.cpp -Wall -g
 * then:
 *   lldb a.out
 *
 * some commands:
 *   h: lists all available commands
 *   run: runs the program
 *   run arg1 [,arg2...]: runs with arguments
 *   breakpoint set--file lldbtest.cpp --line 6: sets a breakpoint at line 6
 *   n: executs the current line
 *   s: steps into a function
 *   gui: loads the ncurses GUI
 * */

#include <iostream>

using namespace std;

int main() {
	int f = 1;

	for (int i = 2; i <= 5; ++i) {
		f *= i;
	}

	cout << f << endl;

	return 0;
}