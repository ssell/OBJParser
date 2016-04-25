/**
 * Copyright 2016 Steven T Sell (ssell@ocularinteractive.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __H__OBJ_PARSER__H__
#define __H__OBJ_PARSER__H__

#include "OBJState.hpp"

//------------------------------------------------------------------------------------------
// Optional Defines
//------------------------------------------------------------------------------------------

// If defined, the parser will use boost memory mapped files for loading files.
// This may drastically speed up file I/O but requires both the Boost System and IOStream library dependencies.
// System is used to calculate the needed memory for the mapping, while IOStream contains the memory map implementation.

#define OBJ_PARSER_USE_MEM_MAP

//------------------------------------------------------------------------------------------
// OBJ Parser
//------------------------------------------------------------------------------------------

/**
 * \class OBJParser
 */
class OBJParser
{
public:

    OBJParser();
    ~OBJParser();

    bool parseString(std::string const& str);
    bool parseFile(std::string const& path);

    OBJState* getOBJState();

protected:

    bool parseFilefstream(std::string const& path);
    bool parseFileMemMap(std::string const& path);
   
    OBJState m_OBJState;

private:
};

//------------------------------------------------------------------------------------------

#endif