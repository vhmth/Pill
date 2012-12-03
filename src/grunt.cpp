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

#include "include/grunt.h"

std::string Grunt::makeGrepQuery(
	std::string query,
	std::string path,
	std::vector<std::string> extensions,
	unsigned int max_results
) {
	std::string resultQuery = std::string("grep");

	// grep flags
	// TODO: make them passable from the command line
	resultQuery += std::string(" -ir");

	std::string extensionsQuery("");
	for (unsigned int i = 0; i < extensions.size(); i++) {
		if (extensions[i].compare("*")) {
			extensionsQuery += std::string(" --include *.");
			extensionsQuery += extensions[i];
		} else {
			extensionsQuery = std::string("");
			break;
		}
	}

	resultQuery += extensionsQuery;
	resultQuery += std::string(" ") + query;
	resultQuery += std::string(" ") + path;

	return resultQuery;
}
