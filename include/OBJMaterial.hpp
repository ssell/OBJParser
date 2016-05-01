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

//------------------------------------------------------------------------------------------

enum class OBJMaterialPropertyType
{
    RGB,     ///< Property is specified using RGB values
    XYZ,     ///< Property is specified using CIEXYZ values
    RFL      ///< Property is specified using a .rfl file
};

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

    float factor;                 ///< Optional argument specified along with a rfl file. Only used if type is RFL
    std::string rflPath;          ///< .rfl file that the property is specified in. Only used if type is RFL
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
    OBJTextureDescriptor()
        : blendU(true),
          blendV(true), 
          clamp(false),
          colorCorrection(false),
          resolution(0),
          bumpMultiplier(1.0f),
          boost(1.0f),
          rangeModBase(0.0f),
          rangeModGain(1.0f),
          imfchan(OBJTextureChannel::None)
    {

    }

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

enum OBJReflectionMapType
{
    None = 0,
    Sphere,
    Cube
};

struct OBJReflectionMapSphere
{
    OBJTextureDescriptor texture;
};

struct OBJReflectionMapCubeSide
{
    OBJTextureDescriptor texture;
};

struct OBJReflectionMapCube
{
    OBJReflectionMapCubeSide sideTop;
    OBJReflectionMapCubeSide sideBottom;
    OBJReflectionMapCubeSide sideFront;
    OBJReflectionMapCubeSide sideBack;
    OBJReflectionMapCubeSide sideLeft;
    OBJReflectionMapCubeSide sideRight;
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

    //--------------------------------------------------------------------------------------
    // MTL Parser/Grammar Methods
    //--------------------------------------------------------------------------------------

    void setAmbientReflectivityRGB(OBJVector3 const& rgb);
    void setAmbientReflectivityXYZ(OBJVector3 const& xyz);
    void setAmbientReflectivityRFL(std::string const& path, float factor);

    //--------------------------------------------------------------------------------------
    // Member Variables
    //--------------------------------------------------------------------------------------

    std::string name;

    //--------------------------------------------------------------------
    // Color and Illumination

    OBJMaterialProperty ambientReflectivity;
    OBJMaterialProperty diffuseReflectivity;
    OBJMaterialProperty specularReflectivity;
    OBJMaterialProperty transmissionFilter;

    OBJMaterialDissolve dissolve;

    uint32_t illuminationModel;   ///< Value between 0 and 10 corresponding to an illumination model. See MTL specification for model details.
    uint32_t specularExponent;    ///< Focus of the specular highlight. Higher values result in tighter highlights. Typical range (0 - 1000)
    uint32_t sharpness;           ///< Sharpness of reflections. Higher values result in sharper reflections. Typical range of (0 - 1000); Default of 60

    float opticalDensity;         ///< AKA index of refraction. Range (0.001 - 10.0). 1.0 indicates light does not bend as it passes through. Glass is 1.5

    //--------------------------------------------------------------------
    // Texture Map

    OBJTextureDescriptor textureAmbient;
    OBJTextureDescriptor textureDiffuse;
    OBJTextureDescriptor textureSpecular;
    OBJTextureDescriptor textureSpecularExponent;
    OBJTextureDescriptor textureDissolve;
    OBJTextureDescriptor textureDecal;
    OBJTextureDescriptor textureDisplacement;
    OBJTextureDescriptor textureBump;

    bool textureAntiAliasing;

    //--------------------------------------------------------------------
    // Reflection Map

    OBJReflectionMapType reflectionMapType;

    OBJReflectionMapSphere reflectionMapSphere;
    OBJReflectionMapCube reflectionMapCube;

protected:

private:
};

//------------------------------------------------------------------------------------------

#endif