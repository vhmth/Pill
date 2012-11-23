/*
 * Copyright (c) 2012 The Backplane Incorporated,
 *     Vinay Hiremath
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
#include <json/json.h>

using namespace std;

const unsigned int NUM_OPTIONS = 4;
string options[NUM_OPTIONS] = {
	"--editor",
	"--path",
	"--role",
	"--max"
};

void pill_help() {
	printf("\nWelcome to Pill! Pill allows you to easily pillage your codebase.\n\n");
	printf("The format of a Pill call is as follows:\n");
	printf("    pill <SEARCH_STRING> <FLAGS>\n\n");
	printf("The following flags are valid options for Pill:\n");

	for (unsigned int i = 0; i < NUM_OPTIONS; i++) {
		printf("    %s:\n", options[i].c_str());
		switch (i) {
			case 0:
				printf("        Specifies which editor Pill will open files with if you\n");
				printf("        choose to quickly open Pill results on the command line.\n");
				printf("        Defaults to 'vim'.");
				break;
			case 1:
				printf("        Specifies the top-level path you wish Pill to search from.\n");
				printf("        Defaults to the current directory './'.\n");
				break;
			case 2:
				printf("        Specifies a role you wish to use for the query. Roles are\n");
				printf("        defined as key values pointing to an array of strings.\n");
				printf("        Each string entry in the array should denote a file\n");
				printf("        extension. Roles are defined in the folder 'options'\n");
				printf("        in a file called roles.js. A sample roles file can be\n");
				printf("        found in the 'options' folder. Defaults to all extensions\n");
				printf("        (['*']).");
				break;

			default:
				printf("        Integer specifiying the maximum number of results Pill\n");
				printf("        should return. Defaults to '25'.");
				break;
		}
		printf("\n");
	}

	return;
}

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
		if (!options[0].compare(argv[i])) {
			if (i != (argc - 1)) {
				editor = argv[i + 1];
				i++;
			}
		} else if (!options[1].compare(argv[i])) {
			if (i != (argc - 1)) {
				path = argv[i + 1];
				i++;
			}
		} else if (!options[2].compare(argv[i])) {
			if (i != (argc - 1)) {
				role = argv[i + 1];
				i++;
			}
		} else if (!options[3].compare(argv[i])) {
			if (i != (argc - 1)) {
				results_cap = atoi(argv[i + 1]);
				i++;
			}
		} else if (i == 1) {
			query = argv[i];
		} else if (strcmp(argv[i], "--help")) {
			printf("%s is not a valid option for Pill.", argv[i]);
		}
	}

	if (!query.compare("")) {
		printf("You must provide a search string as the first argument to Pill.");
		return 0;
	}

	if (!query.compare("--help")) {
		pill_help();
		return 0;
	}

	// grab the option from external files
	if (role.compare("none") || !role.compare("")) {
		string default_role = "";
		Json::Value root;
		Json::Reader reader;
		// TODO: get the default role from ../options/options.json

		if (default_role.compare("")) {
			// TODO:
			// we have the default role from options.json, so let's
			// load the extensions array with the array pointed to
			// by the default_role key in ../options/roles.json
		}
	}

	// TODO: execute grep

	// TODO: while (1) { TAKE_IN_FILE_TO_OPEN }

	return 0;
}
