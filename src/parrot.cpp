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
#include <stdio.h>

#include "include/parrot.h"

// ~ FOR TRIMMING ~
static inline std::string &ltrim(std::string &s) {
	s.erase(
		s.begin(),
		std::find_if(
			s.begin(),
			s.end(),
			std::not1(
				std::ptr_fun<int, int>(std::isspace)
			)
		)
	);
	return s;
}

static inline std::string &rtrim(std::string &s) {
	s.erase(
		std::find_if(
			s.rbegin(),
			s.rend(),
			std::not1(
				std::ptr_fun<int, int>(std::isspace)
			)
		).base(),
		s.end()
	);
	return s;
}

static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}
// ~ FOR TRIMMING ~

bool Parrot::listenAndSquawk(
	const std::string cmd,
	std::map<std::string, std::vector<std::string> > *_results) {

	const unsigned int MAX_BUFF_SIZE = 500;

	FILE *stream = popen(cmd.c_str(), "r");
	char res_buffer[MAX_BUFF_SIZE];

	if (stream == NULL) {
		printf("\nParrot: Could not open a file stream with popen.\n");
	}

	while (fgets(res_buffer, MAX_BUFF_SIZE, stream) != NULL) {
		std::string curr_buff(res_buffer);
		size_t wallpos = curr_buff.find(":");
		if (wallpos != std::string::npos) {
			std::string path(curr_buff.substr(0, wallpos));
			std::string occurance(curr_buff.substr(wallpos + 1));
			occurance = trim(occurance);
			(*_results)[path].push_back(occurance);
		}
	}

	if (pclose(stream) == -1) {
		printf("\nParrot: Error closing file stream.\n");
		return false;
	}

	return true;
}
