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
// This may drastically speed up file I/O but requires IOStream library dependency.

//#define OBJ_PARSER_USE_MEM_MAP

//------------------------------------------------------------------------------------------
// OBJ Parser
//------------------------------------------------------------------------------------------

/**
 * \class OBJParser
 *
 * Parser for OBJ and MTL format files.
 *
 * If OBJ_PARSER_USE_MEM_MAP is defined, then files will be read in using memory-mapped files. <br/>
 * Otherwise, files will be read in using the standard filestream.
 *
 * Upon parsing, the resulting OBJ and MTL data is stored within the internal OBJState object.
 * This state is automatically cleared prior to any parsing actions.
 */
class OBJParser
{
public:

    /**
     * \enum Result
     */
    enum class Result
    {
        Success = 0,              ///< No errors encountered during parsing
        FailedOBJFileRead,        ///< Failed to open and/or read OBJ file
        FailedOBJParseError,      ///< Error encountered during OBJ parsing, see getLastError
        FailedMTLFileRead,        ///< Failed to open and/or read MTL file
        FailedMTLParseError       ///< Error encountered during MTL parsing, see getLastError
    };

    //--------------------------------------------------------------------

    OBJParser();
    ~OBJParser();

    //--------------------------------------------------------------------

    /**
     * Attempts to parse an OBJ formatted string.
     *
     * If successful, resulting OBJ data will be stored in the member OBJState
     * object which may be accessed via getOBJState().
     *
     * If failed getLastError() may be used to for additional information.
     *
     * \param[in] str OBJ formatted string to parse
     * \return 
     */
    Result parseOBJString(std::string const& str);

    /**
     * Attempts to parse an OBJ formatted file.
     *
     * If successful, resulting OBJ data will be stored in the member OBJState
     * object which may be accessed via getOBJState().
     *
     * If failed getLastError() may be used to for additional information.
     *
     * \param[in] path Relative path to the OBJ file.
     * \return 
     */
    Result parseOBJFile(std::string const& path);

    /**
     * Attempts to parse a MTL formatted file.
     *
     * If successful, resulting materials will be stored in the member OBJState
     * object which may be accessed via getOBJState().
     *
     * If failed getLastError() may be used to for additional information.
     *
     * \param[in] path Relative path to the MTL file.
     * \return 
     */
    Result parseMTLFile(std::string const& path);

    /**
     * Returns a pointer to the internal OBJState object.
     * \note This state is cleared during each parse call (pointer remains valid).
     */
    OBJState* getOBJState();

    /**
     * If enabled, free-form statements will be aniticipated and parsed.
     * Enabling this may cause a decrease in parsing performance.
     *
     * If disabled and a free-form statement is encountered, then parsing
     * will fail. By default, this setting is disabled.
     *
     * \param[in] enable
     */
    void enableFreeFormParsing(bool enable);

    /**
     * \return Human-readable string description of last error encountered.
     */
    std::string const& getLastError() const;

protected:

    Result parseOBJFilefstream(std::string const& path);
    Result parseOBJFileMemMap(std::string const& path);
   
    Result parseMTLFilefstream(std::string const& path);
    Result parseMTLFileMemMap(std::string const& path);

    std::string buildRelativeMTLPath(std::string const& objPath, std::string const& mtlPath);

    std::string extractLastLine(const char* str);
    std::string extractLastLine(std::ifstream& stream);

    //--------------------------------------------------------------------

    OBJState m_OBJState;          ///< Internal OBJ state

    std::string m_LastError;      ///< String representation of last error

    bool m_ParseFreeForm;

private:
};

//------------------------------------------------------------------------------------------

#endif