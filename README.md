# OBJParser
OBJ File Parser Powered by Boost Spirit

## Brief

OBJParser is a fast, efficient, and open-source modern C++ parser for the OBJ and MTL file formats. It supports fully both file standards (OBJ, MTL) as well as unofficial, but widely adopted, optional statements.

It has a dependency of Boost C++, specifically the Spirit library. Spirit itself is a header-only library and so no external binaries are required. There is also optional support for the Boost Iostreams library to make use of memory-mapped files. Employing memory-mapped files can result in performance improvements, but necessitates the need for the Iostreams binary.

More information of dependencies and other topics may be found in the documentation.

## Website

Find more information and downloads at [http://www.vertexfragment.com/objparser/index.html](http://www.vertexfragment.com/objparser/index.html)
