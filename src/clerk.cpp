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

#include "include/clerk.h"

#define RED "\e[31m"
#define BLUE "\e[34m"
#define RESET_COLOR "\e[m"

static inline bool compareVectorsBySize(
	const std::vector<std::string>& a,
	const std::vector<std::string>& b
) {
	return a.size() > b.size();
}

bool Clerk::sortAndRank(
	const std::vector<std::string> * extensions,
	std::map<std::string, std::vector<std::string> > *messy,
	std::vector<std::vector<std::string> > *clean,
	const unsigned int MAX_RESULTS
) {
	// sort the messy vector giving weight to the following
	// factors (in order of decreasing precedence):
	//     file extension (based on extensions vector)
	//     number of occurances
	//
	// cuts off the results at MAX_RESULTS

	std::map<std::string, std::vector<std::string> >::iterator it;
	unsigned int curr_starting_pos = 0;

	for (unsigned int i = 0; i < extensions->size() && clean->size() < MAX_RESULTS; i++) {

		for (it = messy->begin(); it != messy->end(); it++) {
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

				if (!(*extensions)[i].compare("*") || !(*extensions)[i].compare(currPathExtension)) {
					currPathVector.push_back(path);
					for (unsigned int i = 0; i < (*messy)[path].size(); i++) {
						currPathVector.push_back((*messy)[path][i]);
					}
				}
				if (currPathVector.size()) {
					clean->push_back(currPathVector);
				}
				if (clean->size() == MAX_RESULTS) {
					break;
				}
			}
		}

		std::vector<std::vector<std::string> >::iterator vecIt = clean->begin();
		unsigned int j = 0;
		while (curr_starting_pos != 0 && j < (curr_starting_pos + 1)) {
			vecIt++;
			j++;
		}
		sort(vecIt, clean->end(), compareVectorsBySize);
		curr_starting_pos = clean->size() - 1;
	}

	return true;
}

void Clerk::speakUp(const std::vector<std::vector<std::string> > *verdict) {
	for (unsigned int i = 0; i < verdict->size(); i++) {
		const char *path = (*verdict)[i][0].c_str();
		char pathStr[strlen(path) + strlen(RED) + 1];
		strcpy(pathStr, RED);
		printf(strcat(pathStr, "%s:\n"), path);
		for (unsigned int j = 1; j < (*verdict)[i].size(); j++) {
			const char *occurance = (*verdict)[i][j].c_str();
			char occStr[strlen(occurance) + strlen(BLUE) + 1];
			strcpy(occStr, BLUE);
			printf(strcat(occStr, "\t\t%s\n"), (*verdict)[i][j].c_str());
		}
	}
	printf(RESET_COLOR);
}
