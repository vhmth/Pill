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

#include "include/scout.h"
#include "include/grunt.h"
#include "include/parrot.h"

#define RED "\e[31m"
#define BLUE "\e[34m"
#define RESET_COLOR "\e[m"

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

static inline bool compareVectorsBySize(
	const std::vector<std::string>& a,
	const std::vector<std::string>& b) {
	return a.size() > b.size();
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
		} else if (strcmp(argv[i], "--help") || strcmp(argv[i], "--get-roles")) {
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
	if (!role.compare("")) {
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

	// sort parrot_results giving weight to the following
	// factors (in order of decreasing precedence):
	//     file extension (based on extensions vector)
	//     number of occurances
	//
	// cuts off the results at results_cap
	std::vector<std::vector<std::string> > sorted_results;
	std::map<std::string, std::vector<std::string> >::iterator it;
	unsigned int curr_starting_pos = 0;

	for (unsigned int i = 0; i < extensions.size() && sorted_results.size() < results_cap; i++) {

		for (it = parrot_results.begin(); it != parrot_results.end(); it++) {
			std::string path = (*it).first;
			size_t ext_pos = path.find_last_of(".");
			if (ext_pos != std::string::npos) {
				// fix if fget call in Parrot didn't null terminate path string
				if (path.c_str()[path.size() + 1] == '\0') {
					char strtemp[path.size()];
					for (unsigned k = 0; k < path.size() - 1; k++) {
						strtemp[k] = path.at(k);
					}
					strtemp[path.size() - 1] = '\0';
					path = std::string(strtemp);
				}

				std::string currPathExtension = path.substr(ext_pos + 1);
				std::vector<std::string> currPathVector;

				if (!extensions[i].compare("*") || !extensions[i].compare(currPathExtension)) {
					currPathVector.push_back(path);
					for (unsigned int i = 0; i < parrot_results[path].size(); i++) {
						currPathVector.push_back(parrot_results[path][i]);
					}
				}
				if (currPathVector.size()) {
					sorted_results.push_back(currPathVector);
				}
				if (sorted_results.size() == results_cap) {
					break;
				}
			}
		}

		std::vector<std::vector<std::string> >::iterator vecIt = sorted_results.begin();
		unsigned int j = 0;
		while (curr_starting_pos != 0 && j < (curr_starting_pos + 1)) {
			vecIt++;
			j++;
		}
		sort(vecIt, sorted_results.end(), compareVectorsBySize);
		curr_starting_pos = sorted_results.size() - 1;
	}

	for (unsigned int i = 0; i < sorted_results.size(); i++) {
		const char *path = sorted_results[i][0].c_str();
		char pathStr[strlen(path) + strlen(RED) + 1];
		strcpy(pathStr, RED);
		printf(strcat(pathStr, "%s:\n"), path);
		for (unsigned int j = 1; j < sorted_results[i].size(); j++) {
			const char *occurance = sorted_results[i][j].c_str();
			char occStr[strlen(occurance) + strlen(BLUE) + 1];
			strcpy(occStr, BLUE);
			printf(strcat(occStr, "\t\t%s\n"), sorted_results[i][j].c_str());
		}
	}
	printf(RESET_COLOR);

	// TODO: while (1) { TAKE_IN_FILE_TO_OPEN }

	return 0;
}
