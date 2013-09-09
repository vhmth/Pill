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

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "include/scout.h"
#include "include/grunt.h"
#include "include/parrot.h"
#include "include/clerk.h"

const unsigned int NUM_OPTIONS = 5;
std::string options[NUM_OPTIONS] = {
	"--editor",
	"--path",
	"--role",
	"--max",
	"--get-roles"
};

void pill_help() {
	printf("\nWelcome to Pill! Pill allows you to easily pillage your codebase.\n\n");
	printf("The format of a Pill call is as follows:\n");
	printf("    pill <FLAGS> <SEARCH_STRING>\n\n");
	printf("The following flags are valid options for Pill:\n");

	for (unsigned int i = 0; i < NUM_OPTIONS; i++) {
		printf("    %s:\n", options[i].c_str());
		switch (i) {
			case 0:
				printf("        Specifies which editor Pill will open files with if you\n");
				printf("        choose to quickly open Pill results on the command line.\n");
				printf("        If no editor is supplied and none exists in options.json\n");
				printf("        the Pill subshell prompt for quickly opening its results\n");
				printf("        will not appear. Additionally, passing 'none' into this\n");
				printf("        flag will also deactivate the quick-open prompt (useful\n");
				printf("        if you have an editor specified in options.json but wish\n");
				printf("        to override this option to only display the Pill results).");
				break;
			case 1:
				printf("        Specifies the top-level path you wish Pill to search from.\n");
				printf("        Defaults to the current directory './'.");
				break;
			case 2:
				printf("        Specifies a role you wish to use for the query. Roles are\n");
				printf("        defined as key values pointing to an array of strings.\n");
				printf("        Each string entry in the array should denote a file\n");
				printf("        extension. Roles are defined in the folder 'options'\n");
				printf("        in a file called roles.js. A sample roles file can be\n");
				printf("        found in the 'options' folder. Defaults to all extensions\n");
				printf("        (['*']). If 'none' is supplied to this flag, all extensions\n");
				printf("        will be considered (useful for overriding a default role).");
				break;
			case 3:
				printf("        Integer specifiying the maximum number of results Pill\n");
				printf("        should return. Defaults to '25'.");
				break;
			default:
				printf("        Prints out the different roles as well as the file\n");
				printf("        extensions that make them up (in order of importance).\n");
				break;
		}
		printf("\n");
	}

	return;
}

int main(int argc, char *argv[]) {
	std::string query = "";
	std::string editor = "";
	std::string path = "";
	std::string role = "";
	std::vector<std::string> extensions;
	unsigned int results_cap = 0;

	// get the options from the command line
	for (int i = 1; i < argc; i++) {
		if (!options[0].compare(argv[i])) {
			editor = argv[i + 1];
			i++;
		} else if (!options[1].compare(argv[i])) {
			path = argv[i + 1];
			i++;
		} else if (!options[2].compare(argv[i])) {
			role = argv[i + 1];
			i++;
		} else if (!options[3].compare(argv[i])) {
			results_cap = atoi(argv[i + 1]);
			i++;
		} else if (i == argc - 1) {
			query = argv[i];
		} else if (strcmp(argv[i], "--help") || strcmp(argv[i], "--get-roles")) {
			printf("%s is not a valid option for Pill.\n", argv[i]);
		}
	}

	if (!query.compare("")) {
		printf("You must provide a search string as the last argument to Pill.\n");
		return 0;
	}

	if (!query.compare("--help")) {
		pill_help();
		return 0;
	}

	// scout will take care of parsing roles.json and options.json
	Scout scout;
	if (!scout.initialize(role)) {
		return 0;
	}

	if (!query.compare("--get-roles")) {
		std::vector<std::string> role_names = scout.getRoleNames();
		printf("The following roles are available for Pill:\n");
		for (unsigned int i = 0; i < role_names.size(); i++) {
			printf("%s: ", role_names[i].c_str());
			extensions = scout.getRole(role_names[i]);
			for (unsigned int j = 0; j < extensions.size() - 1; j++) {
				printf("%s, ", extensions[j].c_str());
			}
			printf("%s\n", extensions[extensions.size() - 1].c_str());
		}
		return 0;
	}

	if (results_cap == 0) {
		results_cap = scout.getMaxResults();
		if (results_cap <= 0) {
			return 0;
		}
	}
	if (!editor.compare("")) {
		editor = scout.getEditor();
	}
	if (!path.compare("")) {
		path = scout.getRoot();
	}
	if (!role.compare("") || !role.compare("none")) {
		extensions = scout.getDefaultRole();
	} else {
		extensions = scout.getRole(role);
	}

	std::string grep_query = Grunt::makeGrepQuery(
		query,
		path,
		extensions,
		results_cap
	);

	std::map<std::string, std::vector<std::string> > parrot_results;
	if (!Parrot::listenAndSquawk(grep_query, &parrot_results)) {
		return 0;
	}

	std::vector<std::vector<std::string> > pill_results;
	std::string prependPath = std::string("");
	if (grep_query.at(0) == 'c') {
		prependPath = path;
		if (prependPath.at(prependPath.size() - 1) != '/') {
			prependPath += std::string("/");
		}
	}
	if (!Clerk::sortAndRank(
			&extensions,
			&parrot_results,
			&pill_results,
			prependPath,
			results_cap)) {
		return 0;
	}
	if (!pill_results.size()) {
		return 0;
	}
	Clerk::speakUp(&pill_results, query.c_str());

	if (editor.compare("none")) {
		bool running = true;
		std::string choice;
		while (running) {
			printf("\nPlease enter a number 1 - %lu to open a file. Type 'exit' if you\n", pill_results.size());
			printf("wish to exit Pill:\n");
			std::getline(std::cin, choice);

			if (!choice.compare("exit")) {
				break;
			}

			unsigned int iChoice = atoi(choice.c_str());
			if (iChoice < 1 || iChoice > pill_results.size()) {
				printf("\n\t%s is an invalid file choice.\n", choice.c_str());
			} else {
				running  = false;
				iChoice--;

				// execute opening command
				char openCmd[
					editor.size() +
					1 +
					pill_results[iChoice][0].size() +
					1
				];
				strcpy(openCmd, editor.c_str());
				strcat(openCmd, " ");
				strcat(openCmd, pill_results[iChoice][0].c_str());
				openCmd[editor.size() + 1 + pill_results[iChoice][0].size()] = '\0';

				system(openCmd);
			}
		}
	}

	return 0;
}
