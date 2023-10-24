========================
What are TagVFS's goals?
========================

At a high level:

- Support organizing files by "tags"
- Provide FUSE filesystem for presenting a normal filesystem to the rest of the
  OS (and therefore any other programs that can interact with on-disk data)
- Multi-platform client support
- Store arbitrary files metadata
- Do my best at preventing duplicate file storage


Anti-goals
----------

This program *avoids* attempting to do the following:

* Re-invent photo formats

  * No messing with JPEG/PNG/NEF etc, or creating anything directly comparable
    to them

* Re-invent metadata formats

  * No altering the syntax of XML, YAML, or creating anything directly
    comparable to them
  * As much as possible, don't reinvent a parser for such a language (i.e., if
    interacting with XML data, use a preexisting library for XML rather than
    rolling a new one)

* Re-invent compression algorithms

  * No altering the process of existing compression algorithms (zip, gzip,
    rar, the like) or creating new ones

Requirements
------------

1. Define a "wrapper" format around an arbitrary file to store arbitrary metadata

   1. This format shall allow storage of any file, regardless of extension/type
      or contents
   2. This format shall allow storage of metadata pertinent to the "wrapped"
      file

      1. This format shall allow for storage of any amount of metadata
      2. This format shall allow for storage of any data type of metadata
         (e.g., strings, numbers, etc)
      3. This format shall allow for structured metadata; i.e., hierarchies of
         data (e.g., the ``date`` tag may have ``year``, ``month``, and
         ``day`` tags)

   3. This format shall facilitate efficient creation, parsing, and editing by
      programmatic means
   4. This format should facilitate creation, parsing, and editing by manual
      means (this is secondary to programmatic efficiency, and does **not**
      say the format should be *easy* to edit for "average Joe")
   5. This format shall allow, at user discretion, compression of both metadata
      and "wrapped" file contents

      1. The compression algorithms, if enabled, shall be individually
         selectable for the metadata and wrapped file content sections
      2. The compression algorithms, if enabled, shall be recorded within the
         wrapper file itself

         1. If no compression is enabled, this shall be recorded within the
            wrapper file itself (by indicating the compression
            algorithm is ``None``).

2. Access a storage array containing "wrapper" files

   1. The program shall support discovery of wrapper files in a
      storage-medium-agnostic manner

3. Present a mountable, FUSE-driven, layered filesystem

   1. The program shall utilize the filesystem-in-userspace (FUSE) interface
   2. The program shall present "views" of the storage array of wrapper files

      1. The program shall define a "view" for each metadata tag available in
         any wrapper file
      2. The program's view structure shall mirror that of the metadata
         structure (e.g., the ``date`` folder view may have ``year``,
         ``month``, and ``day`` sub-folder views)

4. Present an API that supports access to all capabilities of the program

   1. The API shall be usable from the C programming language

      1. The C API shall make available to the application all functions
         declared by the program, regardless of intended use (no *truly*
         private functions -- only marking as *not recommended for API use*).

   2. The API shall be usable from the Python 3 programming language

      1. As much as possible, the Python 3 API must utilize the C
         implementation for its operations
      2. The Python 3 API shall expose all the *capabilities* of the C library
         (i.e., if you can do it with the C API, you can do it with the
         Python API).

         1. The Python 3 API may opt to not expose *every single function* of
            the C library, insofar as the capabilities the function
            provides are provided elsewhere (i.e., if you can do it
            with the C API, you *can* do it with the Python API, but
            it may not be the same function calls).

