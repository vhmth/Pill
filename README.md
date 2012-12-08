Pill
====

A convenience wrapper around grep which allows you to easily pillage your code base.

This tool is still under heavy development and will be moved to [The Backplane GitHub](https://github.com/Backplane) once it is stable.

Installation
============

In the top-level Pill directory (the one you found this README in),
you simply need to type:

    sudo sh install.sh RC_FILE_PATH_HERE

substituting RC_FILE_PATH_HERE with the path to your shell's configuration
file. If you're using Bash, this is typically ~/.bashrc. If you're using zsh,
this is typically ~/.zshrc.

Flags
=====

For a list of flags that can be used as options for Pill on the command
line, type:

    pill --help

Customization
=============

You may customize Pill via the options.json and roles.json files in
the 'options' folder. The format of these objects is listed below. Note
that, although you need not have all the fileds filled (or any at all)
in either of these JSON files, the objects themselves must be defined
as at least an empty object literal ({}).

1.) options.json:

    {
        "role":         DEFAULT_ROLE_STRING, (defaults to "")
        "max_results":  DEFAULT_NUM_RESULTS_RETURNED, (defaults to 25)
        "root":         DEFAULT_PATH_STRING_FOR_SEARCH, (defaults to "./")
        "editor":       DEFAULT_EDITOR_FOR_OPENING_FILES_FROM_SEARCH (defaults to "vim")
    }

An example may be:

    {
        "role":           "frontend",
        "max_results":    100,
        "root":           "/Users/me/Desktop/Code",
        "editor":         "nano" // lol I know, good joke
    }

2.) roles.json:

This file associates an string value to an array of strings which each
denote a file extension. The array should be in order of greatest to least
importance of file type results returned from Pill.

    {
        "SOME_STRING_KEY_DEFINING_A_ROLE": ARRAY_OF_FILE_EXTENSIONS,
        "SOME_OTHER_ROLE":                 ANOTHER_ARRAY_OF_FILE_EXTENSION_STRINGS
    }

An example might be:

    {
        "frontend": ["js", "html", "css"],
        "backend":  ["php", "py", "java"]
    }

Note that, if command line flags are passed in, those will take precedence
over default options in options.json. If an option in options.json is not
present and a command line flag has not been defined for that option, the
default for that specific option detailed above will be used.

Uninstallation
==============

Again, in the top-level Pill directory (the one you found this README in),
you simply need to type:

    sudo sh uninstall.sh RC_FILE_PATH_HERE

where RC_FILE_PATH_HERE is the file path to the same shell configuration
file you supplied to install.sh.

Future Plans
============

* Allow for grep flags (i, r, etc.).
* Allow for file name search (ls -a | grep).
* Allow for specific file extension search from command line (no need for role to be declared).
* Allow for specifying color switch off on output.
* Allow for no auto-open shell (only display).
* Allow for specifying file path and occurance colors in options.json.
* Flags should be taken in first and then the query (less awkward).
