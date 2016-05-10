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

#include "MTLGrammar.hpp"
#include "OBJState.hpp"

//------------------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------------------

MTLGrammar::MTLGrammar(OBJState* state)
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

//------------------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------------------

void MTLGrammar::resetCurrentTexture()
{
    m_CurrentTexture = OBJTextureDescriptor();
}

void MTLGrammar::finishCurrentMaterial()
{
    if(m_pOBJState)
    {
        m_pOBJState->setMaterial(m_CurrentMaterial.getName(), m_CurrentMaterial);
    }

    m_CurrentMaterial = OBJMaterial();
}

//------------------------------------------------------------------------------------------
// Protected Methods
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------------------

void MTLGrammar::setupGeneralRules()
{
    ruleString = +(qi::char_ - qi::space);
    ruleStringNoSkip = qi::lexeme[+(qi::graph)];

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

void MTLGrammar::setupNewMaterialRules()
{
    ruleNewMaterial =
        qi::lit("newmtl ") [boost::phoenix::bind(&MTLGrammar::finishCurrentMaterial, this)] >>
        ruleString [boost::phoenix::bind(&OBJMaterial::setName, &m_CurrentMaterial, qi::_1)] >>
        qi::eol;
}

void MTLGrammar::setupColorIlluminationRules()
{
    ruleRFL =
        ruleStringNoSkip >>
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
    // Emissive Color
    //----------------------------------------------------------------

    ruleEmissiveRGB = 
        ruleVector3Data [boost::phoenix::bind(&OBJMaterial::setEmissiveReflectivityRGB, &m_CurrentMaterial, qi::_1)];

    ruleEmissiveSpectral =
        qi::lit("spectral") >>
        ruleRFL [boost::phoenix::bind(&OBJMaterial::setEmissiveReflectivityRFL, &m_CurrentMaterial, qi::_1)];

    ruleEmissiveXYZ =
        qi::lit("xyz") >>
        ruleVector3Data [boost::phoenix::bind(&OBJMaterial::setEmissiveReflectivityXYZ, &m_CurrentMaterial, qi::_1)];

    ruleEmissive = 
        qi::lit("Ke") >> 
        (ruleEmissiveRGB | 
            ruleEmissiveSpectral | 
            ruleEmissiveXYZ);

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
        qi::float_ [boost::phoenix::bind(&OBJMaterial::setSpecularExponent, &m_CurrentMaterial, qi::_1)];

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
            ruleEmissive |
            ruleTransmission | 
            ruleIllumination |
            ruleDissolve |
            ruleSpecularExponent |
            ruleSharpness |
            ruleOpticalDensity) >>
            qi::eol;
}

void MTLGrammar::setupTextureMapRules()
{
    setupTextureMapFlagRules();

    //----------------------------------------------------------------
    // Texture Map Types
    //----------------------------------------------------------------

    ruleTextureMapBody =
        ruleTextureFlags >>
        ruleStringNoSkip [boost::phoenix::bind(&OBJTextureDescriptor::setPath, &m_CurrentTexture, qi::_1)] >>
        qi::eol;

    ruleTextureMapAmbient =
        qi::lit("map_Ka")  >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setAmbientTexture, &m_CurrentMaterial, &m_CurrentTexture)];

    ruleTextureMapDiffuse =
        qi::lit("map_Kd") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setDiffuseTexture, &m_CurrentMaterial, &m_CurrentTexture)];

    ruleTextureMapSpecular =
        qi::lit("map_Ks") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setSpecularTexture, &m_CurrentMaterial, &m_CurrentTexture)];

    ruleTextureMapSpecularExponent =
        qi::lit("map_Ns") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setSpecularExponentTexture, &m_CurrentMaterial, &m_CurrentTexture)];

    ruleTextureMapEmissive =
        qi::lit("map_Ke") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setEmissiveTexture, &m_CurrentMaterial, &m_CurrentTexture)];

    ruleTextureMapDissolve =
        qi::lit("map_d") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setDissolveTexture, &m_CurrentMaterial, &m_CurrentTexture)];

    ruleTextureMapDecal =
        qi::lit("decal") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setDecalTexture, &m_CurrentMaterial, &m_CurrentTexture)];

    ruleTextureMapDisplacement =
        qi::lit("disp") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setDisplacementTexture, &m_CurrentMaterial, &m_CurrentTexture)];

    ruleTextureMapBump =
        qi::lit("bump") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setBumpTexture, &m_CurrentMaterial, &m_CurrentTexture)];

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
            ruleTextureMapEmissive |
            ruleTextureMapDissolve |
            ruleTextureMapDecal |
            ruleTextureMapDisplacement |
            ruleTextureMapBump |
            ruleTextureAntiAliasing);
}

void MTLGrammar::setupTextureMapFlagRules()
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

    ruleTextureFlagimfchan =
        qi::omit[qi::lit("-imfchan")] >>
        (qi::graph)[boost::phoenix::bind(&OBJTextureDescriptor::setimfchan, &m_CurrentTexture, qi::_1)];


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
            ruleTextureFlagBoost |
            ruleTextureFlagimfchan);
}

void MTLGrammar::setupReflectionMapRules()
{
    ruleReflectionMapSphere =
        qi::lit("refl -type sphere") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapSphere, &m_CurrentMaterial, &m_CurrentTexture)];

    ruleReflectionMapCubeTop =
        qi::lit("refl -type cube_top") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapCubeSide, &m_CurrentMaterial, OBJReflectionMapCubeSide::Top, &m_CurrentTexture)];

    ruleReflectionMapCubeBottom =
        qi::lit("refl -type cube_bottom") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapCubeSide, &m_CurrentMaterial, OBJReflectionMapCubeSide::Bottom, &m_CurrentTexture)];

    ruleReflectionMapCubeFront =
        qi::lit("refl -type cube_front") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapCubeSide, &m_CurrentMaterial, OBJReflectionMapCubeSide::Front, &m_CurrentTexture)];

    ruleReflectionMapCubeBack =
        qi::lit("refl -type cube_back") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapCubeSide, &m_CurrentMaterial, OBJReflectionMapCubeSide::Back, &m_CurrentTexture)];

    ruleReflectionMapCubeLeft =
        qi::lit("refl -type cube_left") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapCubeSide, &m_CurrentMaterial, OBJReflectionMapCubeSide::Left, &m_CurrentTexture)];

    ruleReflectionMapCubeRight =
        qi::lit("refl -type cube_right") [boost::phoenix::bind(&MTLGrammar::resetCurrentTexture, this)] >>
        ruleTextureMapBody [boost::phoenix::bind(&OBJMaterial::setReflectionMapCubeSide, &m_CurrentMaterial, OBJReflectionMapCubeSide::Right, &m_CurrentTexture)];

    ruleReflectionMap =
        (ruleReflectionMapSphere |
            ruleReflectionMapCubeTop |
            ruleReflectionMapCubeBottom |
            ruleReflectionMapCubeFront |
            ruleReflectionMapCubeBack |
            ruleReflectionMapCubeLeft |
            ruleReflectionMapCubeRight);
}