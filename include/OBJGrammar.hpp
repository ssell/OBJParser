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
 *
 * Spirit grammar for parsing OBJ format files.
 * 
 * Based on the format specification at:
 * http://www.paulbourke.net/dataformats/obj/
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
        // Group Rule
        //----------------------------------------------------------------

        ruleGroupName = *(qi::char_ - ascii::space);
        ruleGroup = 
            qi::lit("g ") [boost::phoenix::bind(&OBJState::clearActiveGroups, m_pOBJState)] >> 
            (ruleGroupName [boost::phoenix::bind(&OBJState::addActiveGroup, m_pOBJState, qi::_1)] % +(ascii::blank)) >> 
            qi::eol;
        
        //----------------------------------------------------------------
        // Data Rule
        //----------------------------------------------------------------

        ruleVector2Data = qi::skip(qi::blank)[qi::float_ >> qi::float_];
        ruleVector3Data = qi::skip(qi::blank)[qi::float_ >> qi::float_ >> qi::float_];

        //----------------------------------------------------------------
        // Vertex Rule
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
        // Face Rule
        //----------------------------------------------------------------

        // parse an integer or, if none is found, default to 0
        ruleIndexValue = qi::int_ | qi::attr(0);

        // parse an index integer, then optionally parse a / followed by an optional index, and then optionally parse a / followed by an optional index
        ruleVertexGroupData = ruleIndexValue >> -qi::omit[qi::char_('/')] >> ruleIndexValue >> -qi::omit[qi::char_('/')] >> ruleIndexValue;
        
        // parse mandatory three vertex groups, followed by an optional fourth
        ruleFaceData = ruleVertexGroupData >> qi::omit[qi::blank] >> ruleVertexGroupData >> qi::omit[qi::blank] >> ruleVertexGroupData >> -qi::omit[qi::blank] >> -(ruleVertexGroupData);

        // parses triangle and quad faces
        ruleFace =
            qi::lit("f ") >>
            ruleFaceData [boost::phoenix::bind(&OBJState::addFace, m_pOBJState, qi::_1)] >>
            qi::eol;

        //----------------------------------------------------------------
        // Line Rule
        //----------------------------------------------------------------

        ruleLineData = +(qi::omit[qi::blank] >> ruleVertexGroupData);

        ruleLine =
            qi::lit("l") >>
            ruleLineData [boost::phoenix::bind(&OBJState::addLine, m_pOBJState, qi::_1)] >>
            qi::eol;

        //----------------------------------------------------------------
        // Point Rule
        //----------------------------------------------------------------
        
        rulePointData = +(qi::omit[qi::blank] >> ruleVertexGroupData);

        rulePoint =
            qi::lit("p") >>
            rulePointData [boost::phoenix::bind(&OBJState::addPointCollection, m_pOBJState, qi::_1)] >>
            qi::eol;

        //----------------------------------------------------------------
        // Start Rule
        //----------------------------------------------------------------

        ruleStart = +(ruleGroup | 
                      ruleVertexSpatial | 
                      ruleVertexTexture | 
                      ruleVertexNormal | 
                      ruleFace | 
                      ruleLine |
                      rulePoint);
    }

    //--------------------------------------------------------------------
    // Member Variables
    //--------------------------------------------------------------------

    qi::rule<Iterator, Skipper> ruleStart;
    qi::rule<Iterator, Skipper> ruleGroup;
    qi::rule<Iterator, std::string(), Skipper> ruleGroupName;
    
    qi::rule<Iterator, OBJVector2(), Skipper> ruleVector2Data;            ///< Parses "#.# #.#" of vertex point declarations (vt)
    qi::rule<Iterator, OBJVector3(), Skipper> ruleVector3Data;            ///< Parses "#.# #.# #.#" of vertex point declarations (v, vn, etc.)
    qi::rule<Iterator, OBJVertexGroup(), Skipper> ruleVertexGroupData;    ///< Parses "#/#/#" of vertex group declarations. Secondary elements (and their slashes) are optional.

    qi::rule<Iterator, int32_t(), Skipper> ruleIndexValue;
    qi::rule<Iterator, OBJFace(), Skipper> ruleFaceData;

    qi::rule<Iterator, Skipper> ruleFace;

    qi::rule<Iterator, std::vector<OBJVertexGroup>(), Skipper> ruleLineData;
    qi::rule<Iterator, Skipper> ruleLine;

    qi::rule<Iterator, std::vector<OBJVertexGroup>(), Skipper> rulePointData;
    qi::rule<Iterator, Skipper> rulePoint;

    qi::rule<Iterator, Skipper> ruleVertexSpatial;
    qi::rule<Iterator, Skipper> ruleVertexTexture;
    qi::rule<Iterator, Skipper> ruleVertexNormal;

protected:

    OBJState* m_pOBJState;

private:

};

//------------------------------------------------------------------------------------------

#endif

