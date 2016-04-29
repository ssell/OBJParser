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

#ifndef __H__OBJ_PARSER_MTL_GRAMMAR__H__
#define __H__OBJ_PARSER_MTL_GRAMMAR__H__

#define BOOST_SPIRIT_USE_PHOENIX_V3

#pragma warning (disable:4348)

#include "OBJState.hpp"
#include "OBJGrammarSkipper.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;

//------------------------------------------------------------------------------------------

/**
 * \class MTLGrammar
 *
 * Spirit grammar for parsing MTL format files.
 *
 * Based on the format specification at:
 * http://www.paulbourke.net/dataformats/mtl/
 */
template<typename Iterator, typename Skipper = OBJCommentSkipper<Iterator>>
class MTLGrammar : public qi::grammar<Iterator, Skipper>
{
public:

    MTLGrammar(OBJState* state)
        : OBJGrammar::base_type(ruleStart),
          m_pOBJState(state)
    {
        setupGeneralRules();
        setupNewMaterialRules();
        setupColorIlluminationRules();
        setupTextureMapRules();
        setupReflectionMapRules();

        ruleStart = (ruleNewMaterial |
                     ruleColorIllumination |
                     ruleTextureMap |
                     ruleReflectionMap);
    }

protected:

    //--------------------------------------------------------------------------------------
    // Rule Setup Methods
    //--------------------------------------------------------------------------------------

    void setupGeneralRules()
    {
        ruleString = *(qi::char_ - qi::space);
    }

    void setupNewMaterialRules()
    {
        ruleNewMaterial =
            qi::lit("newmtl ") >>
            ruleString >>
            qi::eol;
    }

    void setupColorIlluminationRules()
    {

    }

    void setupTextureMapRules()
    {

    }

    void setupReflectionMapRules()
    {

    }

    //--------------------------------------------------------------------------------------
    // Member Variables
    //--------------------------------------------------------------------------------------
    
    //--------------------------------------------------------------------
    // Top-Level Rules

    qi::rule<Iterator, Skipper> ruleStart;
    qi::rule<Iterator, Skipper> ruleNewMaterial;
    qi::rule<Iterator, Skipper> ruleColorIllumination;
    qi::rule<Iterator, Skipper> ruleTextureMap;
    qi::rule<Iterator, Skipper> ruleReflectionMap;

    //--------------------------------------------------------------------
    // General Rules

    qi::rule<Iterator, std::string(), Skipper> ruleString;

    //--------------------------------------------------------------------
    // Color / Illumination Rules

    //--------------------------------------------------------------------
    // Texture Map Rules

    //--------------------------------------------------------------------
    // Reflection Map Rules

    //--------------------------------------------------------------------
    // Non-Rule Members

    OBJState* m_pOBJState;

private:

};

//------------------------------------------------------------------------------------------

#endif