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

#ifndef __SCOUT_H__
#define __SCOUT_H__

#include <string>
#include <vector>
#include <map>

class Scout {
	public:
		Scout();
		~Scout();
		bool initialize(std::string cmd_role);

		std::vector<std::string> getDefaultRole();
		std::vector<std::string> getRole(std::string role);
		std::vector<std::string> getRoleNames();

		unsigned int getMaxResults();
		std::string getRoot();
		std::string getEditor();
	private:
		unsigned int _max;
		std::string _root;
		std::string _editor;
		std::map<std::string, std::vector<std::string> > _roles;
		std::string _defaultRole;

		bool reportJsonParseError(std::string err);
};

#endif
