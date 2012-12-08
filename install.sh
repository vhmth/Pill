# Copyright (c) 2012 The Backplane Incorporated,
#     Vinay Hiremath
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify,
# merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished
# to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
# ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#!/usr/bin/env bash

PILL_INSTALL_GREEN="\033[1;32m"
PILL_INSTALL_RED="\033[1;31m"

if [ -z "$1" ]
then
	echo $PILL_INSTALL_RED "$(tput bold)No shell configurations file (such as ~/.bashrc or ~/.zshrc) supplied."
else
	make

	PWD = "$(pwd)"

	ln -s $PWD /
	echo "Added a symlink to this directory in your root directory."

	echo 'export PATH=$PATH:/Pill/bin' > export.txt
	cp $1 $1.bak
	cat export.txt >> $1
	echo "Modified $1 to add Pill export to path. A backup of $1 was saved at $1.bak"
	rm export.txt

	echo $PILL_INSTALL_GREEN "\n\n$(tput bold)Pill has been installed. Please close out of this terminal window and open a new one to begin using it."
fi
