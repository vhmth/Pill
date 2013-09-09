Pill
====

A convenience wrapper around grep which allows you to easily pillage your code base.

Installation
============

In the top-level Pill directory (the one you found this README in),
you simply need to type:

    sudo sh install.sh RC_FILE_PATH_HERE

substituting RC_FILE_PATH_HERE with the path to your shell's configuration
file. If you're using Bash, this is typically ~/.bashrc. If you're using zsh,
this is typically ~/.zshrc.

Updating
========

Since Pill will be heavily improved upon, updates will be roled out. To update Pill,
simply run:

    sh update.sh

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
        "editor":       DEFAULT_EDITOR_FOR_OPENING_FILES_FROM_SEARCH (defaults to nothing)
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

3.) The 'editor' flag:

Pill comes equipped with a built-in subshell that allows you to open
its results directly from command line. This flag specifies the editor
which the subshell will use to open a result returned by Pill. If the
editor option in options.json is not present nor is an editor passed
in via the "--editor" flag when calling Pill, the quick-open subshell
will not open, but the results returned by Pill will still be displayed.
If you have specified the "editor" field in options.json but wish to
only see the results and not fire this subshell, passing 'none' to the
command line "--editor" flag will ensure this behavior.

Uninstallation
==============

Again, in the top-level Pill directory (the one you found this README in),
you simply need to type:

    sudo sh uninstall.sh RC_FILE_PATH_HERE

where RC_FILE_PATH_HERE is the file path to the same shell configuration
file you supplied to install.sh.
