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
#include <stdio.h>

#include "include/grunt.h"

static inline bool gitExists(const char *path) {
	char cmd[
		3 +
		strlen(path) +
		30 +
		1
	];
	strcpy(cmd, "cd ");
	strcat(cmd, path);
	strcat(cmd, ";sh /Pill/scripts/gitexists.sh");
	FILE *stream = popen(cmd, "r");
	if (stream != NULL) {
		char result[10];
		if (fgets(result, 10, stream) != NULL) {
			return !strcmp(result, "TRUE\n");
		}
	}

	return false;
}

static inline std::string makeExtensionsQuery(
	std::vector<std::string> extensions,
	const char *path,
	bool isGitDir,
	bool pwdIsGitDir
) {
	std::string extensionsQuery("");

	for (unsigned int i = 0; i < extensions.size(); i++) {
		if (extensions[i].compare("*")) {
			if (!isGitDir) {
				extensionsQuery += std::string(" --include=*.");
			} else {
				extensionsQuery += std::string(" -- '");
				if (pwdIsGitDir) {
					extensionsQuery += std::string(path);
				}
				extensionsQuery += std::string("*.");
			}

			extensionsQuery += extensions[i];

			if (isGitDir) {
				extensionsQuery += std::string("'");
			}
		} else {
			extensionsQuery = std::string("");
			break;
		}
	}

	if (!extensionsQuery.compare("") && isGitDir && pwdIsGitDir) {
		extensionsQuery = std::string(" ");
		extensionsQuery += std::string(path);
	}

	return extensionsQuery;
}

std::string Grunt::makeGrepQuery(
	const std::string query,
	const std::string path,
	const std::vector<std::string> extensions,
	const unsigned int max_results
) {
	std::string resultQuery;
	bool isGitDir = gitExists(path.c_str());
	bool pwdIsGitDir = gitExists("./");

	if (!isGitDir) {
		resultQuery = std::string("grep -iIr");
	} else {
		if (pwdIsGitDir) {
			resultQuery = std::string("git grep -Ii");
		} else {
			resultQuery = std::string("cd ");
			resultQuery += path;
			resultQuery += std::string(";git grep -Ii");
		}
	}

	std::string extensionsQuery = makeExtensionsQuery(
		extensions,
		path.c_str(),
		isGitDir,
		pwdIsGitDir
	);

	if (!isGitDir) {
		resultQuery += extensionsQuery;
		resultQuery += std::string(" ") + query;
		resultQuery += std::string(" ") + path;
	} else {
		resultQuery += std::string(" ") + query;
		resultQuery += extensionsQuery;
	}

	return resultQuery;
}
