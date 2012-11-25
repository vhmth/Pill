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
#include <iostream>
#include <fstream>
#include <json/json.h>

#include "include/scout.h"

Scout::Scout() {}

bool Scout::initialize(std::string cmd_role){
	Json::Value json_root;
	Json::Reader json_reader;
	std::ifstream file_stream("options/options.json", std::ifstream::binary);

	if (!json_reader.parse(file_stream, json_root, false)) {
		return reportJsonParseError(json_reader.getFormattedErrorMessages());
	}

	if (json_root.isMember("max_results")) {
		_max = json_root.get("max_results", "UTF-8").asUInt();
	} else {
		_max = 25;
	}

	if (json_root.isMember("editor")) {
		_editor = json_root.get("editor", "UTF-8").asString();
	} else {
		_editor = "vim";
	}

	if (json_root.isMember("root")) {
		_root = json_root.get("root", "UTF-8").asString();
	} else {
		_root = "./";
	}

	if (json_root.isMember("role")) {
		_defaultRole = json_root.get("role", "UTF-8").asString();
	} else {
		_defaultRole = "";
	}

	file_stream.close();
	file_stream.open("options/roles.json");

	if (!json_reader.parse(file_stream, json_root, false)) {
		return reportJsonParseError(json_reader.getFormattedErrorMessages());
	}

	if (cmd_role.compare("") && !json_root.isMember(cmd_role)) {
		printf("%s does not exist in roles.json\n", cmd_role.c_str());
		return false;
	}
	if (_defaultRole.compare("") && !json_root.isMember(_defaultRole)) {
		printf("%s does not exist in roles.json. Please", _defaultRole.c_str());
		printf(" make changes to options.json to include");
		printf(" a role that exists in roles.json or remove the");
		printf(" 'role' field.\n");
		return false;
	}

	for (Json::ValueIterator key_it = json_root.begin(); key_it != json_root.end(); key_it++) {
		std::string key = key_it.key().asString();
		for (unsigned int i = 0; i < json_root[key].size(); i++) {
			_roles[key].push_back(json_root[key][i].asString());
		}
	}

	return true;
}

Scout::~Scout() {}


std::vector<std::string> Scout::getDefaultRole() {
	if (_defaultRole.compare("")) {
		return _roles[_defaultRole];
	} else {
		return std::vector<std::string>(1, "*");
	}
}


std::vector<std::string> Scout::getRole(std::string role) {
	return _roles[role];
}

std::vector<std::string> Scout::getRoleNames() {
	std::vector<std::string> roles;
	std::map<std::string, std::vector<std::string> >::iterator it;

	for (it = _roles.begin(); it != _roles.end(); it++) {
		roles.push_back((*it).first);
	}

	return roles;
}

unsigned int Scout::getMaxResults() {
	return _max;
}

std::string Scout::getRoot() {
	return _root;
}

std::string Scout::getEditor() {
	return _editor;
}

bool Scout::reportJsonParseError(std::string err) {
	printf("There was an error parsing options.json:\n\n");
	printf("%s", err.c_str());
	return false;
}
