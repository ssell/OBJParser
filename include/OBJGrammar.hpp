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

#ifndef __H__OBJ_PARSER_GRAMMAR__H__
#define __H__OBJ_PARSER_GRAMMAR__H__

#define BOOST_SPIRIT_USE_PHOENIX_V3

#pragma warning (disable:4348)

#include "OBJState.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;

//------------------------------------------------------------------------------------------
// OBJ Comment Skipper
//------------------------------------------------------------------------------------------

/**
 * \class OBJCommentSkipper
 * 
 * Grammar defining how to skip OBJ comments.
 * A comment is started with the '#' character and continues until the end of the line.
 */
template<typename Iterator>
class OBJCommentSkipper : public boost::spirit::qi::grammar<Iterator>
{
public:

    OBJCommentSkipper()
        : OBJCommentSkipper::base_type(ruleSkip)
    {
        ruleSkip = (qi::char_('#') >> *(qi::char_ - qi::eol) >> qi::eol);
    }

    boost::spirit::qi::rule<Iterator> ruleSkip;

protected:

private:
};


//------------------------------------------------------------------------------------------
// OBJ Grammar
//------------------------------------------------------------------------------------------

/**
 * \class OBJGrammar
 */
template<typename Iterator, typename Skipper = OBJCommentSkipper<Iterator>>
class OBJGrammar : public qi::grammar<Iterator, Skipper>
{
public:

    OBJGrammar(OBJState* state) 
        : OBJGrammar::base_type(ruleStart),
          m_pOBJState(state)
    {
        //----------------------------------------------------------------
        // Group Grammar
        //----------------------------------------------------------------

        ruleGroupName = *(qi::char_ - ascii::space);
        ruleGroup = 
            qi::lit("g ") [boost::phoenix::bind(&OBJState::clearActiveGroups, m_pOBJState)] >> 
            (ruleGroupName [boost::phoenix::bind(&OBJState::addActiveGroup, m_pOBJState, qi::_1)] % +(ascii::blank)) >> 
            qi::eol;
        
        //----------------------------------------------------------------
        // Data Grammar
        //----------------------------------------------------------------

        ruleVector2Data = qi::skip(qi::blank)[qi::float_ >> qi::float_];
        ruleVector3Data = qi::skip(qi::blank)[qi::float_ >> qi::float_ >> qi::float_];
        
        //----------------------------------------------------------------
        // Vertex Grammar
        //----------------------------------------------------------------

        ruleVertexSpatial = 
            qi::lit("v ") >> 
            ruleVector3Data [boost::phoenix::bind(&OBJState::addVertexSpatial, m_pOBJState, qi::_1)] >> 
            qi::eol;

        ruleVertexTexture =
            qi::lit("vt ") >>
            ruleVector2Data [boost::phoenix::bind(&OBJState::addVertexTexture, m_pOBJState, qi::_1)] >>
            qi::eol;

        ruleVertexNormal =
            qi::lit("vn ") >>
            ruleVector3Data [boost::phoenix::bind(&OBJState::addVertexNormal, m_pOBJState, qi::_1)] >>
            qi::eol;
        
        //----------------------------------------------------------------
        // Face Grammar
        //----------------------------------------------------------------
        
        //----------------------------------------------------------------
        // Start Grammar
        //----------------------------------------------------------------

        ruleStart = +(ruleGroup | ruleVertexSpatial | ruleVertexTexture | ruleVertexNormal);
    }

    //--------------------------------------------------------------------
    // Member Variables
    //--------------------------------------------------------------------

    qi::rule<Iterator, Skipper> ruleStart;
    qi::rule<Iterator, Skipper> ruleGroup;
    qi::rule<Iterator, std::string(), Skipper> ruleGroupName;
    
    qi::rule<Iterator, OBJVector2(), Skipper> ruleVector2Data;
    qi::rule<Iterator, OBJVector3(), Skipper> ruleVector3Data;
    
    qi::rule<Iterator, Skipper> ruleVertexSpatial;
    qi::rule<Iterator, Skipper> ruleVertexTexture;
    qi::rule<Iterator, Skipper> ruleVertexNormal;

protected:

    OBJState* m_pOBJState;

private:

};

//------------------------------------------------------------------------------------------

#endif

