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

#include "OBJMaterial.hpp"

//------------------------------------------------------------------------------------------
// OBJMaterialPropertyRFL
//------------------------------------------------------------------------------------------

OBJMaterialPropertyRFL::OBJMaterialPropertyRFL()
    : factor(1.0f)
{

}

//------------------------------------------------------------------------------------------
// OBJMaterialProperty
//------------------------------------------------------------------------------------------

OBJMaterialProperty::OBJMaterialProperty()
    : type(OBJMaterialPropertyType::None),
      r(0.0f),
      g(0.0f),
      b(0.0f)
{

}

OBJMaterialProperty const& OBJMaterialProperty::operator=(OBJMaterialProperty const& rhs)
{
    type = rhs.type;
    r = rhs.r;
    g = rhs.g;
    b = rhs.b;
    rfl = rhs.rfl;

    return (*this);
}

//------------------------------------------------------------------------------------------
// OBJMaterialDissolve
//------------------------------------------------------------------------------------------

OBJMaterialDissolve::OBJMaterialDissolve()
    : halo(false),
      factor(1.0f)
{

}

//------------------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------------------

OBJMaterial::OBJMaterial()
    : m_Name(""),
      m_IlluminationModel(1),
      m_Transparency(0.0f),
      m_SpecularExponent(1.0f),
      m_Sharpness(60),
      m_OpticalDensity(1.0f),
      m_TextureAntiAliasing(false),
      m_ReflectionMapType(OBJReflectionMapType::None)
{

}

OBJMaterial::~OBJMaterial()
{

}

//------------------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------------------

void OBJMaterial::setName(std::string const& name)
{
    m_Name = name;
}

std::string const& OBJMaterial::getName() const
{
    return m_Name;
}

//------------------------------------------------------------------------
// Ambient Reflectivity
//------------------------------------------------------------------------

void OBJMaterial::setAmbientReflectivityRGB(OBJVector3 const& rgb)
{
    m_AmbientReflectivity.type = OBJMaterialPropertyType::RGB;
    m_AmbientReflectivity.r = rgb.x;
    m_AmbientReflectivity.g = rgb.y;
    m_AmbientReflectivity.b = rgb.z;
}

void OBJMaterial::setAmbientReflectivityXYZ(OBJVector3 const& xyz)
{    
    m_AmbientReflectivity.type = OBJMaterialPropertyType::XYZ;
    m_AmbientReflectivity.x = xyz.x;
    m_AmbientReflectivity.y = xyz.y;
    m_AmbientReflectivity.z = xyz.z;
}

void OBJMaterial::setAmbientReflectivityRFL(OBJMaterialPropertyRFL const& rfl)
{
    m_AmbientReflectivity.type = OBJMaterialPropertyType::RFL;
    m_AmbientReflectivity.rfl.path = rfl.path;
    m_AmbientReflectivity.rfl.factor = rfl.factor;
}

OBJMaterialProperty const& OBJMaterial::getAmbientReflectivity() const
{
    return m_AmbientReflectivity;
}

//------------------------------------------------------------------------
// Diffuse Reflectivity
//------------------------------------------------------------------------

void OBJMaterial::setDiffuseReflectivityRGB(OBJVector3 const& rgb)
{
    m_DiffuseReflectivity.type = OBJMaterialPropertyType::RGB;
    m_DiffuseReflectivity.r = rgb.x;
    m_DiffuseReflectivity.g = rgb.y;
    m_DiffuseReflectivity.b = rgb.z;
}

void OBJMaterial::setDiffuseReflectivityXYZ(OBJVector3 const& xyz)
{    
    m_DiffuseReflectivity.type = OBJMaterialPropertyType::XYZ;
    m_DiffuseReflectivity.x = xyz.x;
    m_DiffuseReflectivity.y = xyz.y;
    m_DiffuseReflectivity.z = xyz.z;
}

void OBJMaterial::setDiffuseReflectivityRFL(OBJMaterialPropertyRFL const& rfl)
{
    m_DiffuseReflectivity.type = OBJMaterialPropertyType::RFL;
    m_DiffuseReflectivity.rfl.path = rfl.path;
    m_DiffuseReflectivity.rfl.factor = rfl.factor;
}

OBJMaterialProperty const& OBJMaterial::getDiffuseReflectivity() const
{
    return m_DiffuseReflectivity;
}

//------------------------------------------------------------------------
// Specular Reflectivity
//------------------------------------------------------------------------

void OBJMaterial::setSpecularReflectivityRGB(OBJVector3 const& rgb)
{
    m_SpecularReflectivity.type = OBJMaterialPropertyType::RGB;
    m_SpecularReflectivity.r = rgb.x;
    m_SpecularReflectivity.g = rgb.y;
    m_SpecularReflectivity.b = rgb.z;
}

void OBJMaterial::setSpecularReflectivityXYZ(OBJVector3 const& xyz)
{    
    m_SpecularReflectivity.type = OBJMaterialPropertyType::XYZ;
    m_SpecularReflectivity.x = xyz.x;
    m_SpecularReflectivity.y = xyz.y;
    m_SpecularReflectivity.z = xyz.z;
}

void OBJMaterial::setSpecularReflectivityRFL(OBJMaterialPropertyRFL const& rfl)
{
    m_SpecularReflectivity.type = OBJMaterialPropertyType::RFL;
    m_SpecularReflectivity.rfl.path = rfl.path;
    m_SpecularReflectivity.rfl.factor = rfl.factor;
}

OBJMaterialProperty const& OBJMaterial::getSpecularReflectivity() const
{
    return m_SpecularReflectivity;
}

//------------------------------------------------------------------------
// Emissive Reflectivity
//------------------------------------------------------------------------

void OBJMaterial::setEmissiveReflectivityRGB(OBJVector3 const& rgb)
{
    m_EmissiveReflectivity.type = OBJMaterialPropertyType::RGB;
    m_EmissiveReflectivity.r = rgb.x;
    m_EmissiveReflectivity.g = rgb.y;
    m_EmissiveReflectivity.b = rgb.z;
}

void OBJMaterial::setEmissiveReflectivityXYZ(OBJVector3 const& xyz)
{    
    m_EmissiveReflectivity.type = OBJMaterialPropertyType::XYZ;
    m_EmissiveReflectivity.x = xyz.x;
    m_EmissiveReflectivity.y = xyz.y;
    m_EmissiveReflectivity.z = xyz.z;
}

void OBJMaterial::setEmissiveReflectivityRFL(OBJMaterialPropertyRFL const& rfl)
{
    m_EmissiveReflectivity.type = OBJMaterialPropertyType::RFL;
    m_EmissiveReflectivity.rfl.path = rfl.path;
    m_EmissiveReflectivity.rfl.factor = rfl.factor;
}

OBJMaterialProperty const& OBJMaterial::getEmissiveReflectivity() const
{
    return m_EmissiveReflectivity;
}

//------------------------------------------------------------------------
// Transmission Filter
//------------------------------------------------------------------------

void OBJMaterial::setTransmissionFilterRGB(OBJVector3 const& rgb)
{
    m_TransmissionFilter.type = OBJMaterialPropertyType::RGB;
    m_TransmissionFilter.r = rgb.x;
    m_TransmissionFilter.g = rgb.y;
    m_TransmissionFilter.b = rgb.z;
}

void OBJMaterial::setTransmissionFilterXYZ(OBJVector3 const& xyz)
{    
    m_TransmissionFilter.type = OBJMaterialPropertyType::XYZ;
    m_TransmissionFilter.x = xyz.x;
    m_TransmissionFilter.y = xyz.y;
    m_TransmissionFilter.z = xyz.z;
}

void OBJMaterial::setTransmissionFilterRFL(OBJMaterialPropertyRFL const& rfl)
{
    m_TransmissionFilter.type = OBJMaterialPropertyType::RFL;
    m_TransmissionFilter.rfl.path = rfl.path;
    m_TransmissionFilter.rfl.factor = rfl.factor;
}

OBJMaterialProperty const& OBJMaterial::getTransmissionFilter() const
{
    return m_TransmissionFilter;
}

//------------------------------------------------------------------------
// Dissolve
//------------------------------------------------------------------------

void OBJMaterial::setDissolve(OBJMaterialDissolve const& dissolve)
{
    m_Dissolve.factor = dissolve.factor;
    m_Dissolve.halo = dissolve.halo;
}

OBJMaterialDissolve const& OBJMaterial::getDissolve() const
{
    return m_Dissolve;
}

//------------------------------------------------------------------------
// Transparency
//------------------------------------------------------------------------

void OBJMaterial::setTransparency(float transparency)
{
    m_Transparency = transparency;
}

float OBJMaterial::getTransparency() const
{
    return m_Transparency;
}

//------------------------------------------------------------------------
// Illumination Model
//------------------------------------------------------------------------

void OBJMaterial::setIlluminationModel(uint32_t const model)
{
    m_IlluminationModel = model;
}

uint32_t OBJMaterial::getIlluminationModel() const
{
    return m_IlluminationModel;
}

//------------------------------------------------------------------------
// Sharpness
//------------------------------------------------------------------------

void OBJMaterial::setSharpness(uint32_t const sharpness)
{
    m_Sharpness = sharpness;
}

uint32_t OBJMaterial::getSharpness() const
{
    return m_Sharpness;
}

//------------------------------------------------------------------------
// Specular Exponent
//------------------------------------------------------------------------

void OBJMaterial::setSpecularExponent(float const exponent)
{
    m_SpecularExponent = exponent;
}

float OBJMaterial::getSpecularExponent() const
{
    return m_SpecularExponent;
}

//------------------------------------------------------------------------
// Optical Density
//------------------------------------------------------------------------

void OBJMaterial::setOpticalDensity(float const density)
{
    m_OpticalDensity = density;
}

float OBJMaterial::getOpticalDensity() const
{
    return m_OpticalDensity;
}

//------------------------------------------------------------------------
// Ambient Texture
//------------------------------------------------------------------------

void OBJMaterial::setAmbientTexture(OBJTextureDescriptor const* descriptor)
{
    m_TextureAmbient = *descriptor;
}

OBJTextureDescriptor const& OBJMaterial::getAmbientTexture() const
{
    return m_TextureAmbient;
}

//------------------------------------------------------------------------
// Diffuse Texture
//------------------------------------------------------------------------

void OBJMaterial::setDiffuseTexture(OBJTextureDescriptor const* descriptor)
{
    m_TextureDiffuse = *descriptor;
}

OBJTextureDescriptor const& OBJMaterial::getDiffuseTexture() const
{
    return m_TextureDiffuse;
}

//------------------------------------------------------------------------
// Specular Texture
//------------------------------------------------------------------------

void OBJMaterial::setSpecularTexture(OBJTextureDescriptor const* descriptor)
{
    m_TextureSpecular = *descriptor;
}

OBJTextureDescriptor const& OBJMaterial::getSpecularTexture() const
{
    return m_TextureSpecular;
}

//------------------------------------------------------------------------
// SpecularExponent Texture
//------------------------------------------------------------------------

void OBJMaterial::setSpecularExponentTexture(OBJTextureDescriptor const* descriptor)
{
    m_TextureSpecularExponent = *descriptor;
}

OBJTextureDescriptor const& OBJMaterial::getSpecularExponentTexture() const
{
    return m_TextureSpecularExponent;
}

//------------------------------------------------------------------------
// Emissive Texture
//------------------------------------------------------------------------

void OBJMaterial::setEmissiveTexture(OBJTextureDescriptor const* descriptor)
{
    m_TextureEmissive = *descriptor;
}

OBJTextureDescriptor const& OBJMaterial::getEmissiveTexture() const
{
    return m_TextureEmissive;
}

//------------------------------------------------------------------------
// Dissolve Texture
//------------------------------------------------------------------------

void OBJMaterial::setDissolveTexture(OBJTextureDescriptor const* descriptor)
{
    m_TextureDissolve = *descriptor;
}

OBJTextureDescriptor const& OBJMaterial::getDissolveTexture() const
{
    return m_TextureDissolve;
}

//------------------------------------------------------------------------
// Decal Texture
//------------------------------------------------------------------------

void OBJMaterial::setDecalTexture(OBJTextureDescriptor const* descriptor)
{
    m_TextureDecal = *descriptor;
}

OBJTextureDescriptor const& OBJMaterial::getDecalTexture() const
{
    return m_TextureDecal;
}

//------------------------------------------------------------------------
// Displacement Texture
//------------------------------------------------------------------------

void OBJMaterial::setDisplacementTexture(OBJTextureDescriptor const* descriptor)
{
    m_TextureDisplacement = *descriptor;
}

OBJTextureDescriptor const& OBJMaterial::getDisplacementTexture() const
{
    return m_TextureDisplacement;
}

//------------------------------------------------------------------------
// Bump Texture
//------------------------------------------------------------------------

void OBJMaterial::setBumpTexture(OBJTextureDescriptor const* descriptor)
{
    m_TextureBump = *descriptor;
}

OBJTextureDescriptor const& OBJMaterial::getBumpTexture() const
{
    return m_TextureBump;
}

//------------------------------------------------------------------------
// Anti-Aliasing
//------------------------------------------------------------------------

void OBJMaterial::setAntiAliasing(bool aa)
{
    m_TextureAntiAliasing = aa;
}

bool OBJMaterial::getAntiAliasing() const
{
    return m_TextureAntiAliasing;
}

//------------------------------------------------------------------------
// Reflection Map
//------------------------------------------------------------------------

OBJReflectionMapType OBJMaterial::getReflectionMapType() const
{
    return m_ReflectionMapType;
}

void OBJMaterial::setReflectionMapSphere(OBJTextureDescriptor const* descriptor)
{
    m_ReflectionMapType = OBJReflectionMapType::Sphere;
    m_ReflectionMapSphere = *descriptor;
}

OBJTextureDescriptor const& OBJMaterial::getReflectionMapSphere() const
{
    return m_ReflectionMapSphere;
}

void OBJMaterial::setReflectionMapCubeSide(OBJReflectionMapCubeSide side, OBJTextureDescriptor const* descriptor)
{
    m_ReflectionMapType = OBJReflectionMapType::Cube;
    m_ReflectionMapCube[static_cast<uint32_t>(side)] = *descriptor;
}

OBJTextureDescriptor const& OBJMaterial::getReflectionMapCubeSide(OBJReflectionMapCubeSide side) const
{
    return m_ReflectionMapCube[static_cast<uint32_t>(side)];
}

//------------------------------------------------------------------------------------------
// Protected Methods
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------------------