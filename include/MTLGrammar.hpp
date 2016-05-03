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
#include "MTLGrammarSkipper.hpp"
#include "OBJMaterial.hpp"

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
        : MTLGrammar::base_type(ruleStart),
          m_pOBJState(state)
    {
        setupGeneralRules();
        setupNewMaterialRules();
        setupColorIlluminationRules();
        setupTextureMapRules();
        setupReflectionMapRules();

        ruleStart = +(ruleNewMaterial |
                      ruleColorIllumination |
                      ruleTextureMap |
                      ruleReflectionMap |
                      qi::eol);
    }

    void resetCurrentTexture()
    {
        m_CurrentTexture = OBJTextureDescriptor();
    }

    void finishCurrentMaterial()
    {
        if(m_pOBJState)
        {
            m_pOBJState->setMaterial(m_CurrentMaterial.getName(), m_CurrentMaterial);
        }

        m_CurrentMaterial = OBJMaterial();
    }

protected:

    //--------------------------------------------------------------------------------------
    // Rule Setup Methods
    //--------------------------------------------------------------------------------------

    void setupGeneralRules()
    {
        ruleString = *(qi::char_ - qi::space);

        ruleVector2Data = 
            qi::float_ >> 
            qi::float_;

        ruleVector3Data = 
            qi::float_ >> 
            qi::float_ >> 
            qi::float_;

        ruleBoolOnOffFalse =
            (qi::omit[qi::lit("on")] [qi::_val = true] |
            (qi::omit[qi::lit("off")] [qi::_val = false]) |
            (qi::attr(false)));

        ruleBoolOnOffTrue =
            (qi::omit[qi::lit("on")] [qi::_val = true] |
            (qi::omit[qi::lit("off")] [qi::_val = false]) |
            (qi::attr(true)));
    }

    void setupNewMaterialRules()
    {
        ruleNewMaterial =
            qi::lit("newmtl ") [boost::phoenix::bind(&MTLGrammar::finishCurrentMaterial, this)] >>
            ruleString [boost::phoenix::bind(&OBJMaterial::setName, &m_CurrentMaterial, qi::_1)] >>
            qi::eol;
    }

    void setupColorIlluminationRules()
    {
        ruleRFL =
            ruleString >>
            (qi::float_ | qi::attr(1.0f));

        //----------------------------------------------------------------
        // Ambient Color
        //----------------------------------------------------------------

        ruleAmbientRGB =
            ruleVector3Data [boost::phoenix::bind(&OBJMaterial::setAmbientReflectivityRGB, &m_CurrentMaterial, qi::_1)];

        ruleAmbientSpectral =
            qi::lit("spectral") >>
            ruleRFL [boost::phoenix::bind(&OBJMaterial::setAmbientReflectivityRFL, &m_CurrentMaterial, qi::_1)];

        ruleAmbientXYZ =
            qi::lit("xyz") >>
            ruleVector3Data [boost::phoenix::bind(&OBJMaterial::setAmbientReflectivityXYZ, &m_CurrentMaterial, qi::_1)];

        ruleAmbient = 
            qi::lit("Ka") >> 
            (ruleAmbientRGB | ruleAmbientSpectral | ruleAmbientXYZ);

        //----------------------------------------------------------------
        // Diffuse Color
        //----------------------------------------------------------------

        ruleDiffuseRGB = 
            ruleVector3Data [boost::phoenix::bind(&OBJMaterial::setDiffuseReflectivityRGB, &m_CurrentMaterial, qi::_1)];

        ruleDiffuseSpectral =
            qi::lit("spectral") >>
            ruleRFL [boost::phoenix::bind(&OBJMaterial::setDiffuseReflectivityRFL, &m_CurrentMaterial, qi::_1)];

        ruleDiffuseXYZ =
            qi::lit("xyz") >>
            ruleVector3Data [boost::phoenix::bind(&OBJMaterial::setDiffuseReflectivityXYZ, &m_CurrentMaterial, qi::_1)];

        ruleDiffuse = 
            qi::lit("Kd") >> 
            (ruleDiffuseRGB | 
             ruleDiffuseSpectral | 
             ruleDiffuseXYZ);
        
        //----------------------------------------------------------------
        // Specular Color
        //----------------------------------------------------------------

        ruleSpecularRGB = 
            ruleVector3Data [boost::phoenix::bind(&OBJMaterial::setSpecularReflectivityRGB, &m_CurrentMaterial, qi::_1)];

        ruleSpecularSpectral =
            qi::lit("spectral") >>
            ruleRFL [boost::phoenix::bind(&OBJMaterial::setSpecularReflectivityRFL, &m_CurrentMaterial, qi::_1)];

        ruleSpecularXYZ =
            qi::lit("xyz") >>
            ruleVector3Data [boost::phoenix::bind(&OBJMaterial::setSpecularReflectivityXYZ, &m_CurrentMaterial, qi::_1)];

        ruleSpecular = 
            qi::lit("Ks") >> 
            (ruleSpecularRGB | 
             ruleSpecularSpectral | 
             ruleSpecularXYZ);

        //----------------------------------------------------------------
        // Transmission Filter
        //----------------------------------------------------------------

        ruleTransmissionRGB = 
            ruleVector3Data [boost::phoenix::bind(&OBJMaterial::setTransmissionFilterRGB, &m_CurrentMaterial, qi::_1)];

        ruleTransmissionSpectral =
            qi::lit("spectral") >>
            ruleRFL [boost::phoenix::bind(&OBJMaterial::setTransmissionFilterRFL, &m_CurrentMaterial, qi::_1)];

        ruleTransmissionXYZ =
            qi::lit("xyz") >>
            ruleVector3Data [boost::phoenix::bind(&OBJMaterial::setTransmissionFilterXYZ, &m_CurrentMaterial, qi::_1)];

        ruleTransmission = 
            qi::lit("Tf") >> 
            (ruleTransmissionRGB | 
             ruleTransmissionSpectral | 
             ruleTransmissionXYZ);

        //----------------------------------------------------------------
        // Dissolve
        //----------------------------------------------------------------

        ruleDissolveHalo =
            (qi::omit[qi::lit("-halo")] [qi::_val = true] |
            (qi::attr(false)));

        ruleDissolveData = 
            ruleDissolveHalo >> 
            qi::float_;

        ruleDissolve =
            qi::lit("d") >>
            ruleDissolveData [boost::phoenix::bind(&OBJMaterial::setDissolve, &m_CurrentMaterial, qi::_1)];

        //----------------------------------------------------------------
        // Illumination
        //----------------------------------------------------------------

        ruleIllumination = 
            qi::lit("illum") >> 
            qi::uint_ [boost::phoenix::bind(&OBJMaterial::setIlluminationModel, &m_CurrentMaterial, qi::_1)];

        //----------------------------------------------------------------
        // Other
        //----------------------------------------------------------------

        ruleSpecularExponent =
            qi::lit("Ns") >>
            qi::uint_ [boost::phoenix::bind(&OBJMaterial::setSpecularExponent, &m_CurrentMaterial, qi::_1)];

        ruleSharpness =
            qi::lit("sharpness") >>
            qi::uint_ [boost::phoenix::bind(&OBJMaterial::setSharpness, &m_CurrentMaterial, qi::_1)];

        ruleOpticalDensity =
            qi::lit("Ni") >>
            qi::float_ [boost::phoenix::bind(&OBJMaterial::setOpticalDensity, &m_CurrentMaterial, qi::_1)];

        //----------------------------------------------------------------

        ruleColorIllumination = 
            (ruleAmbient | 
             ruleDiffuse | 
             ruleSpecular | 
             ruleTransmission | 
             ruleIllumination |
             ruleDissolve |
             ruleSpecularExponent |
             ruleSharpness |
             ruleOpticalDensity) >>
             qi::eol;
    }

    void setupTextureMapRules()
    {
        setupTextureMapFlagRules();

        //----------------------------------------------------------------
        // Texture Map Types
        //----------------------------------------------------------------

        ruleTextureMapBody =
            ruleTextureFlags >>
            ruleString >>
            qi::eol;

        ruleTextureMapAmbient =
            qi::lit("map_Ka") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setAmbientTexture, &m_CurrentMaterial, m_CurrentTexture)];

        ruleTextureMapDiffuse =
            qi::lit("map_Kd") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setDiffuseTexture, &m_CurrentMaterial, m_CurrentTexture)];

        ruleTextureMapSpecular =
            qi::lit("map_Ks") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setSpecularTexture, &m_CurrentMaterial, m_CurrentTexture)];

        ruleTextureMapSpecularExponent =
            qi::lit("map_Ns") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setSpecularExponentTexture, &m_CurrentMaterial, m_CurrentTexture)];

        ruleTextureMapDissolve =
            qi::lit("map_d") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setDissolveTexture, &m_CurrentMaterial, m_CurrentTexture)];

        ruleTextureMapDecal =
            qi::lit("decal") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setDecalTexture, &m_CurrentMaterial, m_CurrentTexture)];

        ruleTextureMapDisplacement =
            qi::lit("disp") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setDisplacementTexture, &m_CurrentMaterial, m_CurrentTexture)];

        ruleTextureMapBump =
            qi::lit("bump") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setBumpTexture, &m_CurrentMaterial, m_CurrentTexture)];

        //----------------------------------------------------------------
        // Anti-Aliasing
        //----------------------------------------------------------------

        ruleTextureAntiAliasing =
            qi::lit("map_aat") >>
            ruleBoolOnOffFalse [boost::phoenix::bind(&OBJMaterial::setAntiAliasing, &m_CurrentMaterial, qi::_1)] >>
            qi::eol;

        //----------------------------------------------------------------

        ruleTextureMap =
            (ruleTextureMapAmbient |
             ruleTextureMapDiffuse |
             ruleTextureMapSpecular |
             ruleTextureMapSpecularExponent |
             ruleTextureMapDissolve |
             ruleTextureMapDecal |
             ruleTextureMapDisplacement |
             ruleTextureMapBump |
             ruleTextureAntiAliasing);
    }

    void setupTextureMapFlagRules()
    {
        //----------------------------------------------------------------
        // On / Off Rules
        //----------------------------------------------------------------

        ruleTextureFlagBlendU =
            qi::omit[qi::lit("-blendu")] >>
            ruleBoolOnOffTrue [boost::phoenix::bind(&OBJTextureDescriptor::setBlendU, &m_CurrentTexture, qi::_1)];
        
        ruleTextureFlagBlendV =
            qi::omit[qi::lit("-blendv")] >>
            ruleBoolOnOffTrue [boost::phoenix::bind(&OBJTextureDescriptor::setBlendV, &m_CurrentTexture, qi::_1)];
        
        ruleTextureFlagColorCorrection =
            qi::omit[qi::lit("-cc")] >>
            ruleBoolOnOffFalse [boost::phoenix::bind(&OBJTextureDescriptor::setColorCorrection, &m_CurrentTexture, qi::_1)];
        
        ruleTextureFlagClamp =
            qi::omit[qi::lit("-clamp")] >>
            ruleBoolOnOffFalse [boost::phoenix::bind(&OBJTextureDescriptor::setClamp, &m_CurrentTexture, qi::_1)];

        //----------------------------------------------------------------
        // Vector Rules
        //----------------------------------------------------------------

        ruleTextureFlagOffset =
            qi::omit[qi::lit("-o")] >>
            ruleVector3Data [boost::phoenix::bind(&OBJTextureDescriptor::setOffset, &m_CurrentTexture, qi::_1)];

        ruleTextureFlagScale =
            qi::omit[qi::lit("-s")] >>
            ruleVector3Data [boost::phoenix::bind(&OBJTextureDescriptor::setScale, &m_CurrentTexture, qi::_1)];

        ruleTextureFlagTurbulence =
            qi::omit[qi::lit("-t")] >>
            ruleVector3Data [boost::phoenix::bind(&OBJTextureDescriptor::setTurbulence, &m_CurrentTexture, qi::_1)];

        //----------------------------------------------------------------
        // Other
        //----------------------------------------------------------------

        ruleTextureFlagRangeMod =
            qi::omit[qi::lit("-mm")] >>
            ruleVector2Data [boost::phoenix::bind(&OBJTextureDescriptor::setRangeMod, &m_CurrentTexture, qi::_1)];

        ruleTextureFlagResolution =
            qi::omit[qi::lit("-texres")] >>
            qi::uint_ [boost::phoenix::bind(&OBJTextureDescriptor::setResolution, &m_CurrentTexture, qi::_1)];

        ruleTextureFlagBumpMultiplier =
            qi::omit[qi::lit("-bm")] >>
            qi::float_ [boost::phoenix::bind(&OBJTextureDescriptor::setBumpMultiplier, &m_CurrentTexture, qi::_1)];

        ruleTextureFlagBoost =
            qi::omit[qi::lit("-boost")] >>
            qi::float_ [boost::phoenix::bind(&OBJTextureDescriptor::setBoost, &m_CurrentTexture, qi::_1)];

        //----------------------------------------------------------------

        ruleTextureFlags = 
            *(ruleTextureFlagBlendU |
              ruleTextureFlagBlendV |
              ruleTextureFlagColorCorrection |
              ruleTextureFlagClamp |
              ruleTextureFlagOffset | 
              ruleTextureFlagScale |
              ruleTextureFlagTurbulence |
              ruleTextureFlagRangeMod |
              ruleTextureFlagResolution |
              ruleTextureFlagBumpMultiplier |
              ruleTextureFlagBoost);
    }

    void setupReflectionMapRules()
    {
        ruleReflectionMapSphere =
            qi::lit("refl -type sphere") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapSphere, &m_CurrentMaterial, m_CurrentTexture)];

        ruleReflectionMapCubeTop =
            qi::lit("refl -type cube_top") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapCubeSide, &m_CurrentMaterial, OBJReflectionMapCubeSide::Top, m_CurrentTexture)];

        ruleReflectionMapCubeBottom =
            qi::lit("refl -type cube_bottom") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapCubeSide, &m_CurrentMaterial, OBJReflectionMapCubeSide::Bottom, m_CurrentTexture)];

        ruleReflectionMapCubeFront =
            qi::lit("refl -type cube_front") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapCubeSide, &m_CurrentMaterial, OBJReflectionMapCubeSide::Front, m_CurrentTexture)];

        ruleReflectionMapCubeBack =
            qi::lit("refl -type cube_back") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapCubeSide, &m_CurrentMaterial, OBJReflectionMapCubeSide::Back, m_CurrentTexture)];

        ruleReflectionMapCubeLeft =
            qi::lit("refl -type cube_left") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapCubeSide, &m_CurrentMaterial, OBJReflectionMapCubeSide::Left, m_CurrentTexture)];

        ruleReflectionMapCubeRight =
            qi::lit("refl -type cube_right") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
            ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapCubeSide, &m_CurrentMaterial, OBJReflectionMapCubeSide::Right, m_CurrentTexture)];

        ruleReflectionMap =
            (ruleReflectionMapSphere |
             ruleReflectionMapCubeTop |
             ruleReflectionMapCubeBottom |
             ruleReflectionMapCubeFront |
             ruleReflectionMapCubeBack |
             ruleReflectionMapCubeLeft |
             ruleReflectionMapCubeRight);
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
    qi::rule<Iterator, OBJVector2(), Skipper> ruleVector2Data;
    qi::rule<Iterator, OBJVector3(), Skipper> ruleVector3Data;
    qi::rule<Iterator, bool(), Skipper> ruleBoolOnOffTrue;           ///< Parses flag value "on" and "off". If none specified, returns TRUE by default
    qi::rule<Iterator, bool(), Skipper> ruleBoolOnOffFalse;          ///< Parses flag value "on" and "off". If none specified, returns FALSE by default

    //--------------------------------------------------------------------
    // Color / Illumination Rules

    qi::rule<Iterator, Skipper> ruleColor;
    qi::rule<Iterator, Skipper> ruleIllumination;
    qi::rule<Iterator, OBJMaterialPropertyRFL(), Skipper> ruleRFL;

    qi::rule<Iterator, Skipper> ruleAmbientRGB;
    qi::rule<Iterator, Skipper> ruleAmbientSpectral;
    qi::rule<Iterator, Skipper> ruleAmbientXYZ;
    qi::rule<Iterator, Skipper> ruleAmbient;

    qi::rule<Iterator, Skipper> ruleDiffuseRGB;
    qi::rule<Iterator, Skipper> ruleDiffuseSpectral;
    qi::rule<Iterator, Skipper> ruleDiffuseXYZ;
    qi::rule<Iterator, Skipper> ruleDiffuse;

    qi::rule<Iterator, Skipper> ruleSpecularRGB;
    qi::rule<Iterator, Skipper> ruleSpecularSpectral;
    qi::rule<Iterator, Skipper> ruleSpecularXYZ;
    qi::rule<Iterator, Skipper> ruleSpecular;

    qi::rule<Iterator, Skipper> ruleTransmissionRGB;
    qi::rule<Iterator, Skipper> ruleTransmissionSpectral;
    qi::rule<Iterator, Skipper> ruleTransmissionXYZ;
    qi::rule<Iterator, Skipper> ruleTransmission;

    qi::rule<Iterator, bool(), Skipper> ruleDissolveHalo;
    qi::rule<Iterator, OBJMaterialDissolve(), Skipper> ruleDissolveData;
    qi::rule<Iterator, Skipper> ruleDissolve;

    qi::rule<Iterator, Skipper> ruleSpecularExponent;
    qi::rule<Iterator, Skipper> ruleSharpness;
    qi::rule<Iterator, Skipper> ruleOpticalDensity;

    //--------------------------------------------------------------------
    // Texture Map Rules
    
    // Top Texture Rules

    qi::rule<Iterator, Skipper> ruleTextureFlags;
    qi::rule<Iterator, Skipper> ruleTextureMapBody;
    qi::rule<Iterator, Skipper> ruleTextureAntiAliasing;

    // Individual Texture Map Rules

    qi::rule<Iterator, Skipper> ruleTextureMapAmbient;
    qi::rule<Iterator, Skipper> ruleTextureMapDiffuse;
    qi::rule<Iterator, Skipper> ruleTextureMapSpecular;
    qi::rule<Iterator, Skipper> ruleTextureMapSpecularExponent;
    qi::rule<Iterator, Skipper> ruleTextureMapDissolve;
    qi::rule<Iterator, Skipper> ruleTextureMapDecal;
    qi::rule<Iterator, Skipper> ruleTextureMapDisplacement;
    qi::rule<Iterator, Skipper> ruleTextureMapBump;

    // Optional Flags

    qi::rule<Iterator, bool(), Skipper> ruleTextureFlagBlendU;
    qi::rule<Iterator, bool(), Skipper> ruleTextureFlagBlendV;
    qi::rule<Iterator, bool(), Skipper> ruleTextureFlagColorCorrection;
    qi::rule<Iterator, bool(), Skipper> ruleTextureFlagClamp;

    qi::rule<Iterator, OBJVector3(), Skipper> ruleTextureFlagOffset;
    qi::rule<Iterator, OBJVector3(), Skipper> ruleTextureFlagScale;
    qi::rule<Iterator, OBJVector3(), Skipper> ruleTextureFlagTurbulence;

    qi::rule<Iterator, OBJVector2(), Skipper> ruleTextureFlagRangeMod;

    qi::rule<Iterator, uint32_t(), Skipper> ruleTextureFlagResolution;

    qi::rule<Iterator, float(), Skipper> ruleTextureFlagBumpMultiplier; 
    qi::rule<Iterator, float(), Skipper> ruleTextureFlagBoost;

    //--------------------------------------------------------------------
    // Reflection Map Rules
    
    qi::rule<Iterator, Skipper> ruleReflectionMapSphere;
    qi::rule<Iterator, Skipper> ruleReflectionMapCubeTop;
    qi::rule<Iterator, Skipper> ruleReflectionMapCubeBottom;
    qi::rule<Iterator, Skipper> ruleReflectionMapCubeFront;
    qi::rule<Iterator, Skipper> ruleReflectionMapCubeBack;
    qi::rule<Iterator, Skipper> ruleReflectionMapCubeLeft;
    qi::rule<Iterator, Skipper> ruleReflectionMapCubeRight;

    //--------------------------------------------------------------------
    // Non-Rule Members

    OBJState* m_pOBJState;
    OBJMaterial m_CurrentMaterial;                                   ///< The current material being built by the parser
    OBJTextureDescriptor m_CurrentTexture;

private:

};

//------------------------------------------------------------------------------------------

#endif