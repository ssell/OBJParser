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
{

}

OBJParser::~OBJParser()
{

}

//------------------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------------------

bool OBJParser::parseString(std::string const& str)
{
    using Iterator = decltype(str.begin());
    
    m_OBJState.clearState();

    OBJGrammar<Iterator> grammar(&m_OBJState);
    OBJCommentSkipper<Iterator> skipper;

    return qi::phrase_parse(str.begin(), str.end(), grammar, skipper);
}

bool OBJParser::parseFile(std::string const& path)
{
    bool result = false;

    m_OBJState.clearState();

#ifdef OBJ_PARSER_USE_MEM_MAP
    result = parseFileMemMap(path);
#else
    result = parseFilefstream(path);
#endif

    return result;
}

OBJState* OBJParser::getOBJState()
{
    return &m_OBJState;
}

//------------------------------------------------------------------------------------------
// Protected Methods
//------------------------------------------------------------------------------------------

bool OBJParser::parseFilefstream(std::string const& path)
{
    bool result = false;

    return result;
}

bool OBJParser::parseFileMemMap(std::string const& path)
{
    bool result = false;

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

        result = qi::phrase_parse(first, last, grammar, skipper);

        if(result)
        {
            result = (first == last);
        }

        mappedFile.close();
    }

    //--------------------------------------------------------------------
    // Parse the MTL file (if any specified)
    //--------------------------------------------------------------------

    if(mappedFile.is_open())
    {
        auto first = mappedFile.const_data();
        auto last = first + mappedFile.size();

        using Iterator = decltype(first);
        MTLGrammar<Iterator> grammar(&m_OBJState);
        MTLCommentSkipper<Iterator> skipper;

        result = qi::phrase_parse(first, last, grammar, skipper);

        if(result)
        {
            result = (first == last);
        }

        mappedFile.close();
    }

#endif

    return result;
}

//------------------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------------------