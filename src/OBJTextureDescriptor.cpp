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

#include "OBJTextureDescriptor.hpp"

//------------------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------------------

OBJTextureDescriptor::OBJTextureDescriptor()
    : m_BlendU(true),
      m_BlendV(true), 
      m_Clamp(false),
      m_ColorCorrection(false),
      m_Resolution(0),
      m_BumpMultiplier(1.0f),
      m_Boost(1.0f),
      m_RangeModBase(0.0f),
      m_RangeModGain(1.0f),
      m_imfchan(OBJTextureChannel::None)
{

}

OBJTextureDescriptor::~OBJTextureDescriptor()
{

}

OBJTextureDescriptor& OBJTextureDescriptor::operator=(OBJTextureDescriptor const& rhs)
{
    m_BlendU = rhs.getBlendU();
    m_BlendV = rhs.getBlendV();
    m_Clamp = rhs.getClamp();
    m_ColorCorrection = rhs.getColorCorrection();
    m_Resolution = rhs.getResolution();
    m_BumpMultiplier = rhs.getBumpMultiplier();
    m_Boost = rhs.getBoost();
    m_RangeModBase = rhs.getRangeModBase();
    m_RangeModGain = rhs.getRangeModGain();
    m_Offset = rhs.getOffset();
    m_Scale = rhs.getScale();
    m_Turbulence = rhs.getTurbulence();
    m_imfchan = rhs.getimfchan();
    m_Path = rhs.getPath();

    return (*this);
}

//------------------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------------------

void OBJTextureDescriptor::setBlendU(bool const on)
{
    m_BlendU = on;
}

bool OBJTextureDescriptor::getBlendU() const
{
    return m_BlendU;
}

void OBJTextureDescriptor::setBlendV(bool const on)
{
    m_BlendV = on;
}

bool OBJTextureDescriptor::getBlendV() const
{
    return m_BlendV;
}

void OBJTextureDescriptor::setClamp(bool const on)
{
    m_Clamp = on;
}

bool OBJTextureDescriptor::getClamp() const
{
    return m_Clamp;
}

void OBJTextureDescriptor::setColorCorrection(bool const on)
{
    m_ColorCorrection = on;
}

bool OBJTextureDescriptor::getColorCorrection() const
{
    return m_ColorCorrection;
}

void OBJTextureDescriptor::setResolution(uint32_t const resolution)
{
    m_Resolution = resolution;
}

uint32_t OBJTextureDescriptor::getResolution() const
{
    return m_Resolution;
}

void OBJTextureDescriptor::setBumpMultiplier(float const multiplier)
{
    m_BumpMultiplier = multiplier;
}

float OBJTextureDescriptor::getBumpMultiplier() const
{
    return m_BumpMultiplier;
}

void OBJTextureDescriptor::setBoost(float const boost)
{
    m_Boost = boost;
}

float OBJTextureDescriptor::getBoost() const
{
    return m_Boost;
}

void OBJTextureDescriptor::setRangeMod(OBJVector2 const& basegain)
{
    m_RangeModBase = basegain.x;
    m_RangeModGain = basegain.y;
}

float OBJTextureDescriptor::getRangeModBase() const
{
    return m_RangeModBase;
}

float OBJTextureDescriptor::getRangeModGain() const
{
    return m_RangeModGain;
}

void OBJTextureDescriptor::setOffset(OBJVector3 const& offset)
{
    m_Offset = offset;
}

OBJVector3 const& OBJTextureDescriptor::getOffset() const
{
    return m_Offset;
}

void OBJTextureDescriptor::setScale(OBJVector3 const& scale)
{
    m_Scale = scale;
}

OBJVector3 const& OBJTextureDescriptor::getScale() const
{
    return m_Scale;
}

void OBJTextureDescriptor::setTurbulence(OBJVector3 const& turbulence)
{
    m_Turbulence = turbulence;
}

OBJVector3 const& OBJTextureDescriptor::getTurbulence() const
{
    return m_Turbulence;
}

void OBJTextureDescriptor::setimfchan(char const channel)
{
    switch(channel)
    {
    case 'r':
        m_imfchan = OBJTextureChannel::Red;
        break;

    case 'g':
        m_imfchan = OBJTextureChannel::Green;
        break;

    case 'b':
        m_imfchan = OBJTextureChannel::Blue;
        break;

    case 'm':
        m_imfchan = OBJTextureChannel::Matte;
        break;

    case 'l':
        m_imfchan = OBJTextureChannel::Luminance;
        break;

    case 'z':
        m_imfchan = OBJTextureChannel::Depth;
        break;

    default:
        break;
    }
}

OBJTextureChannel OBJTextureDescriptor::getimfchan() const
{
    return m_imfchan;
}

void OBJTextureDescriptor::setPath(std::string const& path)
{
    m_Path = path;
}

std::string const& OBJTextureDescriptor::getPath() const
{
    return m_Path;
}

//------------------------------------------------------------------------------------------
// Protected Methods
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------------------