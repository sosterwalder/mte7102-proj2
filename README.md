# mte7102-proj2
## QCE: Proof of concept - Project work for module MTE-7102

A project work aiming at developping proof of concepts for various components for
being able to interactively set up and render scenes using sphere tracing.

This project work is getting developed within the scope of the module MTE-7102
- 'Vertiefungsprojekt 2' at the University of Applied Science in Bern
and is supervised by Professor Jean-Claude Fuhrer. Details see
http://www.ti.bfh.ch/fileadmin/modules/MTE7102-de.xml

It is written and developed in the spring semester 2016 and is licensed
under the Creative Commons Attribution-ShareAlike 3.0 respectively 
unter the conditions of the MIT License (MIT).

Directory Structure (in near future)
------------------------------------

    bin/       -- Compiled, binary file of the prototype
    build/     -- Temporary directory for build output
    doc/       -- Top folder containing all documentation
        abstract/  -- A short abstract of the project
        doc/       -- The actual documentation
            img/       -- Images used for the documentation
            inc/       -- LaTeX files used for inclusion to maintain
                          readability and managabilty
                static/    -- Static files used for inclusion, e.g. the
                              bibliography, versioning of the document
                              and so on
                    attachment/   -- Attachments for the documentation,
                                     e.g. the minutes of the held meetings
            uml/       -- Source files as well as output files for all
                          UML related documentation data. Mostly Eclipse
                          Papyrus compatible files
    inc/       -- External source files for inclusion, needed by the
                  prototype(s)
    lib/       -- External libraries for linking against when building
                  the prototype(s)
    resources/ -- Various resources needed for building the binary,
        data/      -- Various data files, e.g. GLSL shader files
    src/       -- Source code of the prototype(s)
    xcode/     -- Dynamically generated project file for Apple Xcode


### Build and run

To get the project set up and to run it, checkout this repository, switch to
its directory and call *make build run*:

```bash
$ git clone https://github.com/sosterwalder/mte7102-proj2.git
$ cd mte7102-proj2
mte7102-proj2 $ make build run
```

As the project has quite a lot of dependencies the fetching of the submodules
may take a while (especially if you are on a 3G dial up connection as I am..
hehe).

This process was tested on Arch linux (kernel 4.5.0-1) as well as Apple Mac OS
X (10.11.4). It does not work on Microsoft Windows yet.
