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
	// the default options
	string query = "";
	string editor = "vim";
	string path = "./";
	string role = "";
	string extensions[] = {"*"};
	unsigned int results_cap = 25;

	// get the options from the command line
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--editor")) {
			if (i != (argc - 1)) {
				editor = argv[i + 1];
				i++;
			}
		} else if (!strcmp(argv[i], "--path")) {
			if (i != (argc - 1)) {
				path = argv[i + 1];
				i++;
			}
		} else if (!strcmp(argv[i], "--role")) {
			if (i != (argc - 1)) {
				role = argv[i + 1];
				i++;
			}
		} else if (!strcmp(argv[i], "--max")) {
			if (i != (argc - 1)) {
				results_cap = atoi(argv[i + 1]);
				i++;
			}
		} else if (i == 1) {
			query = argv[i];
		}
	}

	if (!query.compare("")) {
		cout << "You must provide a search string as the first argument to pill." << endl;
		return 0;
	}

	// grab the option from external files
	if (role.compare("none") || !role.compare("")) {
		string default_role = "";
		// TODO: get the default role from ../options/options.json

		if (default_role.compare("")) {
			// we have the default role from options.json, so let's
			// load the extensions array with the array pointed to
			// by the default_role key in ../options/roles.json
		}
	}

	// 1.) execv

	// 2.) while (1) { TAKE_IN_FILE_TO_OPEN }

	return 0;
}
