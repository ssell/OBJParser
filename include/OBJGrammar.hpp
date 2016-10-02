/*
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

#ifndef __H__OBJ_PARSER_GRAMMAR__H__
#define __H__OBJ_PARSER_GRAMMAR__H__

#include "OBJGrammarSkipper.hpp"
#include "OBJStructs.hpp"

using OBJSkipper = OBJGrammarSkipper;

//------------------------------------------------------------------------------------------

class OBJState;

/**
 * \class OBJGrammar
 *
 * Spirit grammar for parsing OBJ format files.
 * 
 * Based on the format specification at:
 * http://www.paulbourke.net/dataformats/obj/
 */
class OBJGrammar : public qi::grammar<OBJIterator, OBJSkipper>
{
public:

    OBJGrammar(OBJState* state);

private:

    //--------------------------------------------------------------------------------------
    // Rule Setup Methods
    //--------------------------------------------------------------------------------------

    void setupDataRules();
    void setupGroupRules();
    void setupVertexRules();
    void setupFaceRules();
    void setupFreeFormRules();
    void setupFreeFormStart();
    void setupFreeFormBody();
    void setupFreeFormEnd();
    void setupFreeFormAttributes();
    void setupFreeFormConnections();
    void setupMaterialRules();
    void setupRenderStateRules();

    //--------------------------------------------------------------------------------------
    // Member Variables
    //--------------------------------------------------------------------------------------

    //--------------------------------------------------------------------
    // Top-Level Rules
    //--------------------------------------------------------------------
    
    qi::rule<OBJIterator, OBJSkipper> ruleStart;
    qi::rule<OBJIterator, OBJSkipper> ruleVertices;
    qi::rule<OBJIterator, OBJSkipper> ruleFaces;
    qi::rule<OBJIterator, OBJSkipper> ruleFreeForms;
    qi::rule<OBJIterator, OBJSkipper> ruleMaterials;
    qi::rule<OBJIterator, OBJSkipper> ruleRenderState;
    qi::rule<OBJIterator, OBJSkipper> ruleEmptyLine;

    //--------------------------------------------------------------------
    // Group Rules
    //--------------------------------------------------------------------

    qi::rule<OBJIterator, OBJSkipper> ruleGroup;
    qi::rule<OBJIterator, std::string(), OBJSkipper> ruleGroupName;
    
    //--------------------------------------------------------------------
    // Misc Data Rules
    //--------------------------------------------------------------------

    qi::rule<OBJIterator, OBJVector2(), OBJSkipper> ruleVector2Data;            ///< Parses "#.# #.#" of vertex point declarations (vt)
    qi::rule<OBJIterator, OBJVector3(), OBJSkipper> ruleVector3Data;            ///< Parses "#.# #.# #.#" of vertex point declarations (vn)
    qi::rule<OBJIterator, OBJVector4(), OBJSkipper> ruleVector4Data;            ///< Parses "#.# #.# #.# #.#" where the fourth element is optional (v)
    qi::rule<OBJIterator, OBJVertexGroup(), OBJSkipper> ruleVertexGroupData;    ///< Parses "#/#/#" of vertex group declarations. Secondary elements (and their slashes) are optional.
    qi::rule<OBJIterator, int32_t(), OBJSkipper> ruleIndexValue;
    qi::rule<OBJIterator, std::vector<OBJVertexGroup>(), OBJSkipper> ruleIndexList;
    qi::rule<OBJIterator, std::string(), OBJSkipper> ruleName;

    //--------------------------------------------------------------------
    // Vertex Rules
    //--------------------------------------------------------------------

    qi::rule<OBJIterator, OBJSkipper> ruleVertexSpatial;
    qi::rule<OBJIterator, OBJSkipper> ruleVertexTexture;
    qi::rule<OBJIterator, OBJSkipper> ruleVertexNormal;
    qi::rule<OBJIterator, OBJSkipper> ruleVertexParameter;

    //--------------------------------------------------------------------
    // Face Rules
    //--------------------------------------------------------------------

    qi::rule<OBJIterator, OBJFace(), OBJSkipper> ruleFaceData;
    qi::rule<OBJIterator, OBJSkipper> ruleFace;

    qi::rule<OBJIterator, OBJSkipper> ruleLine;
    qi::rule<OBJIterator, OBJSkipper> rulePoint;

    //--------------------------------------------------------------------
    // Free-Form Rules
    //--------------------------------------------------------------------

    // start statements
    
    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormStart;

    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormCurve;  
    qi::rule<OBJIterator, OBJCurve(), OBJSkipper> ruleFreeFormCurveData;

    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormCurve2D;  
    qi::rule<OBJIterator, std::vector<int32_t>(), OBJSkipper> ruleFreeFormCurve2DData;

    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormSurface; 
    qi::rule<OBJIterator, OBJSurface(), OBJSkipper> ruleFreeFormSurfaceData;

    // body statements (may only appear between start and end statements)

    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormBody;

    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormParameter;       // parm
    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormTrim;            // trim
    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormHole;            // hole
    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormSpecialCurve;    // scrv
    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormSpecialPoint;    // sp

    // end statement

    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormEnd;
    
    // attribute statements

    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormAttributes;

    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormType;            // cstype
    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormDegree;          // deg
    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormStep;            // step
    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormBasisMatrix;     // bmat
    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormMergeGroup;      // mg

    // connections

    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormConnection;      // con

    //--------------------------------------------------------------------
    // Material Rules
    //--------------------------------------------------------------------

    qi::rule<OBJIterator, OBJSkipper> ruleMaterialLibrary;
    qi::rule<OBJIterator, OBJSkipper> ruleMaterialUse;

    //--------------------------------------------------------------------
    // Render Attribute Rules
    //--------------------------------------------------------------------
    
    // polygon only

    qi::rule<OBJIterator, OBJSkipper> ruleBevelInterp;
    qi::rule<OBJIterator, OBJSkipper> ruleColorInterp;
    qi::rule<OBJIterator, OBJSkipper> ruleDissolveInterp;

    // all

    qi::rule<OBJIterator, OBJSkipper> ruleSmoothing;
    qi::rule<OBJIterator, OBJSkipper> ruleLOD;
    qi::rule<OBJIterator, OBJSkipper> ruleTextureMapLibrary;
    qi::rule<OBJIterator, OBJSkipper> ruleTextureMap;
    qi::rule<OBJIterator, OBJSkipper> ruleShadowObj;
    qi::rule<OBJIterator, OBJSkipper> ruleTraceObj;

    // free-form only

    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormCurveTech; 
    qi::rule<OBJIterator, OBJSkipper> ruleFreeFormSurfaceTech;  

    //--------------------------------------------------------------------
    // Non-Rule Members
    //--------------------------------------------------------------------

    OBJState* m_pOBJState;

private:

};

//------------------------------------------------------------------------------------------

#endif

