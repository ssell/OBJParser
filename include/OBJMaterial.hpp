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

#ifndef __H__OBJ_PARSER_MTL_MATERIAL__H__
#define __H__OBJ_PARSER_MTL_MATERIAL__H__

#include "OBJStructs.hpp"

#include <string>
#include <cstdint>
#include <array>

//------------------------------------------------------------------------------------------

enum class OBJMaterialPropertyType
{
    RGB,     ///< Property is specified using RGB values
    XYZ,     ///< Property is specified using CIEXYZ values
    RFL      ///< Property is specified using a .rfl file
};

struct OBJMaterialPropertyRFL
{
    OBJMaterialPropertyRFL()
        : factor(1.0f)
    {

    }

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
    OBJMaterialPropertyType type;

    union
    {
        float r, x;
        float g, y;
        float b, z;
    };

    OBJMaterialPropertyRFL rfl;
};

//------------------------------------------------------------------------------------------

/**
 * \struct OBJMaterialDissolve
 */
struct OBJMaterialDissolve
{
    OBJMaterialDissolve()
        : halo(false),
          factor(1.0f)
    {

    }

    //--------------------------------------------------------------------
    
    bool halo;                    ///< True if dissolve depends on surface orientation relative to viewer
    float factor;                 ///< The amount the material dissolves. 0.0: fully dissolved, transparent; 1.0: opaque
};

BOOST_FUSION_ADAPT_STRUCT(OBJMaterialDissolve, (bool, halo), (float, factor));

//------------------------------------------------------------------------------------------

enum class OBJTextureChannel
{
    None = 0,
    Red,
    Green,
    Blue,
    Matte,
    Luminance,
    Depth
};

/**
 * \struct OBJTextureDescriptor
 *
 * Generic descriptor for all mapped material textures.
 *
 * Note that not all fields are available for all texture types,
 * or that only a few may actually be in use.
 */
struct OBJTextureDescriptor
{
    OBJTextureDescriptor();
    ~OBJTextureDescriptor();

    OBJTextureDescriptor& operator=(OBJTextureDescriptor const& rhs);

    //--------------------------------------------------------------------

    bool blendU;                  ///< If true, enable texture blending along the horizontal axis. Default is true.
    bool blendV;                  ///< If true, enable texture blending along the vertical axis. Default is true.
    bool clamp;                   ///< If true, enable texture clamping. Default is false.
    bool colorCorrection;         ///< If true, enable color correction. Only used on ambient, diffuse, and specular textures.

    uint32_t resolution;          ///< Texture resolution. Default of 0 if no resolution specified.

    float bumpMultiplier;         ///< Multiplier of values in bump textures only. Positive or negative. Typical range (0.0 - 1.0)
    float boost;                  ///< Increases sharpness of mip-mapped textures. Positive only. Typical range (1.0 - 5.0)

    float rangeModBase;           ///< Modifies texture values by adding a base value. Default is 0.0
    float rangeModGain;           ///< Modifies texture values by increasing the contrast. Default is 1.0

    OBJVector3 offset;            ///< Offsets the position of the texture on the surface via shifting. Default (0.0, 0.0, 0.0). (horiz, vert, depth).
    OBJVector3 scale;             ///< Scales the values of the texture. Default (1.0, 1.0, 1.0). (horiz, vert, depth).
    OBJVector3 turbulence;        ///< Applies turbulence to the texture. Default (0.0, 0.0, 0.0) for no turbulence.  (horiz, vert, depth).

    OBJTextureChannel imfchan;    ///< Specifies the channel used to create a scalar or bump texture.

    std::string path;             ///< Relative path (from material file) to texture source file. Includes file extension.
};

//------------------------------------------------------------------------------------------

enum class OBJReflectionMapType
{
    None = 0,
    Sphere,
    Cube
};

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

    // Transmission Filter

    void setTransmissionFilterRGB(OBJVector3 const& rgb);
    void setTransmissionFilterXYZ(OBJVector3 const& xyz);
    void setTransmissionFilterRFL(OBJMaterialPropertyRFL const& rfl);

    OBJMaterialProperty const& getTransmissionFilter() const;

    // Dissolve

    void setDissolve(OBJMaterialDissolve const& dissolve);
    OBJMaterialDissolve const& getDissolve() const;

    // Illumination Model

    void setIlluminationmodel(uint32_t model);
    uint32_t getIlluminationModel() const;

    // Specular Exponent

    void setSpecularExponent(uint32_t exponent);
    uint32_t getSpecularExponent() const;

    // Sharpness

    void setSharpness(uint32_t sharpness);
    uint32_t getSharpness() const;

    // Optical Density

    void setOpticalDensity(float density);
    float getOpticalDensity() const;

    // Ambient Texture

    void setAmbientTexture(OBJTextureDescriptor const& descriptor);
    OBJTextureDescriptor const& getAmbientTexture() const;

    // Diffuse Texture

    void setDiffuseTexture(OBJTextureDescriptor const& descriptor);
    OBJTextureDescriptor const& getDiffuseTexture() const;

    // Specular Texture

    void setSpecularTexture(OBJTextureDescriptor const& descriptor);
    OBJTextureDescriptor const& getSpecularTexture() const;

    // SpecularExponent Texture

    void setSpecularExponentTexture(OBJTextureDescriptor const& descriptor);
    OBJTextureDescriptor const& getSpecularExponentTexture() const;

    // Dissolve Texture

    void setDissolveTexture(OBJTextureDescriptor const& descriptor);
    OBJTextureDescriptor const& getDissolveTexture() const;

    // Decal Texture

    void setDecalTexture(OBJTextureDescriptor const& descriptor);
    OBJTextureDescriptor const& getDecalTexture() const;

    // Displacement Texture

    void setDisplacementTexture(OBJTextureDescriptor const& descriptor);
    OBJTextureDescriptor const& getDisplacementTexture() const;

    // Bump Texture

    void setBumpTexture(OBJTextureDescriptor const& descriptor);
    OBJTextureDescriptor const& getBumpTexture() const;

    // Anti-Aliasing

    void setAntiAliasing(bool aa);
    bool getAntiAliasing() const;

    // Reflection Map

    OBJReflectionMapType getReflectionMapType() const;

    void setReflectionMapSphere(OBJTextureDescriptor const& descriptor);
    OBJTextureDescriptor const& getReflectionMapSphere() const;

    void setReflectionMapCubeSide(OBJReflectionMapCubeSide side, OBJTextureDescriptor const& descriptor);
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
    OBJMaterialProperty m_TransmissionFilter;

    OBJMaterialDissolve m_Dissolve;

    uint32_t m_IlluminationModel;   ///< Value between 0 and 10 corresponding to an illumination model. See MTL specification for model details.
    uint32_t m_SpecularExponent;    ///< Focus of the specular highlight. Higher values result in tighter highlights. Typical range (0 - 1000)
    uint32_t m_Sharpness;           ///< Sharpness of reflections. Higher values result in sharper reflections. Typical range of (0 - 1000); Default of 60

    float m_OpticalDensity;         ///< AKA index of refraction. Range (0.001 - 10.0). 1.0 indicates light does not bend as it passes through. Glass is 1.5

    //--------------------------------------------------------------------
    // Texture Map

    OBJTextureDescriptor m_TextureAmbient;
    OBJTextureDescriptor m_TextureDiffuse;
    OBJTextureDescriptor m_TextureSpecular;
    OBJTextureDescriptor m_TextureSpecularExponent;
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