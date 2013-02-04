========================================================================
    CONSOLE APPLICATION : Sync Project Overview
========================================================================

This file contains a summary of what you will find in each of the files that
make up your Sync application.


Sync.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

Sync.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

include/*
	Header files, interface specifications.

src/list.c
	Implementation of a list data structure.
src/set.c
	Implementation of a Set data structure. Uses a hash table for internals.
src/util.c
	Various utility functions and macros.
src/playlist.c
	Implementation file for a playlist representation.
src/Sync.c
	Implementation file for synching to devices.
src/main.c
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:


/////////////////////////////////////////////////////////////////////////////
Other notes:

TODO:
Add a CLI option for setting the target (in disk space) after which sync should stop transferring files to a device.
Add support for other playlist file types.
Document the interfaces.

/////////////////////////////////////////////////////////////////////////////
