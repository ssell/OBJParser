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

        ruleVector3Data = 
            qi::skip(qi::blank)[
                qi::float_ >> 
                qi::float_ >> 
                qi::float_ >> 
                *(qi::char_ - qi::eol)];

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
            qi::lit("newmtl ") >>
            ruleString >>
            qi::eol;
    }

    void setupColorIlluminationRules()
    {
        //----------------------------------------------------------------
        // Ambient Color
        //----------------------------------------------------------------

        ruleAmbientRGB = 
            ruleVector3Data [boost::phoenix::bind(&OBJMaterial::setAmbientReflectivityRGB, &m_CurrentMaterial, qi::_1)] >> 
            qi::eol;

        ruleAmbientSpectral = 
            qi::lit("spectral") >> 
            ruleString >> 
            -(float_) >> 
            qi::eol;

        ruleAmbientXYZ =
            qi::lit("xyz") [boost::phoenix::bind(&OBJMaterial::setAmbientReflectivityXYZ, &m_CurrentMaterial, qi::_1)] >>
            ruleVector3Data >>
            qi::eol;

        ruleAmbient = 
            qi::lit("Ka") >> 
            (ruleAmbientRGB | 
             ruleAmbientSpectral | 
             ruleAmbientXYZ);

        //----------------------------------------------------------------
        // Diffuse Color
        //----------------------------------------------------------------

        ruleDiffuseRGB = 
            ruleVector3Data >> 
            qi::eol;

        ruleDiffuseSpectral = 
            qi::lit("spectral") >> 
            ruleString >> 
            -(float_) >> 
            qi::eol;

        ruleDiffuseXYZ =
            qi::lit("xyz") >>
            ruleVector3Data >>
            qi::eol;

        ruleDiffuse = 
            qi::lit("Kd") >> 
            (ruleDiffuseRGB | 
             ruleDiffuseSpectral | 
             ruleDiffuseXYZ);
        
        //----------------------------------------------------------------
        // Specular Color
        //----------------------------------------------------------------

        ruleSpecularRGB = 
            ruleVector3Data >> 
            qi::eol;

        ruleSpecularSpectral = 
            qi::lit("spectral") >> 
            ruleString >> 
            -(float_) >> 
            qi::eol;

        ruleSpecularXYZ =
            qi::lit("xyz") >>
            ruleVector3Data >>
            qi::eol;

        ruleSpecular = 
            qi::lit("Ks") >> 
            (ruleSpecularRGB | 
             ruleSpecularSpectral | 
             ruleSpecularXYZ);

        //----------------------------------------------------------------
        // Transmission Filter
        //----------------------------------------------------------------

        ruleTransmissionRGB = 
            ruleVector3Data >> 
            qi::eol;

        ruleTransmissionSpectral = 
            qi::lit("spectral") >> 
            ruleString >> 
            -(float_) >> 
            qi::eol;

        ruleTransmissionXYZ =
            qi::lit("xyz") >>
            ruleVector3Data >>
            qi::eol;

        ruleTransmission = 
            qi::lit("Tf") >> 
            (ruleTransmissionRGB | 
             ruleTransmissionSpectral | 
             ruleTransmissionXYZ);

        //----------------------------------------------------------------
        // Illumination
        //----------------------------------------------------------------

        ruleIllumination = 
            qi::lit("illum") >> 
            qi::uint_ >> 
            qi::eol;

        //----------------------------------------------------------------
        // Dissolve
        //----------------------------------------------------------------

        ruleDissolveFactor = qi::float_;
        ruleDissolveHalo = (qi::bool_ | qi::attr(false));

        ruleDissolve =
            qi::lit("d") >>
            ruleDissolveHalo >>
            ruleDissolveFactor >>
            qi::omit[qi::eol];

        //----------------------------------------------------------------
        // Other
        //----------------------------------------------------------------

        ruleSpecularExponent =
            qi::lit("Ns") >>
            qi::uint_ >>
            qi::eol;

        ruleSharpness =
            qi::lit("sharpness") >>
            qi::uint_ >>
            qi::eol;

        ruleOpticalDensity =
            qi::lit("optical_density") >>
            qi::float_ >>
            qi::eol

        //----------------------------------------------------------------

        ruleColorIllumination = 
            qi::skip(qi::blank)[
            (ruleAmbient | 
             ruleDiffuse | 
             ruleSpecular | 
             ruleTransmission | 
             ruleIllumination |
             ruleDissolve |
             ruleSpecularExponent |
             ruleSharpness |
             ruleOpticalDensity)];
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
            qi::lit("map_Ka") >>
            ruleTextureMapBody;

        ruleTextureMapDiffuse =
            qi::lit("map_Kd") >>
            ruleTextureMapBody;

        ruleTextureMapSpecular =
            qi::lit("map_Ks") >>
            ruleTextureMapBody;

        ruleTextureMapSpecularExponent =
            qi::lit("map_Ns") >>
            ruleTextureMapBody;

        ruleTextureMapDissolve =
            qi::lit("map_d") >>
            ruleTextureMapBody;

        ruleTextureMapDecal =
            qi::lit("decal") >>
            ruleTextureMapBody;

        ruleTextureMapDisplacement =
            qi::lit("disp") >>
            ruleTextureMapBody;

        ruleTextureMapBump =
            qi::lit("bump") >>
            ruleTextureMapBody;

        //----------------------------------------------------------------
        // Anti-Aliasing
        //----------------------------------------------------------------

        ruleTextureAntiAliasing =
            qi::lit("map_aat") >>
            ruleBoolOnOffFalse;

        //----------------------------------------------------------------

        ruleTextureMap =
            qi::skip(qi::blank)[
                (ruleTextureMapAmbient |
                 ruleTextureMapDiffuse |
                 ruleTextureMapSpecular |
                 ruleTextureMapSpecularExponent |
                 ruleTextureMapDissolve |
                 ruleTextureMapDecal |
                 ruleTextureMapDisplacement |
                 ruleTextureMapBump |
                 ruleTextureAntiAliasing)];
    }

    void setupTextureMapFlagRules()
    {
        //----------------------------------------------------------------
        // On / Off Rules
        //----------------------------------------------------------------

        ruleTextureFlagBlendU =
            qi::omit[qi::lit("-blendu")] >>
            ruleBoolOnOffTrue;
        
        ruleTextureFlagBlendV =
            qi::omit[qi::lit("-blendv")] >>
            ruleBoolOnOffTrue;
        
        ruleTextureFlagColorCorrection =
            qi::omit[qi::lit("-cc")] >>
            ruleBoolOnOffFalse;
        
        ruleTextureFlagClamp =
            qi::omit[qi::lit("-clamp")] >>
            ruleBoolOnOffFalse;

        //----------------------------------------------------------------
        // Vector Rules
        //----------------------------------------------------------------

        ruleTextureFlagOffset =
            qi::omit[qi::lit("-o")] >>
            qi::float_ >> qi::float_ >> qi::float_;

        ruleTextureFlagScale =
            qi::omit[qi::lit("-s")] >>
            qi::float_ >> qi::float_ >> qi::float_;

        ruleTextureFlagTurbulence =
            qi::omit[qi::lit("-t")] >>
            qi::float_ >> qi::float_ >> qi::float_;

        //----------------------------------------------------------------
        // Other
        //----------------------------------------------------------------

        ruleTextureFlagRangeMod =
            qi::omit[qi::lit("-mm")] >>
            qi::float_ >> qi::float_;

        ruleTextureFlagResolution =
            qi::omit[qi::lit("-texres")] >>
            qi::uint_;

        ruleTextureFlagBumpMultiplier =
            qi::omit[qi::lit("-bm")] >>
            qi::float_;

        ruleTextureFlagBoost =
            qi::omit[qi::lit("-boost")] >>
            qi::float_;

        //----------------------------------------------------------------

        ruleTextureFlags = 
            qi::skip(qi::blank)[
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
                  ruleTextureFlagBoost)];
    }

    void setupReflectionMapRules()
    {
        ruleReflectionMapSphere =
            qi::lit("refl -type sphere") >>
            ruleTextureMapBody;

        ruleReflectionMapSphere =
            qi::lit("refl -type cube_top") >>
            ruleTextureMapBody;

        ruleReflectionMapSphere =
            qi::lit("refl -type cube_bottom") >>
            ruleTextureMapBody;

        ruleReflectionMapSphere =
            qi::lit("refl -type cube_front") >>
            ruleTextureMapBody;

        ruleReflectionMapSphere =
            qi::lit("refl -type cube_back") >>
            ruleTextureMapBody;

        ruleReflectionMapSphere =
            qi::lit("refl -type cube_left") >>
            ruleTextureMapBody;

        ruleReflectionMapSphere =
            qi::lit("refl -type cube_right") >>
            ruleTextureMapBody;

        ruleReflectionMap =
            qi::skip(qi::blank)[
                (ruleReflectionMapSphere |
                 ruleReflectionMapCubeTop |
                 ruleReflectionMapCubeBottom |
                 ruleReflectionMapCubeFront |
                 ruleReflectionMapCubeBack |
                 ruleReflectionMapCubeLeft |
                 ruleReflectionMapCubeRight)];
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
    qi::rule<Iterator, OBJVector3(), Skipper> ruleVector3Data;
    qi::rule<Iterator, bool(), Skipper> ruleBoolOnOffTrue;           ///< Parses flag value "on" and "off". If none specified, returns TRUE by default
    qi::rule<Iterator, bool(), Skipper> ruleBoolOnOffFalse;          ///< Parses flag value "on" and "off". If none specified, returns FALSE by default

    //--------------------------------------------------------------------
    // Color / Illumination Rules

    qi::rule<Iterator, Skipper> ruleColor;
    qi::rule<Iterator, Skipper> ruleIllumination;

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

    qi::rule<Iterator, float(), Skipper> ruleDissolveFactor;
    qi::rule<Iterator, bool(), Skipper> ruleDissolveHalo;
    qi::rule<Iterator, OBJMaterialDissolve(), Skipper> ruleDissolve;

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
private:

};

//------------------------------------------------------------------------------------------

#endif