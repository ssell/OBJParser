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

#ifndef __H__OBJ_PARSER_TEXTURE_DESCRIPTOR__H__
#define __H__OBJ_PARSER_TEXTURE_DESCRIPTOR__H__

#include "OBJStructs.hpp"

#include <string>
#include <cstdint>

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
class OBJTextureDescriptor
{
public:

    OBJTextureDescriptor();
    ~OBJTextureDescriptor();

    OBJTextureDescriptor& operator=(OBJTextureDescriptor const& rhs);

    void setBlendU(bool on);
    bool getBlendU() const;

    void setBlendV(bool on);
    bool getBlendV() const;

    void setClamp(bool on);
    bool getClamp() const;

    void setColorCorrection(bool on);
    bool getColorCorrection() const;

    void setResolution(uint32_t resolution);
    uint32_t getResolution() const;

    void setBumpMultiplier(float multiplier);
    float getBumpMultiplier() const;

    void setBoost(float boost);
    float getBoost() const;

    void setRangeMod(OBJVector2 const& basegain);

    float getRangeModBase() const;
    float getRangeModGain() const;

    void setOffset(OBJVector3 const& offset);
    OBJVector3 const& getOffset() const;

    void setScale(OBJVector3 const& scale);
    OBJVector3 const& getScale() const;

    void setTurbulence(OBJVector3 const& turbulence);
    OBJVector3 const& getTurbulence() const;

    void setimfchan(char channel);
    OBJTextureChannel getimfchan() const;

    void setPath(std::string const& path);
    std::string const& getPath() const;

protected:

    bool m_BlendU;                  ///< If true, enable texture blending along the horizontal axis. Default is true.
    bool m_BlendV;                  ///< If true, enable texture blending along the vertical axis. Default is true.
    bool m_Clamp;                   ///< If true, enable texture clamping. Default is false.
    bool m_ColorCorrection;         ///< If true, enable color correction. Only used on ambient, diffuse, and specular textures.

    uint32_t m_Resolution;          ///< Texture resolution. Default of 0 if no resolution specified.

    float m_BumpMultiplier;         ///< Multiplier of values in bump textures only. Positive or negative. Typical range (0.0 - 1.0)
    float m_Boost;                  ///< Increases sharpness of mip-mapped textures. Positive only. Typical range (1.0 - 5.0)

    float m_RangeModBase;           ///< Modifies texture values by adding a base value. Default is 0.0
    float m_RangeModGain;           ///< Modifies texture values by increasing the contrast. Default is 1.0

    OBJVector3 m_Offset;            ///< Offsets the position of the texture on the surface via shifting. Default (0.0, 0.0, 0.0). (horiz, vert, depth).
    OBJVector3 m_Scale;             ///< Scales the values of the texture. Default (1.0, 1.0, 1.0). (horiz, vert, depth).
    OBJVector3 m_Turbulence;        ///< Applies turbulence to the texture. Default (0.0, 0.0, 0.0) for no turbulence.  (horiz, vert, depth).

    OBJTextureChannel m_imfchan;    ///< Specifies the channel used to create a scalar or bump texture.

    std::string m_Path;             ///< Relative path (from material file) to texture source file. Includes file extension.

private:
};

//------------------------------------------------------------------------------------------

#endif