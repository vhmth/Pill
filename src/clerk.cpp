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
#include <locale>

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
				if (path.c_str()[path.size() - 1] != '\0') {
					char strtemp[path.size() + 1];
					for (unsigned k = 0; k < path.size(); k++) {
						strtemp[k] = path.at(k);
					}
					strtemp[path.size()] = '\0';
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

void Clerk::speakUp(
	const std::vector<std::vector<std::string> > *verdict,
	const char *query
) {
	size_t query_len = strlen(query);
	// case insensitive querying
	std::locale settings;
	std::string ciQuery;
	for (unsigned int i = 0; i < query_len; i++) {
		ciQuery += (std::toupper(query[i], settings));
	}

	for (unsigned int i = 0; i < verdict->size(); i++) {
		const char *path = (*verdict)[i][0].c_str();
		printf(RED "%d.) %s\n", i + 1, path);
		for (unsigned int j = 1; j < (*verdict)[i].size(); j++) {
			// find out number of occurances and their positions
			const char *occ = (*verdict)[i][j].c_str();
			std::string ciOccLine;
			size_t occLineSize = strlen(occ);
			for (unsigned int k = 0; k < occLineSize; k++) {
				ciOccLine += (std::toupper(occ[k], settings));
			}

			char occLine[strlen(occ) + 1];
			strcpy(occLine, occ);

			std::vector<size_t> occPositions;
			size_t occPos = ciOccLine.find(ciQuery, 0);
			while (occPos != std::string::npos) {
				occPositions.push_back(occPos);
				occPos = ciOccLine.find(ciQuery, occPos + 1);
			}

			size_t numOccs = occPositions.size();

			printf("\t\t");
			unsigned int currIndex = 0;
			unsigned int k = 0;
			while (currIndex < occLineSize) {
				if (k < numOccs && occPositions[k] == currIndex) {
					char queryBuf[query_len + 1];
					memcpy(
						queryBuf,
						&occ[currIndex],
						query_len
					);
					queryBuf[query_len] = '\0';
					printf(RED "%s", queryBuf);
					currIndex += query_len;
					k++;
				} else {
					size_t lineLen = occLineSize - currIndex;
					if (k < numOccs) {
						lineLen = occPositions[k] - currIndex;
					}
					char lineBuf[lineLen + 1];
					memcpy(
						lineBuf,
						&occ[currIndex],
						lineLen
					);
					lineBuf[lineLen] = '\0';
					printf(BLUE "%s", lineBuf);
					currIndex += lineLen;
				}
			}
			printf("\n");
		}
	}
	printf(RESET_COLOR);
}
