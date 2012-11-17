/*
 * Copyright (c) 2012 The Backplane Incorporated,
 *     Vinay Hiremath,
 *     Zach Tratar
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

#include <string.h>
#include "loot.h"

using namespace std;

Loot::Loot(string dir, unsigned int freq) :
	_dir(new string(&dir)), _freq(freq) {}

Loot::Loot(Loot const & source) {
	_dir = new string(&source._dir);
	_freq = source._freq;
}

Loot const & Loot::operator=(Loot const & source) {
	if (this != source) {
		delete _dir;
		_dir = new string(&source._dir);
		_freq = source._freq;
	}
	return *this;
}

Loot::~Loot() {
	delete _dir;
	_dir = NULL;
}

string Loot::spill() {
	return _dir + " : " + _freq;
}
