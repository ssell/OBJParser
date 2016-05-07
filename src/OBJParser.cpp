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

#include "OBJParser.hpp"
#include "OBJGrammar.hpp"
#include "MTLGrammar.hpp"

#ifdef OBJ_PARSER_USE_MEM_MAP
#include <boost/iostreams/device/mapped_file.hpp>
#else
#include <fstream>
#endif

//------------------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------------------

OBJParser::OBJParser()
    : m_LastError("No Error")
{

}

OBJParser::~OBJParser()
{

}

//------------------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------------------

OBJParser::Result OBJParser::parseOBJString(std::string const& str)
{
    OBJParser::Result result = OBJParser::Result::Success;

    using Iterator = decltype(str.begin());
    
    m_OBJState.clearState();

    OBJGrammar<Iterator> grammar(&m_OBJState);
    OBJCommentSkipper<Iterator> skipper;

    if(!qi::phrase_parse(str.begin(), str.end(), grammar, skipper))
    {
        result = OBJParser::Result::FailedOBJParseError;
    }

    return result;
}

OBJParser::Result OBJParser::parseOBJFile(std::string const& path)
{
    OBJParser::Result result = OBJParser::Result::Success;

    m_OBJState.clearState();

#ifdef OBJ_PARSER_USE_MEM_MAP
    result = parseOBJFileMemMap(path);
#else
    result = parseOBJFilefstream(path);
#endif

    return result;
}

OBJState* OBJParser::getOBJState()
{
    return &m_OBJState;
}

std::string const& OBJParser::getLastError() const
{
    return m_LastError;
}

//------------------------------------------------------------------------------------------
// Protected Methods
//------------------------------------------------------------------------------------------

OBJParser::Result OBJParser::parseOBJFilefstream(std::string const& path)
{
    OBJParser::Result result = OBJParser::Result::Success;

    //--------------------------------------------------------------------
    // Parse the OBJ file
    //--------------------------------------------------------------------

    std::ifstream stream(path.c_str());

    if(stream.is_open())
    {
        stream >> std::noskipws;

        using Iterator = boost::spirit::istream_iterator;

        Iterator first(stream);
        Iterator last;

        OBJGrammar<Iterator> grammar(&m_OBJState);
        OBJCommentSkipper<Iterator> skipper;

        if(qi::phrase_parse(first, last, grammar, skipper))
        {
            if(first != last)
            {
                result = OBJParser::Result::FailedOBJParseError;
                m_LastError = "Failed to parse line '" + extractLastLine(stream) + "' in file '" + path + "'";
            }
        }
        else
        {
            result = OBJParser::Result::FailedOBJParseError;
            m_LastError = "Failed to parse line '" + extractLastLine(stream) + "' in file '" + path + "'";
        }

        stream.close();
    }
    else
    {
        result = OBJParser::Result::FailedOBJFileRead;
        m_LastError = "Failed to open file '" + path + "'";
    }

    //--------------------------------------------------------------------
    // Parse the MTL file (if any specified)
    //--------------------------------------------------------------------

    if(result == OBJParser::Result::Success)
    {
        auto materialLibraries = m_OBJState.getMaterialLibraries();

        for(auto mtlPath : *materialLibraries)
        {
            result = parseMTLFilefstream(buildRelativeMTLPath(path, mtlPath));

            if(result != OBJParser::Result::Success)
            {
                break;
            }
        }
    }

    return result;
}

OBJParser::Result OBJParser::parseMTLFilefstream(std::string const& path)
{
    OBJParser::Result result = OBJParser::Result::Success;

    std::ifstream stream(path.c_str());

    if(stream.is_open())
    {
        stream >> std::noskipws;

        using Iterator = boost::spirit::istream_iterator;

        Iterator first(stream);
        Iterator last;

        MTLGrammar<Iterator> grammar(&m_OBJState);
        MTLCommentSkipper<Iterator> skipper;

        if(qi::phrase_parse(first, last, grammar, skipper))
        {
            if(first == last)
            {
                grammar.finishCurrentMaterial();
            }
            else
            {
                result = OBJParser::Result::FailedOBJParseError;
                m_LastError = "Failed to parse line '" + extractLastLine(stream) + "' in file '" + path + "'";
            }
        }
        else
        {
            result = OBJParser::Result::FailedOBJParseError;
            m_LastError = "Failed to parse line '" + extractLastLine(stream) + "' in file '" + path + "'";
        }

        stream.close();
    }
    else
    {
        result = OBJParser::Result::FailedMTLFileRead;
        m_LastError = "Failed to open file '" + path + "'";
    }

    return result;
}


OBJParser::Result OBJParser::parseOBJFileMemMap(std::string const& path)
{
    OBJParser::Result result = OBJParser::Result::Success;

#ifdef OBJ_PARSER_USE_MEM_MAP

    //--------------------------------------------------------------------
    // Parse the OBJ file
    //--------------------------------------------------------------------

    boost::iostreams::mapped_file mappedFile(path, boost::iostreams::mapped_file::readonly);

    if(mappedFile.is_open())
    {
        auto first = mappedFile.const_data();
        auto last = first + mappedFile.size();

        using Iterator = decltype(first);
        OBJGrammar<Iterator> grammar(&m_OBJState);
        OBJCommentSkipper<Iterator> skipper;

        if(qi::phrase_parse(first, last, grammar, skipper))
        {
            if(first != last)
            {
                result = OBJParser::Result::FailedOBJParseError;
                m_LastError = "Failed to parse line '" + extractLastLine(first) + "' in file '" + path + "'";
            }
        }
        else
        {
            result = OBJParser::Result::FailedOBJParseError;
            m_LastError = "Failed to parse line '" + extractLastLine(first) + "' in file '" + path + "'";
        }

        mappedFile.close();
    }
    else
    {
        result = OBJParser::Result::FailedOBJFileRead;
        m_LastError = "Failed to open file '" + path + "'";
    }

    //--------------------------------------------------------------------
    // Parse the MTL file (if any specified)
    //--------------------------------------------------------------------

    if(result == OBJParser::Result::Success)
    {
        auto materialLibraries = m_OBJState.getMaterialLibraries();

        for(auto mtlPath : *materialLibraries)
        {
            result = parseMTLFileMemMap(buildRelativeMTLPath(path, mtlPath));

            if(result != OBJParser::Result::Success)
            {
                break;
            }
        }
    }
#endif

    return result;
}

OBJParser::Result OBJParser::parseMTLFileMemMap(std::string const& path)
{
    OBJParser::Result result = OBJParser::Result::Success;

#ifdef OBJ_PARSER_USE_MEM_MAP

    boost::iostreams::mapped_file mappedFile(path, boost::iostreams::mapped_file::readonly);

    if(mappedFile.is_open())
    {
        auto first = mappedFile.const_data();
        auto last = first + mappedFile.size();

        using Iterator = decltype(first);
        MTLGrammar<Iterator> grammar(&m_OBJState);
        MTLCommentSkipper<Iterator> skipper;

        if(qi::phrase_parse(first, last, grammar, skipper))
        {
            if(first == last)
            {
                grammar.finishCurrentMaterial();
            }
            else
            {
                result = OBJParser::Result::FailedOBJParseError;
                m_LastError = "Failed to parse line '" + extractLastLine(first) + "' in file '" + path + "'";
            }
        }
        else
        {
            result = OBJParser::Result::FailedOBJParseError;
            m_LastError = "Failed to parse line '" + extractLastLine(first) + "' in file '" + path + "'";
        }

        mappedFile.close();
    }
    else
    {
        result = OBJParser::Result::FailedMTLFileRead;
        m_LastError = "Failed to open file '" + path + "'";
    }

#endif

    return result;
}

std::string OBJParser::buildRelativeMTLPath(std::string const& objPath, std::string const& mtlPath)
{
    auto find = objPath.find_last_of('/');

    if(find != std::string::npos)
    {
        find += 1;
    }
    else
    {
        find = objPath.find_last_of('\\');

        if(find != std::string::npos)
        {
            find += 1;
        }
        else
        {
            find = objPath.size();
        }
    }

    const std::string objDir = objPath.substr(0, find);
    const std::string result = objDir + mtlPath;

    return result;
}

std::string OBJParser::extractLastLine(const char* str)
{
    // Extract the next line and remove any pesky carriage returns

    std::string result = str;
    auto find = result.find_first_of('\n');

    if(find != std::string::npos)
    {
        result = result.substr(0, find);
        result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
    }

    return result;
}

std::string OBJParser::extractLastLine(std::ifstream& stream)
{
    // Seek backwards to the last newline (or file start) and then read in the following line

    std::string result;

    const uint32_t start = static_cast<uint32_t>(stream.tellg());
    uint32_t current = start;

    char c = ' ';

    while((current > 0) && (c != '\n'))
    {
        stream.seekg(--current, stream.beg);
        stream.get(c);
    }

    std::getline(stream, result);
    
    return result;
}

//------------------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------------------