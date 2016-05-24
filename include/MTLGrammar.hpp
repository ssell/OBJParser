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

#ifndef __H__OBJ_PARSER_MTL_GRAMMAR__H__
#define __H__OBJ_PARSER_MTL_GRAMMAR__H__

#include "MTLGrammarSkipper.hpp"
#include "OBJMaterial.hpp"

using MTLSkipper = MTLGrammarSkipper;

//------------------------------------------------------------------------------------------

class OBJState;

/**
 * \class MTLGrammar
 *
 * Spirit grammar for parsing MTL format files.
 *
 * Based on the format specification at:
 * http://www.paulbourke.net/dataformats/mtl/
 */
class MTLGrammar : public qi::grammar<OBJIterator, MTLSkipper>
{
public:

    MTLGrammar(OBJState* state);

    void resetCurrentTexture();
    void finishCurrentMaterial();

protected:

    //--------------------------------------------------------------------------------------
    // Rule Setup Methods
    //--------------------------------------------------------------------------------------

    void setupGeneralRules();
    void setupNewMaterialRules();
    void setupColorIlluminationRules();
    void setupTextureMapRules();
    void setupTextureMapFlagRules();
    void setupReflectionMapRules();

    //--------------------------------------------------------------------------------------
    // Member Variables
    //--------------------------------------------------------------------------------------
    
    //--------------------------------------------------------------------
    // Top-Level Rules

    qi::rule<OBJIterator, MTLSkipper> ruleStart;
    qi::rule<OBJIterator, MTLSkipper> ruleNewMaterial;
    qi::rule<OBJIterator, MTLSkipper> ruleColorIllumination;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureMap;
    qi::rule<OBJIterator, MTLSkipper> ruleReflectionMap;

    //--------------------------------------------------------------------
    // General Rules

    qi::rule<OBJIterator, std::string(), MTLSkipper> ruleString;
    qi::rule<OBJIterator, std::string(), MTLSkipper> ruleStringNoSkip;
    qi::rule<OBJIterator, OBJVector2(), MTLSkipper> ruleVector2Data;
    qi::rule<OBJIterator, OBJVector3(), MTLSkipper> ruleVector3Data;
    qi::rule<OBJIterator, bool(), MTLSkipper> ruleBoolOnOffTrue;           ///< Parses flag value "on" and "off". If none specified, returns TRUE by default
    qi::rule<OBJIterator, bool(), MTLSkipper> ruleBoolOnOffFalse;          ///< Parses flag value "on" and "off". If none specified, returns FALSE by default

    //--------------------------------------------------------------------
    // Color / Illumination Rules

    qi::rule<OBJIterator, MTLSkipper> ruleColor;
    qi::rule<OBJIterator, MTLSkipper> ruleIllumination;
    qi::rule<OBJIterator, OBJMaterialPropertyRFL(), MTLSkipper> ruleRFL;

    qi::rule<OBJIterator, MTLSkipper> ruleAmbientRGB;
    qi::rule<OBJIterator, MTLSkipper> ruleAmbientSpectral;
    qi::rule<OBJIterator, MTLSkipper> ruleAmbientXYZ;
    qi::rule<OBJIterator, MTLSkipper> ruleAmbient;

    qi::rule<OBJIterator, MTLSkipper> ruleDiffuseRGB;
    qi::rule<OBJIterator, MTLSkipper> ruleDiffuseSpectral;
    qi::rule<OBJIterator, MTLSkipper> ruleDiffuseXYZ;
    qi::rule<OBJIterator, MTLSkipper> ruleDiffuse;

    qi::rule<OBJIterator, MTLSkipper> ruleSpecularRGB;
    qi::rule<OBJIterator, MTLSkipper> ruleSpecularSpectral;
    qi::rule<OBJIterator, MTLSkipper> ruleSpecularXYZ;
    qi::rule<OBJIterator, MTLSkipper> ruleSpecular;

    qi::rule<OBJIterator, MTLSkipper> ruleEmissiveRGB;
    qi::rule<OBJIterator, MTLSkipper> ruleEmissiveSpectral;
    qi::rule<OBJIterator, MTLSkipper> ruleEmissiveXYZ;
    qi::rule<OBJIterator, MTLSkipper> ruleEmissive;

    qi::rule<OBJIterator, MTLSkipper> ruleTransmissionRGB;
    qi::rule<OBJIterator, MTLSkipper> ruleTransmissionSpectral;
    qi::rule<OBJIterator, MTLSkipper> ruleTransmissionXYZ;
    qi::rule<OBJIterator, MTLSkipper> ruleTransmission;

    qi::rule<OBJIterator, bool(), MTLSkipper> ruleDissolveHalo;
    qi::rule<OBJIterator, OBJMaterialDissolve(), MTLSkipper> ruleDissolveData;
    qi::rule<OBJIterator, MTLSkipper> ruleDissolve;

    qi::rule<OBJIterator, MTLSkipper> ruleTransparency;

    qi::rule<OBJIterator, MTLSkipper> ruleSpecularExponent;
    qi::rule<OBJIterator, MTLSkipper> ruleSharpness;
    qi::rule<OBJIterator, MTLSkipper> ruleOpticalDensity;

    //--------------------------------------------------------------------
    // Texture Map Rules
    
    // Top Texture Rules

    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlags;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureMapBody;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureAntiAliasing;

    // Individual Texture Map Rules

    qi::rule<OBJIterator, MTLSkipper> ruleTextureMapAmbient;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureMapDiffuse;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureMapSpecular;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureMapSpecularExponent;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureMapEmissive;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureMapDissolve;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureMapDecal;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureMapDisplacement;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureMapBump;

    // Optional Flags

    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlagBlendU;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlagBlendV;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlagColorCorrection;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlagClamp;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlagOffset;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlagScale;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlagTurbulence;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlagRangeMod;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlagResolution;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlagBumpMultiplier; 
    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlagBoost;
    qi::rule<OBJIterator, MTLSkipper> ruleTextureFlagimfchan;

    //--------------------------------------------------------------------
    // Reflection Map Rules
    
    qi::rule<OBJIterator, MTLSkipper> ruleReflectionMapSphere;
    qi::rule<OBJIterator, MTLSkipper> ruleReflectionMapCubeTop;
    qi::rule<OBJIterator, MTLSkipper> ruleReflectionMapCubeBottom;
    qi::rule<OBJIterator, MTLSkipper> ruleReflectionMapCubeFront;
    qi::rule<OBJIterator, MTLSkipper> ruleReflectionMapCubeBack;
    qi::rule<OBJIterator, MTLSkipper> ruleReflectionMapCubeLeft;
    qi::rule<OBJIterator, MTLSkipper> ruleReflectionMapCubeRight;

    //--------------------------------------------------------------------
    // Non-Rule Members

    OBJState* m_pOBJState;
    OBJMaterial m_CurrentMaterial;                                   ///< The current material being built by the parser
    OBJTextureDescriptor m_CurrentTexture;

private:

};

//------------------------------------------------------------------------------------------

#endif