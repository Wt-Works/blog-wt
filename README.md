Blog example
------------

This Code is not only about a blog, but about C++, wt and a rework.

The main scope is to demonstrate two things:

- a) How simple it can be to develope web applications with C++
- b) The neccessity to rework existent libraries (here wt), in order to simplify them

The code within blog-wt is taken from the wt examples. It is then refactored step by step to become simpler and self-explaining. During the rework, several issues are filed within the wt project, mostly subjecting simplifications of the API's (without breaking existent behaviour).

The final goal is to reduce the entry barriers for any newcomers who like to adopt C++ & wt for their web development.



How to run
----------

Use cmake standard mechanisms to run.

If you don't have and IDE, youu can use qt creator to open the cmakefiles.txt

The example itself will be deployed at '/blog', and the RSS feed at '/blog/feed'. A SQLite3 database will be created in the working directory if it didn't yet exist.



What it illustrates
-------------------

C++

- That C++ code can have beauty and simplicity, without loosing any power.
- That Web Applications can be fast, without big machines.
- That Web Applications can fit in even the smallest devices.

wt

Standalone usage of an wt example

- `Wt::Dbo`   to create a simple database-based model layer
- `Widgets`   how you can use these database objects inside View widgets
- `WTemplate` for recurisvely defining HTML-based views
- `Wt::Auth`  for authentication (including google or other OAuth providers)
- `WResource` for an RSS feed
