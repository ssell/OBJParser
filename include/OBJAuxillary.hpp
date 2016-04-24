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

#ifndef __H__OBJ_PARSER_AUXILIARY__H__
#define __H__OBJ_PARSER_AUXILIARY__H__

#include <string>
#include <cstdint>

//------------------------------------------------------------------------------------------

/**
 * \struct OBJAuxiliary
 * \brief Struct of optional state settings.
 *
 * The settings specified in this struct are on a per-face or free-form basis.
 * Most commonly used of these is the material name. The rest have highly varying
 * levels of support among OBJ writers, other readers, and end-use implementations.
 *
 * Multiple faces/free-forms may reference the same state.
 */
struct OBJAuxiliary
{
    OBJAuxiliary()
        : smoothing(0), lod(0), bevelInterp(false), colorInterp(false), dissolveInterp(false)
    {
    
    }

    //--------------------------------------------------------------------

    uint32_t smoothing;        ///< Smoothing group number. Default/no smoothing group is 0.
    uint32_t lod;              ///< Level-of-detail displayed. Off is 0. Otherwise, range of [1,100]

    bool bevelInterp;          ///< Sets bevel interpolation on/off. Default off.
    bool colorInterp;          ///< Sets color interpolation on/off. Default off.
    bool dissolveInterp;       ///< Sets dissolve interpolation on/off. Default off.

    std::string material;      ///< Specifies the material to use. Empty means no material. Once set, material can only be changed.
    std::string textureMap;    ///< Specifies the texture map to use. Empty or "off" means no map specified.
    std::string shadowObj;     ///< Specifies the shadow object filename. Empty means no object specified.
    std::string traceObj;      ///< Specifies the ray tracing object filename. Empty means no object specified.
};

//------------------------------------------------------------------------------------------

#endif