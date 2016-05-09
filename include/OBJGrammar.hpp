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
    using Rule = qi::rule<Iterator, Skipper>;

public:

    OBJGrammar(OBJState* state, bool useFreeForms) 
        : OBJGrammar::base_type(ruleStart),
          m_pOBJState(state)
    {
        setupDataRules();
        setupGroupRules();
        setupVertexRules();
        setupFaceRules();
        setupMaterialRules();
        setupRenderStateRules();

        ruleEmptyLine = *(qi::blank) >> qi::eol;

        if(useFreeForms)
        {
            setupFreeFormRules();
            ruleStart = +(ruleGroup       |
                          ruleVertices    |
                          ruleFaces       | 
                          ruleFreeForms   |
                          ruleMaterials   |
                          ruleRenderState |
                          ruleEmptyLine);
        }
        else
        {
            ruleStart = +(ruleGroup       |
                          ruleVertices    |
                          ruleFaces       | 
                          ruleMaterials   |
                          ruleRenderState |
                          ruleEmptyLine);
        }
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
        ruleIndexList = +(qi::omit[qi::blank] >> ruleVertexGroupData);

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

        ruleVertexParameter =
            qi::lit("vp ") >>
            ruleVector3Data [boost::phoenix::bind(&OBJState::addVertexParameter, m_pOBJState, qi::_1)] >>
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

        ruleLine =
            qi::lit("l") >>
            ruleIndexList [boost::phoenix::bind(&OBJState::addLine, m_pOBJState, qi::_1)] >>
            *(qi::char_ - qi::eol) >>
            qi::eol;
        
        //----------------------------------------------------------------
        // Point Rule
        //----------------------------------------------------------------

        // Parses point lines. Example:
        // p 1 2 3 4 5
        
        rulePoint =
            qi::lit("p") >>
            ruleIndexList [boost::phoenix::bind(&OBJState::addPointCollection, m_pOBJState, qi::_1)] >>
            *(qi::char_ - qi::eol) >>
            qi::eol;
        
        ruleFaces = ruleFace | ruleLine | rulePoint;
    }

    void setupFreeFormRules()
    {
        setupFreeFormStart();
        setupFreeFormBody();
        setupFreeFormEnd();
        setupFreeFormAttributes();
        setupFreeFormConnections();

        ruleFreeForms =
            ruleFreeFormAttributes | 
            ruleFreeFormConnection |
            (ruleFreeFormStart >>
             +(ruleFreeFormBody) >>
             ruleFreeFormEnd);
    }

    void setupFreeFormStart()
    {
        //----------------------------------------------------------------
        // Curve
        //----------------------------------------------------------------

        ruleFreeFormCurveData =
            qi::float_ >>
            qi::omit[qi::blank] >> 
            qi::float_ >>
            qi::omit[qi::blank] >> 
            ruleIndexList;

        ruleFreeFormCurve =
            qi::lit("curv ") >>
            ruleFreeFormCurveData [boost::phoenix::bind(&OBJState::addFreeFormCurve, m_pOBJState, qi::_1)] >>
            *(qi::blank) >>
            qi::eol;

        //----------------------------------------------------------------
        // Curve2D
        //----------------------------------------------------------------

        ruleFreeFormCurve2DData =
            +(qi::omit[qi::blank] >> qi::int_);;

        ruleFreeFormCurve2D =
            qi::lit("curv2") >>
            ruleFreeFormCurve2DData [boost::phoenix::bind(&OBJState::addFreeFormCurve2D, m_pOBJState, qi::_1)] >>
            *(qi::blank) >>
            qi::eol;

        //----------------------------------------------------------------
        // Surface
        //----------------------------------------------------------------

        ruleFreeFormSurfaceData =
            qi::float_ >>
            qi::omit[qi::blank] >>
            qi::float_ >>
            qi::omit[qi::blank] >>
            qi::float_ >>
            qi::omit[qi::blank] >>
            qi::float_ >>
            qi::omit[qi::blank] >>
            ruleVertexGroupData;

        ruleFreeFormSurface =
            qi::lit("surf") >>
            ruleFreeFormSurfaceData [boost::phoenix::bind(&OBJState::addFreeFormSurface, m_pOBJState, qi::_1)] >>
            *(qi::blank) >>
            qi::eol;
            
        //----------------------------------------------------------------

        ruleFreeFormStart =
            (ruleFreeFormCurve |
             ruleFreeFormCurve2D |
             ruleFreeFormSurface);
    }

    void setupFreeFormBody()
    {
        //----------------------------------------------------------------
        // Parameters
        //----------------------------------------------------------------

        qi::rule<Iterator, std::vector<float>(), Skipper> ruleParameterData =
            +(qi::omit[qi::blank] >> qi::float_);

        Rule ruleParameterU =
            qi::lit("u") >>
            ruleParameterData [boost::phoenix::bind(&OBJState::addFreeFormParameterU, m_pOBJState, qi::_1)] >>
            *(qi::blank) >>
            qi::eol;

        Rule ruleParameterV =
            qi::lit("v") >>
            ruleParameterData [boost::phoenix::bind(&OBJState::addFreeFormParameterU, m_pOBJState, qi::_1)] >>
            *(qi::blank) >>
            qi::eol;

        ruleFreeFormParameter =
            qi::lit("parm ") >>
            (ruleParameterU | ruleParameterV);
        
        //----------------------------------------------------------------
        // Trim
        //----------------------------------------------------------------

        qi::rule<Iterator, OBJSimpleCurve(), Skipper> ruleSimpleCurve =
            qi::omit[qi::blank] >>
            qi::float_ >>
            qi::omit[qi::blank] >>
            qi::float_ >>
            qi::omit[qi::blank] >>
            qi::int_;

        ruleFreeFormTrim =
            qi::lit("trim") >>
            +(ruleSimpleCurve [boost::phoenix::bind(&OBJState::addFreeFormTrim, m_pOBJState, qi::_1)]) >>
            *(qi::blank) >>
            qi::eol;
        
        //----------------------------------------------------------------
        // Hole
        //----------------------------------------------------------------

        ruleFreeFormHole =
            qi::lit("hole") >>
            +(ruleSimpleCurve [boost::phoenix::bind(&OBJState::addFreeFormHole, m_pOBJState, qi::_1)]) >>
            *(qi::blank) >>
            qi::eol;
        
        //----------------------------------------------------------------
        // Special Curve
        //----------------------------------------------------------------

        ruleFreeFormTrim =
            qi::lit("scrv") >>
            +(ruleSimpleCurve [boost::phoenix::bind(&OBJState::addFreeFormSpecialCurve, m_pOBJState, qi::_1)]) >>
            *(qi::blank) >>
            qi::eol;
        
        //----------------------------------------------------------------
        // Special Point
        //----------------------------------------------------------------
        
        qi::rule<Iterator, std::vector<int32_t>(), Skipper> ruleSpecialPointsData =
            +(qi::omit[qi::blank] >> qi::int_);

        ruleFreeFormSpecialPoint = 
            qi::lit("sp") >>
            ruleSpecialPointsData [boost::phoenix::bind(&OBJState::addFreeFormSpecialPoints, m_pOBJState, qi::_1)] >>
            *(qi::blank) >>
            qi::eol;

        //----------------------------------------------------------------

        ruleFreeFormBody = 
            (ruleFreeFormParameter |
             ruleFreeFormTrim |
             ruleFreeFormHole |
             ruleFreeFormSpecialCurve |
             ruleFreeFormSpecialPoint);
    }

    void setupFreeFormEnd()
    {
        ruleFreeFormEnd =
            qi::lit("end") >>
            *(qi::blank) >>
            qi::eol;
    }

    void setupFreeFormAttributes()
    {
        //----------------------------------------------------------------
        // Type
        //----------------------------------------------------------------

        qi::rule<Iterator, bool(), Skipper> ruleFreeFormRational =
            (qi::omit[qi::lit(" rat")] [qi::_val = true] |
            (qi::attr(false)));

        Rule ruleTypes =
            (qi::lit(" bmatrix")  [boost::phoenix::bind(&OBJState::setFreeFormType, m_pOBJState, OBJFreeFormType::BasisMatrix)] |
             qi::lit(" bezier")   [boost::phoenix::bind(&OBJState::setFreeFormType, m_pOBJState, OBJFreeFormType::Bezier)]      |
             qi::lit(" bspline")  [boost::phoenix::bind(&OBJState::setFreeFormType, m_pOBJState, OBJFreeFormType::BSpline)]     |
             qi::lit(" cardinal") [boost::phoenix::bind(&OBJState::setFreeFormType, m_pOBJState, OBJFreeFormType::Cardinal)]    |
             qi::lit(" taylor")   [boost::phoenix::bind(&OBJState::setFreeFormType, m_pOBJState, OBJFreeFormType::Taylor)]);

        ruleFreeFormType =
            qi::lit("cstype") >>
            ruleFreeFormRational [boost::phoenix::bind(&OBJState::setFreeFormRational, m_pOBJState, qi::_1)] >>
            ruleTypes >>
            *(qi::blank) >>
            qi::eol;

        //----------------------------------------------------------------
        // Degree
        //----------------------------------------------------------------

        ruleFreeFormDegree = 
            qi::lit("deg ") >>
            qi::int_ [boost::phoenix::bind(&OBJState::setFreeFormDegreeU, m_pOBJState, qi::_1)] >>
            -(qi::omit[qi::blank] >> 
              qi::int_ [boost::phoenix::bind(&OBJState::setFreeFormDegreeV, m_pOBJState, qi::_1)]) >>
            *(qi::blank) >>
            qi::eol;

        //----------------------------------------------------------------
        // Step
        //----------------------------------------------------------------

        ruleFreeFormDegree = 
            qi::lit("step ") >>
            qi::int_ [boost::phoenix::bind(&OBJState::setFreeFormStepU, m_pOBJState, qi::_1)] >>
            -(qi::omit[qi::blank] >> 
              qi::int_ [boost::phoenix::bind(&OBJState::setFreeFormStepV, m_pOBJState, qi::_1)]) >>
            *(qi::blank) >>
            qi::eol;

        //----------------------------------------------------------------
        // Basis Matrix
        //----------------------------------------------------------------

        qi::rule<Iterator, std::vector<float>(), Skipper> ruleMatrixData =
            +(qi::skip[qi::blank] >> qi::float_);

        qi::rule<Iterator, Skipper> ruleBasisU =
            qi::lit(" u") >>
            ruleMatrixData [boost::phoenix::bind(&OBJState::setFreeFormBasisMatrixU, m_pOBJState, qi::_1)] >>
            *(qi::blank) >>
            qi::eol;

        qi::rule<Iterator, Skipper> ruleBasisV =
            qi::lit(" v") >>
            ruleMatrixData [boost::phoenix::bind(&OBJState::setFreeFormBasisMatrixV, m_pOBJState, qi::_1)] >>
            *(qi::blank) >>
            qi::eol;
        
        ruleFreeFormBasisMatrix = 
            qi::lit("bmat") >>
            (ruleBasisU | 
             ruleBasisV);

        //----------------------------------------------------------------
        // Merge Group
        //----------------------------------------------------------------

        ruleFreeFormMergeGroup =
            qi::lit("mg ") >>
            qi::int_ [boost::phoenix::bind(&OBJState::setFreeFormMergeGroupNumber, m_pOBJState, qi::_1)] >>
            qi::omit[qi::blank] >>
            qi::float_ [boost::phoenix::bind(&OBJState::setFreeFormMergeGroupResolution, m_pOBJState, qi::_1)] >> 
            *(qi::blank) >>
            qi::eol;

        //----------------------------------------------------------------

        ruleFreeFormAttributes = 
            (ruleFreeFormType |
             ruleFreeFormDegree |
             ruleFreeFormStep |
             ruleFreeFormBasisMatrix |
             ruleFreeFormMergeGroup);
    }

    void setupFreeFormConnections()
    {
        qi::rule<Iterator, OBJSurfaceConnection(), Skipper> ruleConnectionData =
            qi::int_ >>                // surface1
            qi::omit[qi::blank] >>
            qi::float_ >>              // startParam1
            qi::omit[qi::blank] >>
            qi::float_ >>              // endParam1
            qi::omit[qi::blank] >>
            qi::int_ >>                // curve2D1
            qi::omit[qi::blank] >>
            qi::int_ >>                // surface2
            qi::omit[qi::blank] >>
            qi::float_ >>              // startParam2
            qi::omit[qi::blank] >>
            qi::float_ >>              // endParam2
            qi::omit[qi::blank] >>
            qi::int_;                  // curve2D2

        ruleFreeFormConnection = 
            qi::lit("con ") >>
            ruleConnectionData [boost::phoenix::bind(&OBJState::addFreeFormConnection, m_pOBJState, qi::_1)] >>
            *(qi::blank) >>
            qi::eol;
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

    void setupRenderStateRules()
    {
        //----------------------------------------------------------------
        // Polygon Only
        //----------------------------------------------------------------

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
        
        //----------------------------------------------------------------
        // General
        //----------------------------------------------------------------

        ruleSmoothing = 
            qi::lit("s ") >> 
            (qi::lit("off") [boost::phoenix::bind(&OBJState::setSmoothingGroup, m_pOBJState, 0)] |
             qi::uint_ [boost::phoenix::bind(&OBJState::setSmoothingGroup, m_pOBJState, qi::_1)]) >> 
            qi::eol;

        ruleLOD = 
            qi::lit("lod ") >>
            qi::int_ [boost::phoenix::bind(&OBJState::setLevelOfDetail, m_pOBJState, qi::_1)] >>
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
        
        //----------------------------------------------------------------
        // Free-Form Only
        //----------------------------------------------------------------

        // Curve Technique

        Rule ruleCurveParametric =
            qi::lit("cparm") >>
            qi::float_ [boost::phoenix::bind(&OBJState::setTechniqueParametric, m_pOBJState, qi::_1)] >>
            qi::eol;

        Rule ruleCurveSpatial = 
            qi::lit("cspace") >>
            qi::float_ [boost::phoenix::bind(&OBJState::setTechniqueSpatialCurve, m_pOBJState, qi::_1)] >>
            qi::eol;

        Rule ruleCurveCurvature =
            qi::lit("curv") >>
            ruleVector2Data [boost::phoenix::bind(&OBJState::setTechniqueCurvatureCurve, m_pOBJState, qi::_1)] >>
            qi::eol;

        ruleFreeFormCurveTech =
            qi::lit("ctech") >>
            (ruleCurveParametric |
             ruleCurveSpatial |
             ruleCurveCurvature);

        // Surface Technique

        Rule ruleSurfaceParametricA =
            qi::lit("cparma") >>
            ruleVector2Data [boost::phoenix::bind(&OBJState::setTechniqueParametricA, m_pOBJState, qi::_1)] >>
            qi::eol;

        Rule ruleSurfaceParametricB =
            qi::lit("cparmb") >>
            qi::float_ [boost::phoenix::bind(&OBJState::setTechniqueParametricB, m_pOBJState, qi::_1)] >>
            qi::eol;

        Rule ruleSurfaceSpatial = 
            qi::lit("cspace") >>
            qi::float_ [boost::phoenix::bind(&OBJState::setTechniqueSpatialSurface, m_pOBJState, qi::_1)] >>
            qi::eol;

        Rule ruleSurfaceCurvature =
            qi::lit("curv") >>
            ruleVector2Data [boost::phoenix::bind(&OBJState::setTechniqueCurvatureSurface, m_pOBJState, qi::_1)] >>
            qi::eol;

        ruleFreeFormSurfaceTech =
            qi::lit("ctech") >>
            (ruleSurfaceParametricA |
             ruleSurfaceParametricB |
             ruleSurfaceSpatial |
             ruleSurfaceCurvature);


        
        //----------------------------------------------------------------

        ruleRenderState = ruleSmoothing |
                          ruleLOD |
                          ruleBevelInterp |
                          ruleColorInterp |
                          ruleDissolveInterp |
                          ruleTextureMapLibrary |
                          ruleTextureMap |
                          ruleShadowObj |
                          ruleTraceObj | 
                          ruleFreeFormCurveTech |
                          ruleFreeFormSurfaceTech;
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
    qi::rule<Iterator, Skipper> ruleRenderState;
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
    qi::rule<Iterator, std::vector<OBJVertexGroup>(), Skipper> ruleIndexList;
    qi::rule<Iterator, std::string(), Skipper> ruleName;

    //--------------------------------------------------------------------
    // Vertex Rules
    //--------------------------------------------------------------------

    qi::rule<Iterator, Skipper> ruleVertexSpatial;
    qi::rule<Iterator, Skipper> ruleVertexTexture;
    qi::rule<Iterator, Skipper> ruleVertexNormal;
    qi::rule<Iterator, Skipper> ruleVertexParameter;

    //--------------------------------------------------------------------
    // Face Rules
    //--------------------------------------------------------------------

    qi::rule<Iterator, OBJFace(), Skipper> ruleFaceData;
    qi::rule<Iterator, Skipper> ruleFace;

    qi::rule<Iterator, Skipper> ruleLine;
    qi::rule<Iterator, Skipper> rulePoint;

    //--------------------------------------------------------------------
    // Free-Form Rules
    //--------------------------------------------------------------------

    // start statements
    
    qi::rule<Iterator, Skipper> ruleFreeFormStart;

    qi::rule<Iterator, Skipper> ruleFreeFormCurve;  
    qi::rule<Iterator, OBJCurve(), Skipper> ruleFreeFormCurveData;

    qi::rule<Iterator, Skipper> ruleFreeFormCurve2D;  
    qi::rule<Iterator, std::vector<int32_t>(), Skipper> ruleFreeFormCurve2DData;

    qi::rule<Iterator, Skipper> ruleFreeFormSurface; 
    qi::rule<Iterator, OBJSurface(), Skipper> ruleFreeFormSurfaceData;

    // body statements (may only appear between start and end statements)

    qi::rule<Iterator, Skipper> ruleFreeFormBody;

    qi::rule<Iterator, Skipper> ruleFreeFormParameter;       // parm
    qi::rule<Iterator, Skipper> ruleFreeFormTrim;            // trim
    qi::rule<Iterator, Skipper> ruleFreeFormHole;            // hole
    qi::rule<Iterator, Skipper> ruleFreeFormSpecialCurve;    // scrv
    qi::rule<Iterator, Skipper> ruleFreeFormSpecialPoint;    // sp

    // end statement

    qi::rule<Iterator, Skipper> ruleFreeFormEnd;
    
    // attribute statements

    qi::rule<Iterator, Skipper> ruleFreeFormAttributes;

    qi::rule<Iterator, Skipper> ruleFreeFormType;            // cstype
    qi::rule<Iterator, Skipper> ruleFreeFormDegree;          // deg
    qi::rule<Iterator, Skipper> ruleFreeFormStep;            // step
    qi::rule<Iterator, Skipper> ruleFreeFormBasisMatrix;     // bmat
    qi::rule<Iterator, Skipper> ruleFreeFormMergeGroup;      // mg

    // connections

    qi::rule<Iterator, Skipper> ruleFreeFormConnection;      // con

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

    qi::rule<Iterator, Skipper> ruleFreeFormCurveTech; 
    qi::rule<Iterator, Skipper> ruleFreeFormSurfaceTech;  

    //--------------------------------------------------------------------
    // Non-Rule Members
    //--------------------------------------------------------------------

    OBJState* m_pOBJState;

private:

};

//------------------------------------------------------------------------------------------

#endif

