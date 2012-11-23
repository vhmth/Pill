Pill
====

A convenience wrapper around grep which allows you to easily pillage your code base.

This tool is still under heavy development and will be moved to [The Backplane GitHub](https://github.com/Backplane) once it is stable.

Installation
============

1.) In the top-level Pill directory (the one you found this README in),
you simply need to type 'make' in the command prompt. This will compile
Pill as well as its dependency (jsoncpp) and place the pill executable
in the 'bin' folder.

Note: If you would like access to the intermediate object files for
debugging with gdb, type 'make debug', and they will be waiting for
you in the 'bin' folder.

2.) After compiling the Pill executable, you will want to export it to
your path for easy command-line use. You can do this by typing the
following in the command prompt:

    export PATH="$PATH/path/to/Pill/bin/"
