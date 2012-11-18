/*
 * Copyright (c) 2012 The Backplane Incorporated,
 *     Vinay Hiremath,
 *     Zach Tratar
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include <string.h>

using namespace std;

int main(int argc, char *argv[]) {
	// the options:
	// 1.) search string
	// 2.) editor
	// 3.) root search path
	// 4.) role/extension array
	// 5.) maximum results
	const unsigned int MAX_OPTIONS = 5;
	string options[MAX_OPTIONS];

	for (int i = 0; i < MAX_OPTIONS; i++) {
		options[i] = "";
	}

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--editor")) {
			if (i != (argc - 1)) {
				options[1] = argv[i + 1];
				i++;
			}
		} else if (!strcmp(argv[i], "--path")) {
			if (i != (argc - 1)) {
				options[2] = argv[i + 1];
				i++;
			}
		} else if (!strcmp(argv[i], "--role")) {
			if (i != (argc - 1)) {
				options[3] = argv[i + 1];
				i++;
			}
		} else if (!strcmp(argv[i], "--max")) {
			if (i != (argc - 1)) {
				options[4] = argv[i + 1];
				i++;
			}
		} else if (i == 1) {
			options[0] = argv[i];
		}
	}

	// 2.) default any options not set or invalid (empty strings)

	// 3.) grab the options from external files

	// 4.) execv

	// 5.) while (1) { TAKE_IN_FILE_TO_OPEN }

	return 0;
}
