/**
 * Copyright 2016 Steven T Sell (ssell@vertexfragment.com)
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

#include "OBJGrammar.hpp"
#include "OBJState.hpp"

//------------------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------------------

OBJGrammar::OBJGrammar(OBJState* state) 
    : OBJGrammar::base_type(ruleStart),
      m_pOBJState(state)
{
    setupDataRules();
    setupGroupRules();
    setupVertexRules();
    setupFaceRules();
    setupMaterialRules();
    setupRenderStateRules();
    setupFreeFormRules();

    ruleStart = +(ruleGroup       |
                  ruleVertices    |
                  ruleFaces       | 
                  ruleFreeForms   |
                  ruleMaterials   |
                  ruleRenderState |
                  qi::eol);
}

//------------------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// Protected Methods
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------------------

void OBJGrammar::setupDataRules()
{
    // At the end of the vector rules we consume any unexcepted characters to account for certain obj writers
    ruleVector2Data = qi::float_ >> qi::float_ >> *(qi::char_ - qi::eol);
    ruleVector3Data = qi::float_ >> qi::float_ >> qi::float_ >> *(qi::char_ - qi::eol);
    ruleVector4Data = qi::float_ >> qi::float_ >> qi::float_ >> -(qi::float_) >> *(qi::char_ - qi::eol);

    ruleIndexValue = qi::int_ | qi::attr(0);
    ruleVertexGroupData = ruleIndexValue >> -(qi::omit[qi::char_('/')] >> ruleIndexValue >> -(qi::omit[qi::char_('/')] >> ruleIndexValue));
    ruleIndexList = +(ruleVertexGroupData);

    ruleName = qi::lexeme[+(qi::graph)];
}

void OBJGrammar::setupGroupRules()
{
    // Parses group lines. Example:
    // g default

    ruleGroupName = *(qi::char_ - ascii::space);
    ruleGroup = 
        qi::lit("g") [boost::phoenix::bind(&OBJState::clearActiveGroups, m_pOBJState)] >> 
        +(ruleName [boost::phoenix::bind(&OBJState::addActiveGroup, m_pOBJState, qi::_1)]) >> 
        qi::eol;
}

void OBJGrammar::setupVertexRules()
{
    // Parses vertex data lines. Example:
    //  v 0.0 1.0 2.0
    //  vt 1.0 1.0
    //  vn 0.0 0.0 0.0

    ruleVertexSpatial = 
        qi::lit("v") >> 
        ruleVector4Data [boost::phoenix::bind(&OBJState::addVertexSpatial, m_pOBJState, qi::_1)] >> 
        qi::eol;

    ruleVertexTexture =
        qi::lit("vt") >>
        ruleVector2Data [boost::phoenix::bind(&OBJState::addVertexTexture, m_pOBJState, qi::_1)] >>
        qi::eol;

    ruleVertexNormal =
        qi::lit("vn") >>
        ruleVector3Data [boost::phoenix::bind(&OBJState::addVertexNormal, m_pOBJState, qi::_1)] >>
        qi::eol;

    ruleVertexParameter =
        qi::lit("vp") >>
        ruleVector3Data [boost::phoenix::bind(&OBJState::addVertexParameter, m_pOBJState, qi::_1)] >>
        qi::eol;
        
    ruleVertices = 
        +(ruleVertexSpatial) |         // We check for multiple instances of a vertex in a row, as
        +(ruleVertexTexture) |         // the most likely followup to a 'vn' is another 'vn', etc.
        +(ruleVertexNormal);
}

void OBJGrammar::setupFaceRules()
{
    //----------------------------------------------------------------
    // Face Rule
    //----------------------------------------------------------------

    // Parses face lines. Example:
    // f 1//3 2//4 3//5

    ruleFaceData = 
        ruleVertexGroupData >> 
        ruleVertexGroupData >> 
        ruleVertexGroupData >> 
        -(ruleVertexGroupData);

    ruleFace =
        qi::lit("f") >>
        ruleFaceData [boost::phoenix::bind(&OBJState::addFace, m_pOBJState, qi::_1)] >>
        qi::eol;
        
    //----------------------------------------------------------------
    // Line Rule
    //----------------------------------------------------------------

    // Parses line lines. Example:
    // l 1/1 2/2 3/3 4/4 5/5 

    ruleLine =
        qi::lit("l") >>
        ruleIndexList [boost::phoenix::bind(&OBJState::addLine, m_pOBJState, qi::_1)] >>
        qi::eol;
        
    //----------------------------------------------------------------
    // Point Rule
    //----------------------------------------------------------------

    // Parses point lines. Example:
    // p 1 2 3 4 5
        
    rulePoint =
        qi::lit("p") >>
        ruleIndexList [boost::phoenix::bind(&OBJState::addPointCollection, m_pOBJState, qi::_1)] >>
        qi::eol;
        
    ruleFaces = 
        +(ruleFace) |          // We check for multiple instances of a face in a row, as
        +(ruleLine) |          // the most likely followup to a 'f' is another 'f', etc.
        +(rulePoint);
}

void OBJGrammar::setupFreeFormRules()
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

void OBJGrammar::setupFreeFormStart()
{
    //----------------------------------------------------------------
    // Curve
    //----------------------------------------------------------------

    ruleFreeFormCurveData =
        qi::float_ >>
        qi::float_ >>
        ruleIndexList;

    ruleFreeFormCurve =
        qi::lit("curv") >>
        ruleFreeFormCurveData [boost::phoenix::bind(&OBJState::addFreeFormCurve, m_pOBJState, qi::_1)] >>
        qi::eol;

    //----------------------------------------------------------------
    // Curve2D
    //----------------------------------------------------------------

    ruleFreeFormCurve2DData =
        +(qi::int_);

    ruleFreeFormCurve2D =
        qi::lit("curv2") >>
        ruleFreeFormCurve2DData [boost::phoenix::bind(&OBJState::addFreeFormCurve2D, m_pOBJState, qi::_1)] >>
        qi::eol;

    //----------------------------------------------------------------
    // Surface
    //----------------------------------------------------------------

    ruleFreeFormSurfaceData =
        qi::float_ >>
        qi::float_ >>
        qi::float_ >>
        qi::float_ >>
        ruleVertexGroupData;

    ruleFreeFormSurface =
        qi::lit("surf") >>
        ruleFreeFormSurfaceData [boost::phoenix::bind(&OBJState::addFreeFormSurface, m_pOBJState, qi::_1)] >>
        qi::eol;
            
    //----------------------------------------------------------------

    ruleFreeFormStart =
        (ruleFreeFormCurve |
         ruleFreeFormCurve2D |
         ruleFreeFormSurface);
}

void OBJGrammar::setupFreeFormBody()
{
    //----------------------------------------------------------------
    // Parameters
    //----------------------------------------------------------------

    qi::rule<OBJIterator, std::vector<float>(), OBJSkipper> ruleParameterData =
        +(qi::float_);

    qi::rule<OBJIterator, OBJSkipper> ruleParameterU =
        qi::lit("u") >>
        ruleParameterData [boost::phoenix::bind(&OBJState::addFreeFormParameterU, m_pOBJState, qi::_1)] >>
        qi::eol;

    qi::rule<OBJIterator, OBJSkipper> ruleParameterV =
        qi::lit("v") >>
        ruleParameterData [boost::phoenix::bind(&OBJState::addFreeFormParameterU, m_pOBJState, qi::_1)] >>
        qi::eol;

    ruleFreeFormParameter =
        qi::lit("parm") >>
        (ruleParameterU | ruleParameterV);
        
    //----------------------------------------------------------------
    // Trim
    //----------------------------------------------------------------

    qi::rule<OBJIterator, OBJSimpleCurve(), OBJSkipper> ruleSimpleCurve =
        qi::float_ >>
        qi::float_ >>
        qi::int_;

    ruleFreeFormTrim =
        qi::lit("trim") >>
        +(ruleSimpleCurve [boost::phoenix::bind(&OBJState::addFreeFormTrim, m_pOBJState, qi::_1)]) >>
        qi::eol;
        
    //----------------------------------------------------------------
    // Hole
    //----------------------------------------------------------------

    ruleFreeFormHole =
        qi::lit("hole") >>
        +(ruleSimpleCurve [boost::phoenix::bind(&OBJState::addFreeFormHole, m_pOBJState, qi::_1)]) >>
        qi::eol;
        
    //----------------------------------------------------------------
    // Special Curve
    //----------------------------------------------------------------

    ruleFreeFormTrim =
        qi::lit("scrv") >>
        +(ruleSimpleCurve [boost::phoenix::bind(&OBJState::addFreeFormSpecialCurve, m_pOBJState, qi::_1)]) >>
        qi::eol;
        
    //----------------------------------------------------------------
    // Special Point
    //----------------------------------------------------------------
        
    qi::rule<OBJIterator, std::vector<int32_t>(), OBJSkipper> ruleSpecialPointsData =
        +(qi::int_);

    ruleFreeFormSpecialPoint = 
        qi::lit("sp") >>
        ruleSpecialPointsData [boost::phoenix::bind(&OBJState::addFreeFormSpecialPoints, m_pOBJState, qi::_1)] >>
        qi::eol;

    //----------------------------------------------------------------

    ruleFreeFormBody = 
        (ruleFreeFormParameter |
         ruleFreeFormTrim |
         ruleFreeFormHole |
         ruleFreeFormSpecialCurve |
         ruleFreeFormSpecialPoint);
}

void OBJGrammar::setupFreeFormEnd()
{
    ruleFreeFormEnd =
        qi::lit("end") >>
        qi::eol;
}

void OBJGrammar::setupFreeFormAttributes()
{
    //----------------------------------------------------------------
    // Type
    //----------------------------------------------------------------

    qi::rule<OBJIterator, bool(), OBJSkipper> ruleFreeFormRational =
        (qi::omit[qi::lit("rat")] [qi::_val = true] |
        (qi::attr(false)));

    qi::rule<OBJIterator, OBJSkipper> ruleTypes =
        (qi::lit("bmatrix")  [boost::phoenix::bind(&OBJState::setFreeFormType, m_pOBJState, OBJFreeFormType::BasisMatrix)] |
         qi::lit("bezier")   [boost::phoenix::bind(&OBJState::setFreeFormType, m_pOBJState, OBJFreeFormType::Bezier)]      |
         qi::lit("bspline")  [boost::phoenix::bind(&OBJState::setFreeFormType, m_pOBJState, OBJFreeFormType::BSpline)]     |
         qi::lit("cardinal") [boost::phoenix::bind(&OBJState::setFreeFormType, m_pOBJState, OBJFreeFormType::Cardinal)]    |
         qi::lit("taylor")   [boost::phoenix::bind(&OBJState::setFreeFormType, m_pOBJState, OBJFreeFormType::Taylor)]);

    ruleFreeFormType =
        qi::lit("cstype") >>
        ruleFreeFormRational [boost::phoenix::bind(&OBJState::setFreeFormRational, m_pOBJState, qi::_1)] >>
        ruleTypes >>
        qi::eol;

    //----------------------------------------------------------------
    // Degree
    //----------------------------------------------------------------

    ruleFreeFormDegree = 
        qi::lit("deg") >>
        qi::int_ [boost::phoenix::bind(&OBJState::setFreeFormDegreeU, m_pOBJState, qi::_1)] >>
        -(qi::int_ [boost::phoenix::bind(&OBJState::setFreeFormDegreeV, m_pOBJState, qi::_1)]) >>
        qi::eol;

    //----------------------------------------------------------------
    // Step
    //----------------------------------------------------------------

    ruleFreeFormStep = 
        qi::lit("step") >>
        qi::int_ [boost::phoenix::bind(&OBJState::setFreeFormStepU, m_pOBJState, qi::_1)] >>
        -(qi::int_ [boost::phoenix::bind(&OBJState::setFreeFormStepV, m_pOBJState, qi::_1)]) >>
        qi::eol;

    //----------------------------------------------------------------
    // Basis Matrix
    //----------------------------------------------------------------

    qi::rule<OBJIterator, std::vector<float>(), OBJSkipper> ruleMatrixData =
        +(qi::float_);

    qi::rule<OBJIterator, OBJSkipper> ruleBasisU =
        qi::lit("u") >>
        ruleMatrixData [boost::phoenix::bind(&OBJState::setFreeFormBasisMatrixU, m_pOBJState, qi::_1)] >>
        qi::eol;

    qi::rule<OBJIterator, OBJSkipper> ruleBasisV =
        qi::lit("v") >>
        ruleMatrixData [boost::phoenix::bind(&OBJState::setFreeFormBasisMatrixV, m_pOBJState, qi::_1)] >>
        qi::eol;
        
    ruleFreeFormBasisMatrix = 
        qi::lit("bmat") >>
        (ruleBasisU | 
         ruleBasisV);

    //----------------------------------------------------------------
    // Merge Group
    //----------------------------------------------------------------

    ruleFreeFormMergeGroup =
        qi::lit("mg") >>
        qi::int_ [boost::phoenix::bind(&OBJState::setFreeFormMergeGroupNumber, m_pOBJState, qi::_1)] >>
        qi::float_ [boost::phoenix::bind(&OBJState::setFreeFormMergeGroupResolution, m_pOBJState, qi::_1)] >> 
        qi::eol;

    //----------------------------------------------------------------

    ruleFreeFormAttributes = 
        (ruleFreeFormType |
         ruleFreeFormDegree |
         ruleFreeFormStep |
         ruleFreeFormBasisMatrix |
         ruleFreeFormMergeGroup);
}

void OBJGrammar::setupFreeFormConnections()
{
    qi::rule<OBJIterator, OBJSurfaceConnection(), OBJSkipper> ruleConnectionData =
        qi::int_ >>                // surface1
        qi::float_ >>              // startParam1
        qi::float_ >>              // endParam1
        qi::int_ >>                // curve2D1
        qi::int_ >>                // surface2
        qi::float_ >>              // startParam2
        qi::float_ >>              // endParam2
        qi::int_;                  // curve2D2

    ruleFreeFormConnection = 
        qi::lit("con") >>
        ruleConnectionData [boost::phoenix::bind(&OBJState::addFreeFormConnection, m_pOBJState, qi::_1)] >>
        qi::eol;
}

void OBJGrammar::setupMaterialRules()
{
    // Parses material lines. Example:
    // mtllib test.mtl
    // usemtl test

    ruleMaterialLibrary = 
        qi::lit("mtllib") >> 
        +(ruleName [boost::phoenix::bind(&OBJState::addMaterialLibrary, m_pOBJState, qi::_1)]) >> 
        qi::eol;

    ruleMaterialUse = 
        qi::lit("usemtl") >> 
        ruleName [boost::phoenix::bind(&OBJState::setMaterial, m_pOBJState, qi::_1)] >> 
        qi::eol;

    ruleMaterials = ruleMaterialLibrary | ruleMaterialUse;
}

void OBJGrammar::setupRenderStateRules()
{
    //----------------------------------------------------------------
    // Polygon Only
    //----------------------------------------------------------------

    ruleBevelInterp = 
        qi::lit("bevel") >> 
        (qi::lit("on") [boost::phoenix::bind(&OBJState::setBevelInterp, m_pOBJState, true)] | 
         qi::lit("off") [boost::phoenix::bind(&OBJState::setBevelInterp, m_pOBJState, false)]) >> 
        qi::eol;
        
    ruleColorInterp = 
        qi::lit("c_interp") >> 
        (qi::lit("on") [boost::phoenix::bind(&OBJState::setColorInterp, m_pOBJState, true)] | 
         qi::lit("off") [boost::phoenix::bind(&OBJState::setColorInterp, m_pOBJState, false)]) >> 
        qi::eol;
        
    ruleDissolveInterp = 
        qi::lit("d_interp") >> 
        (qi::lit("on") [boost::phoenix::bind(&OBJState::setDissolveInterp, m_pOBJState, true)] | 
         qi::lit("off") [boost::phoenix::bind(&OBJState::setDissolveInterp, m_pOBJState, false)]) >> 
        qi::eol;
        
    //----------------------------------------------------------------
    // General
    //----------------------------------------------------------------

    ruleSmoothing = 
        qi::lit("s") >> 
        (qi::lit("off") [boost::phoenix::bind(&OBJState::setSmoothingGroup, m_pOBJState, 0)] |
         qi::uint_ [boost::phoenix::bind(&OBJState::setSmoothingGroup, m_pOBJState, qi::_1)]) >> 
        qi::eol;

    ruleLOD = 
        qi::lit("lod") >>
        qi::int_ [boost::phoenix::bind(&OBJState::setLevelOfDetail, m_pOBJState, qi::_1)] >>
        qi::eol;

    ruleTextureMapLibrary = 
        qi::lit("maplib") >> 
        +(ruleName [boost::phoenix::bind(&OBJState::addTextureMapLibrary, m_pOBJState, qi::_1)]) >> 
        qi::eol;
        
    ruleTextureMap = 
        qi::lit("usemap") >> 
        (qi::lit("off") [boost::phoenix::bind(&OBJState::setTextureMap, m_pOBJState, "")] |
         ruleName [boost::phoenix::bind(&OBJState::setTextureMap, m_pOBJState, qi::_1)]) >> 
        qi::eol;

    ruleShadowObj = 
        qi::lit("shadow_obj") >> 
        ruleName [boost::phoenix::bind(&OBJState::setShadowObject, m_pOBJState, qi::_1)] >>
        qi::eol;

    ruleTraceObj = 
        qi::lit("trace_obj") >> 
        ruleName [boost::phoenix::bind(&OBJState::setTracingObject, m_pOBJState, qi::_1)] >>
        qi::eol;
        
    //----------------------------------------------------------------
    // Free-Form Only
    //----------------------------------------------------------------

    // Curve Technique

    qi::rule<OBJIterator, OBJSkipper> ruleCurveParametric =
        qi::lit("cparm") >>
        qi::float_ [boost::phoenix::bind(&OBJState::setTechniqueParametric, m_pOBJState, qi::_1)] >>
        qi::eol;

    qi::rule<OBJIterator, OBJSkipper> ruleCurveSpatial = 
        qi::lit("cspace") >>
        qi::float_ [boost::phoenix::bind(&OBJState::setTechniqueSpatialCurve, m_pOBJState, qi::_1)] >>
        qi::eol;

    qi::rule<OBJIterator, OBJSkipper> ruleCurveCurvature =
        qi::lit("curv") >>
        ruleVector2Data [boost::phoenix::bind(&OBJState::setTechniqueCurvatureCurve, m_pOBJState, qi::_1)] >>
        qi::eol;

    ruleFreeFormCurveTech =
        qi::lit("ctech") >>
        (ruleCurveParametric |
         ruleCurveSpatial |
         ruleCurveCurvature);

    // Surface Technique

    qi::rule<OBJIterator, OBJSkipper> ruleSurfaceParametricA =
        qi::lit("cparma") >>
        ruleVector2Data [boost::phoenix::bind(&OBJState::setTechniqueParametricA, m_pOBJState, qi::_1)] >>
        qi::eol;

    qi::rule<OBJIterator, OBJSkipper> ruleSurfaceParametricB =
        qi::lit("cparmb") >>
        qi::float_ [boost::phoenix::bind(&OBJState::setTechniqueParametricB, m_pOBJState, qi::_1)] >>
        qi::eol;

    qi::rule<OBJIterator, OBJSkipper> ruleSurfaceSpatial = 
        qi::lit("cspace") >>
        qi::float_ [boost::phoenix::bind(&OBJState::setTechniqueSpatialSurface, m_pOBJState, qi::_1)] >>
        qi::eol;

    qi::rule<OBJIterator, OBJSkipper> ruleSurfaceCurvature =
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
