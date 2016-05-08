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
#include "OBJGrammarSkipper.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;

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
        setupDataRules();
        setupGroupRules();
        setupVertexRules();
        setupFaceRules();
        setupFreeFormRules();
        setupMaterialRules();
        setupAuxiliaryRules();

        ruleEmptyLine = *(qi::blank) >> qi::eol;

        ruleStart = +(ruleGroup       |
                      ruleVertices    |
                      ruleFaces       | 
                      ruleFreeForms   |
                      ruleMaterials   |
                      ruleAuxiliaries |
                      ruleEmptyLine);
    }

protected:

    //--------------------------------------------------------------------------------------
    // Rule Setup Methods
    //--------------------------------------------------------------------------------------

    void setupDataRules()
    {
        // At the end of the vector rules we consume any unexcepted characters to account for certain obj writers
        ruleVector2Data = qi::skip(qi::blank)[qi::float_ >> qi::float_ >> *(qi::char_ - qi::eol)];
        ruleVector3Data = qi::skip(qi::blank)[qi::float_ >> qi::float_ >> qi::float_ >> *(qi::char_ - qi::eol)];
        ruleVector4Data = qi::skip(qi::blank)[qi::float_ >> qi::float_ >> qi::float_ >> -(qi::float_) >> *(qi::char_ - qi::eol)];

        ruleIndexValue = qi::int_ | qi::attr(0);
        ruleVertexGroupData = ruleIndexValue >> -qi::omit[qi::char_('/')] >> ruleIndexValue >> -qi::omit[qi::char_('/')] >> ruleIndexValue;

        ruleName = +(qi::char_ - qi::space);
    }

    void setupGroupRules()
    {
        // Parses group lines. Example:
        // g default

        ruleGroupName = *(qi::char_ - ascii::space);
        ruleGroup = 
            qi::lit("g ") [boost::phoenix::bind(&OBJState::clearActiveGroups, m_pOBJState)] >> 
            (ruleGroupName [boost::phoenix::bind(&OBJState::addActiveGroup, m_pOBJState, qi::_1)] % +(ascii::blank)) >> 
            qi::eol;
    }

    void setupVertexRules()
    {
        // Parses vertex data lines. Example:
        //  v 0.0 1.0 2.0
        //  vt 1.0 1.0
        //  vn 0.0 0.0 0.0

        ruleVertexSpatial = 
            qi::lit("v ") >> 
            ruleVector4Data [boost::phoenix::bind(&OBJState::addVertexSpatial, m_pOBJState, qi::_1)] >> 
            qi::eol;

        ruleVertexTexture =
            qi::lit("vt ") >>
            ruleVector2Data [boost::phoenix::bind(&OBJState::addVertexTexture, m_pOBJState, qi::_1)] >>
            qi::eol;

        ruleVertexNormal =
            qi::lit("vn ") >>
            ruleVector3Data [boost::phoenix::bind(&OBJState::addVertexNormal, m_pOBJState, qi::_1)] >>
            qi::eol;
        
        ruleVertices = ruleVertexSpatial | ruleVertexTexture | ruleVertexNormal;
    }

    void setupFaceRules()
    {
        //----------------------------------------------------------------
        // Face Rule
        //----------------------------------------------------------------

        // Parses face lines. Example:
        // f 1//3 2//4 3//5

        ruleFaceData = 
            ruleVertexGroupData >> 
            qi::omit[qi::blank] >> 
            ruleVertexGroupData >> 
            qi::omit[qi::blank] >> 
            ruleVertexGroupData >> 
            -qi::omit[qi::blank] >> 
            -(ruleVertexGroupData);

        ruleFace =
            qi::lit("f ") >>
            ruleFaceData [boost::phoenix::bind(&OBJState::addFace, m_pOBJState, qi::_1)] >>
            *(qi::char_ - qi::eol) >>
            qi::eol;
        
        //----------------------------------------------------------------
        // Line Rule
        //----------------------------------------------------------------

        // Parses line lines. Example:
        // l 1/1 2/2 3/3 4/4 5/5 

        ruleLineData = +(qi::omit[qi::blank] >> ruleVertexGroupData);

        ruleLine =
            qi::lit("l") >>
            ruleLineData [boost::phoenix::bind(&OBJState::addLine, m_pOBJState, qi::_1)] >>
            *(qi::char_ - qi::eol) >>
            qi::eol;
        
        //----------------------------------------------------------------
        // Point Rule
        //----------------------------------------------------------------

        // Parses point lines. Example:
        // p 1 2 3 4 5
        
        rulePointData = +(qi::omit[qi::blank] >> ruleVertexGroupData);

        rulePoint =
            qi::lit("p") >>
            rulePointData [boost::phoenix::bind(&OBJState::addPointCollection, m_pOBJState, qi::_1)] >>
            *(qi::char_ - qi::eol) >>
            qi::eol;
        
        ruleFaces = ruleFace | ruleLine | rulePoint;
    }

    void setupFreeFormRules()
    {
    
    }

    void setupMaterialRules()
    {
        // Parses material lines. Example:
        // mtllib test.mtl
        // usemtl test

        ruleMaterialLibrary = 
            qi::lit("mtllib ") >> 
            +(ruleName [boost::phoenix::bind(&OBJState::addMaterialLibrary, m_pOBJState, qi::_1)] >> 
            -(qi::omit[qi::blank]))  >> 
            qi::eol;

        ruleMaterialUse = 
            qi::lit("usemtl ") >> 
            ruleName [boost::phoenix::bind(&OBJState::setMaterial, m_pOBJState, qi::_1)] >> 
            qi::eol;

        ruleMaterials = ruleMaterialLibrary | ruleMaterialUse;
    }

    void setupAuxiliaryRules()
    {
        ruleSmoothing = 
            qi::lit("s ") >> 
            (qi::lit("off") [boost::phoenix::bind(&OBJState::setSmoothingGroup, m_pOBJState, 0)] |
             qi::uint_ [boost::phoenix::bind(&OBJState::setSmoothingGroup, m_pOBJState, qi::_1)]) >> 
            qi::eol;

        ruleLOD = 
            qi::lit("lod ") >>
            qi::int_ [boost::phoenix::bind(&OBJState::setLevelOfDetail, m_pOBJState, qi::_1)] >>
            qi::eol;

        ruleBevelInterp = 
            qi::lit("bevel ") >> 
            (qi::lit("on") [boost::phoenix::bind(&OBJState::setBevelInterp, m_pOBJState, true)] | 
             qi::lit("off") [boost::phoenix::bind(&OBJState::setBevelInterp, m_pOBJState, false)]) >> 
            qi::eol;
        
        ruleColorInterp = 
            qi::lit("c_interp ") >> 
            (qi::lit("on") [boost::phoenix::bind(&OBJState::setColorInterp, m_pOBJState, true)] | 
             qi::lit("off") [boost::phoenix::bind(&OBJState::setColorInterp, m_pOBJState, false)]) >> 
            qi::eol;
        
        ruleDissolveInterp = 
            qi::lit("d_interp ") >> 
            (qi::lit("on") [boost::phoenix::bind(&OBJState::setDissolveInterp, m_pOBJState, true)] | 
             qi::lit("off") [boost::phoenix::bind(&OBJState::setDissolveInterp, m_pOBJState, false)]) >> 
            qi::eol;

        ruleTextureMapLibrary = 
            qi::lit("maplib ") >> 
            +(ruleName [boost::phoenix::bind(&OBJState::addTextureMapLibrary, m_pOBJState, qi::_1)] >> 
            -(qi::omit[qi::blank])) >> 
            qi::eol;
        
        ruleTextureMap = 
            qi::lit("usemap ") >> 
            (qi::lit("off") [boost::phoenix::bind(&OBJState::setTextureMap, m_pOBJState, "")] |
             ruleName [boost::phoenix::bind(&OBJState::setTextureMap, m_pOBJState, qi::_1)]) >> 
            qi::eol;

        ruleShadowObj = 
            qi::lit("shadow_obj ") >> 
            ruleName [boost::phoenix::bind(&OBJState::setShadowObject, m_pOBJState, qi::_1)] >>
            qi::eol;

        ruleTraceObj = 
            qi::lit("trace_obj ") >> 
            ruleName [boost::phoenix::bind(&OBJState::setTracingObject, m_pOBJState, qi::_1)] >>
            qi::eol;

        ruleAuxiliaries = ruleSmoothing |
                          ruleLOD |
                          ruleBevelInterp |
                          ruleColorInterp |
                          ruleDissolveInterp |
                          ruleTextureMapLibrary |
                          ruleTextureMap |
                          ruleShadowObj |
                          ruleTraceObj;
    }

    //--------------------------------------------------------------------------------------
    // Member Variables
    //--------------------------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Top-Level Rules
    //--------------------------------------------------------------------
    
    qi::rule<Iterator, Skipper> ruleStart;
    qi::rule<Iterator, Skipper> ruleVertices;
    qi::rule<Iterator, Skipper> ruleFaces;
    qi::rule<Iterator, Skipper> ruleFreeForms;
    qi::rule<Iterator, Skipper> ruleMaterials;
    qi::rule<Iterator, Skipper> ruleAuxiliaries;
    qi::rule<Iterator, Skipper> ruleEmptyLine;

    //--------------------------------------------------------------------
    // Group Rules
    //--------------------------------------------------------------------

    qi::rule<Iterator, Skipper> ruleGroup;
    qi::rule<Iterator, std::string(), Skipper> ruleGroupName;
    
    //--------------------------------------------------------------------
    // Misc Data Rules
    //--------------------------------------------------------------------

    qi::rule<Iterator, OBJVector2(), Skipper> ruleVector2Data;            ///< Parses "#.# #.#" of vertex point declarations (vt)
    qi::rule<Iterator, OBJVector3(), Skipper> ruleVector3Data;            ///< Parses "#.# #.# #.#" of vertex point declarations (vn)
    qi::rule<Iterator, OBJVector4(), Skipper> ruleVector4Data;            ///< Parses "#.# #.# #.# #.#" where the fourth element is optional (v)
    qi::rule<Iterator, OBJVertexGroup(), Skipper> ruleVertexGroupData;    ///< Parses "#/#/#" of vertex group declarations. Secondary elements (and their slashes) are optional.
    qi::rule<Iterator, int32_t(), Skipper> ruleIndexValue;
    qi::rule<Iterator, std::string(), Skipper> ruleName;

    //--------------------------------------------------------------------
    // Vertex Rules
    //--------------------------------------------------------------------

    qi::rule<Iterator, Skipper> ruleVertexSpatial;
    qi::rule<Iterator, Skipper> ruleVertexTexture;
    qi::rule<Iterator, Skipper> ruleVertexNormal;

    //--------------------------------------------------------------------
    // Face Rules
    //--------------------------------------------------------------------

    qi::rule<Iterator, OBJFace(), Skipper> ruleFaceData;
    qi::rule<Iterator, Skipper> ruleFace;

    qi::rule<Iterator, std::vector<OBJVertexGroup>(), Skipper> ruleLineData;
    qi::rule<Iterator, Skipper> ruleLine;

    qi::rule<Iterator, std::vector<OBJVertexGroup>(), Skipper> rulePointData;
    qi::rule<Iterator, Skipper> rulePoint;

    //--------------------------------------------------------------------
    // Free-Form Rules
    //--------------------------------------------------------------------

    // start statements
    
    qi::rule<Iterator, Skipper> ruleFreeFormCurve;           // curv
    qi::rule<Iterator, Skipper> ruleFreeFormCurve2;          // curv2
    qi::rule<Iterator, Skipper> ruleFreeFormSurface;         // surf

    // end statement

    qi::rule<Iterator, Skipper> ruleFreeFormEnd;             // end
    
    // body statements (may only appear between start and end statements)

    qi::rule<Iterator, Skipper> ruleFreeFormParameter;       // parm
    qi::rule<Iterator, Skipper> ruleFreeFormTrim;            // trim
    qi::rule<Iterator, Skipper> ruleFreeFormHole;            // hole
    qi::rule<Iterator, Skipper> ruleFreeFormSpecialPoint;    // sp
    qi::rule<Iterator, Skipper> ruleFreeFormSpecialCurve;    // scrv

    // state statements
    
    qi::rule<Iterator, Skipper> ruleFreeFormCSType;          // cstype
    qi::rule<Iterator, Skipper> ruleFreeFormCTech;           // ctech
    qi::rule<Iterator, Skipper> ruleFreeFormSTech;           // stech
    qi::rule<Iterator, Skipper> ruleFreeFormDegree;          // deg
    qi::rule<Iterator, Skipper> ruleFreeFormBasisMatrix;     // bmat
    qi::rule<Iterator, Skipper> ruleFreeFormStep;            // step

    // aux statements


    //--------------------------------------------------------------------
    // Material Rules
    //--------------------------------------------------------------------

    qi::rule<Iterator, Skipper> ruleMaterialLibrary;
    qi::rule<Iterator, Skipper> ruleMaterialUse;

    //--------------------------------------------------------------------
    // Render Attribute Rules
    //--------------------------------------------------------------------
    
    // polygon only

    qi::rule<Iterator, Skipper> ruleBevelInterp;
    qi::rule<Iterator, Skipper> ruleColorInterp;
    qi::rule<Iterator, Skipper> ruleDissolveInterp;

    // all

    qi::rule<Iterator, Skipper> ruleSmoothing;
    qi::rule<Iterator, Skipper> ruleLOD;
    qi::rule<Iterator, Skipper> ruleTextureMapLibrary;
    qi::rule<Iterator, Skipper> ruleTextureMap;
    qi::rule<Iterator, Skipper> ruleShadowObj;
    qi::rule<Iterator, Skipper> ruleTraceObj;

    // free-form only

    qi::rule<Iterator, Skipper> ruleFreeFormCurveTech;       // ctech
    qi::rule<Iterator, Skipper> ruleFreeFormSurfaceTech;     // stech

    //--------------------------------------------------------------------
    // Non-Rule Members
    //--------------------------------------------------------------------

    OBJState* m_pOBJState;

private:

};

//------------------------------------------------------------------------------------------

#endif

