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

#ifndef __H__OBJ_PARSER_MTL_MATERIAL__H__
#define __H__OBJ_PARSER_MTL_MATERIAL__H__

#include "OBJTextureDescriptor.hpp"

#include <string>
#include <cstdint>
#include <array>

//------------------------------------------------------------------------------------------

/**
 * \enum OBJMaterialPropertyType
 */
enum class OBJMaterialPropertyType
{
    None = 0,
    RGB,     ///< Property is specified using RGB values
    XYZ,     ///< Property is specified using CIEXYZ values
    RFL      ///< Property is specified using a .rfl file
};

/**
 * \struct OBJMaterialPropertyRFL
 */
struct OBJMaterialPropertyRFL
{
    OBJMaterialPropertyRFL();

    std::string path;
    float factor;
};

BOOST_FUSION_ADAPT_STRUCT(OBJMaterialPropertyRFL, (std::string, path), (float, factor));

/**
 * \struct OBJMaterialProperty
 * 
 * Represents a modular material property that can be represented in one 
 * of three different ways. The type of representation in use is specified
 * by the OBJMaterialPropertyType member.
 *
 * An individual property will only ever represent a single type at a time.
 */
struct OBJMaterialProperty
{
    OBJMaterialProperty();
    OBJMaterialProperty const& operator=(OBJMaterialProperty const& rhs);

    //--------------------------------------------------------------------

    OBJMaterialPropertyType type;

    union { float r, x; };
    union { float g, y; };
    union { float b, z; };

    OBJMaterialPropertyRFL rfl;
};

//------------------------------------------------------------------------------------------

/**
 * \struct OBJMaterialDissolve
 */
struct OBJMaterialDissolve
{
    OBJMaterialDissolve();

    bool halo;                    ///< True if dissolve depends on surface orientation relative to viewer
    float factor;                 ///< The amount the material dissolves. 0.0: fully dissolved, transparent; 1.0: opaque
};

BOOST_FUSION_ADAPT_STRUCT(OBJMaterialDissolve, (bool, halo), (float, factor));

//------------------------------------------------------------------------------------------

/**
 * \enum OBJReflectionMapType
 */
enum class OBJReflectionMapType
{
    None = 0,
    Sphere,
    Cube
};

/**
 * \enum OBJReflectionMapCubeSide
 */
enum class OBJReflectionMapCubeSide
{
    Front = 0,
    Back,
    Top,
    Bottom,
    Left,
    Right
};

//------------------------------------------------------------------------------------------

/**
 * \class OBJMaterial
 */
class OBJMaterial
{
public:

    OBJMaterial();
    ~OBJMaterial();

    //--------------------------------------------------------------------

    // Name

    void setName(std::string const& name);
    std::string const& getName() const;

    // Ambient Reflectivity

    void setAmbientReflectivityRGB(OBJVector3 const& rgb);
    void setAmbientReflectivityXYZ(OBJVector3 const& xyz);
    void setAmbientReflectivityRFL(OBJMaterialPropertyRFL const& rfl);

    OBJMaterialProperty const& getAmbientReflectivity() const;

    // Diffuse Reflectivity

    void setDiffuseReflectivityRGB(OBJVector3 const& rgb);
    void setDiffuseReflectivityXYZ(OBJVector3 const& xyz);
    void setDiffuseReflectivityRFL(OBJMaterialPropertyRFL const& rfl);

    OBJMaterialProperty const& getDiffuseReflectivity() const;

    // Specular Reflectivity

    void setSpecularReflectivityRGB(OBJVector3 const& rgb);
    void setSpecularReflectivityXYZ(OBJVector3 const& xyz);
    void setSpecularReflectivityRFL(OBJMaterialPropertyRFL const& rfl);

    OBJMaterialProperty const& getSpecularReflectivity() const;

    // Emissive Reflectivity

    void setEmissiveReflectivityRGB(OBJVector3 const& rgb);
    void setEmissiveReflectivityXYZ(OBJVector3 const& xyz);
    void setEmissiveReflectivityRFL(OBJMaterialPropertyRFL const& rfl);

    OBJMaterialProperty const& getEmissiveReflectivity() const;

    // Transmission Filter

    void setTransmissionFilterRGB(OBJVector3 const& rgb);
    void setTransmissionFilterXYZ(OBJVector3 const& xyz);
    void setTransmissionFilterRFL(OBJMaterialPropertyRFL const& rfl);

    OBJMaterialProperty const& getTransmissionFilter() const;

    // Dissolve

    void setDissolve(OBJMaterialDissolve const& dissolve);
    OBJMaterialDissolve const& getDissolve() const;

    // Transparency (inverse of dissolve)

    void setTransparency(float transparency);
    float getTransparency() const;

    // Illumination Model

    void setIlluminationModel(uint32_t model);
    uint32_t getIlluminationModel() const;

    // Sharpness

    void setSharpness(uint32_t sharpness);
    uint32_t getSharpness() const;

    // Specular Exponent

    void setSpecularExponent(float exponent);
    float getSpecularExponent() const;

    // Optical Density

    void setOpticalDensity(float density);
    float getOpticalDensity() const;

    // Ambient Texture

    void setAmbientTexture(OBJTextureDescriptor const* descriptor);
    OBJTextureDescriptor const& getAmbientTexture() const;

    // Diffuse Texture

    void setDiffuseTexture(OBJTextureDescriptor const* descriptor);
    OBJTextureDescriptor const& getDiffuseTexture() const;

    // Specular Texture

    void setSpecularTexture(OBJTextureDescriptor const* descriptor);
    OBJTextureDescriptor const& getSpecularTexture() const;

    // SpecularExponent Texture

    void setSpecularExponentTexture(OBJTextureDescriptor const* descriptor);
    OBJTextureDescriptor const& getSpecularExponentTexture() const;

    // Emissive Texture

    void setEmissiveTexture(OBJTextureDescriptor const* descriptor);
    OBJTextureDescriptor const& getEmissiveTexture() const;

    // Dissolve Texture

    void setDissolveTexture(OBJTextureDescriptor const* descriptor);
    OBJTextureDescriptor const& getDissolveTexture() const;

    // Decal Texture

    void setDecalTexture(OBJTextureDescriptor const* descriptor);
    OBJTextureDescriptor const& getDecalTexture() const;

    // Displacement Texture

    void setDisplacementTexture(OBJTextureDescriptor const* descriptor);
    OBJTextureDescriptor const& getDisplacementTexture() const;

    // Bump Texture

    void setBumpTexture(OBJTextureDescriptor const* descriptor);
    OBJTextureDescriptor const& getBumpTexture() const;

    // Anti-Aliasing

    void setAntiAliasing(bool aa);
    bool getAntiAliasing() const;

    // Reflection Map

    OBJReflectionMapType getReflectionMapType() const;

    void setReflectionMapSphere(OBJTextureDescriptor const* descriptor);
    OBJTextureDescriptor const& getReflectionMapSphere() const;

    void setReflectionMapCubeSide(OBJReflectionMapCubeSide side, OBJTextureDescriptor const* descriptor);
    OBJTextureDescriptor const& getReflectionMapCubeSide(OBJReflectionMapCubeSide side) const;

protected:

    //--------------------------------------------------------------------
    // Member Variables
    //--------------------------------------------------------------------

    std::string m_Name;

    //--------------------------------------------------------------------
    // Color and Illumination

    OBJMaterialProperty m_AmbientReflectivity;
    OBJMaterialProperty m_DiffuseReflectivity;
    OBJMaterialProperty m_SpecularReflectivity;
    OBJMaterialProperty m_EmissiveReflectivity;
    OBJMaterialProperty m_TransmissionFilter;

    OBJMaterialDissolve m_Dissolve;

    uint32_t m_IlluminationModel;   ///< Value between 0 and 10 corresponding to an illumination model. See MTL specification for model details.
    uint32_t m_Sharpness;           ///< Sharpness of reflections. Higher values result in sharper reflections. Typical range of (0 - 1000); Default of 60
    
    float m_Transparency;           ///< Optional value set by certain writers. Typically is the inverse of the 'dissolve' value.
    float m_SpecularExponent;       ///< Focus of the specular highlight. Higher values result in tighter highlights. Typical range (0.0 - 1000.0)
    float m_OpticalDensity;         ///< AKA index of refraction. Range (0.001 - 10.0). 1.0 indicates light does not bend as it passes through. Glass is 1.5

    //--------------------------------------------------------------------
    // Texture Map

    OBJTextureDescriptor m_TextureAmbient;
    OBJTextureDescriptor m_TextureDiffuse;
    OBJTextureDescriptor m_TextureSpecular;
    OBJTextureDescriptor m_TextureSpecularExponent;
    OBJTextureDescriptor m_TextureEmissive;
    OBJTextureDescriptor m_TextureDissolve;
    OBJTextureDescriptor m_TextureDecal;
    OBJTextureDescriptor m_TextureDisplacement;
    OBJTextureDescriptor m_TextureBump;

    bool m_TextureAntiAliasing;

    //--------------------------------------------------------------------
    // Reflection Map

    OBJReflectionMapType m_ReflectionMapType;

    OBJTextureDescriptor m_ReflectionMapSphere;
    std::array<OBJTextureDescriptor, 8> m_ReflectionMapCube;

private:
};

//------------------------------------------------------------------------------------------

#endif