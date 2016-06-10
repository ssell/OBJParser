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

#ifndef __H__OBJ_PARSER_RENDER_STATE__H__
#define __H__OBJ_PARSER_RENDER_STATE__H__

#include <boost/fusion/adapted.hpp>

#include <string>
#include <cstdint>

//------------------------------------------------------------------------------------------

enum class OBJSubdivision
{
    None = 0,                     ///< No technique specified (technique not in use)
    Parametric,                   ///< Constant parametric subdivision using one resolution parameter
    ParametricA,                  ///< Constant parametric subdivision using separate resolution parameters for U and V
    ParametricB,                  ///< Constant parametric subdivision using one resolution parameter for both U and V
    Spatial,                      ///< Constant spatial subdivison
    Curvature,                    ///< Curvature-dependent subdivision

};

/**
 * \struct OBJCurveTechnique
 * \brief Specifies free-form curve subdivision technique.
 */
struct OBJCurveTechnique
{
    OBJCurveTechnique()
        : technique(OBJSubdivision::None),
          res(0.0f),
          maxLength(0.0f),
          maxDistance(0.0f),
          maxAngle(0.0f)
    {

    }

    OBJSubdivision technique;     ///< Subdivision type. For curves this may be: Parametric, Spatial, or Curvature. If None, then this technique has not been specified.

    float res;                    ///< Resolution used with Parametric subdivision. 0.0 if not used.

    float maxLength;              ///< Maximum length of line segments with Spatial subdivision. 0.0 if not used.
    float maxDistance;            ///< Maximum distance between line segments and curve in Curvature subdivision. 0.0 if not used.
    float maxAngle;               ///< Angle between tangent vectors at line segment ends in Curvature subdivision. 0.0 if not used.
};

/**
 * \struct OBJSurfaceTechnique
 * \brief Specifies free-form surface subdivision technique.
 */
struct OBJSurfaceTechnique
{
    OBJSurfaceTechnique()
        : technique(OBJSubdivision::None),
          resU(0.0f),
          resV(0.0f),
          maxLength(0.0f),
          maxDistance(0.0f),
          maxAngle(0.0f)
    {

    }

    OBJSubdivision technique;     ///< Subdivision type. For surfaces this may be: ParametricA, ParametricB, Spatial, or Curvature. If None, then this technique has not been specified.

    float resU;                   ///< Resolution parameter for the U direction with Parametric subdivision. If technique is ParametricB, then resU == resV
    float resV;                   ///< Resolution parameter for the V direction with Parametric subdivision. If technique is ParametricB, then resU == resV

    float maxLength;              ///< Maximum length of line segments with Spatial subdivision. 0.0 if not used.
    float maxDistance;            ///< Maximum distance between line segments and curve in Curvature subdivision. 0.0 if not used.
    float maxAngle;               ///< Angle between tangent vectors at line segment ends in Curvature subdivision. 0.0 if not used.
};

/**
 * \struct OBJRenderState
 * \brief Struct of optional state settings.
 *
 * The settings specified in this struct are on a per-face or free-form basis.
 * Most commonly used of these is the material name. The rest have highly varying
 * levels of support among OBJ writers, other readers, and end-use implementations.
 *
 * Multiple faces/free-forms may reference the same state.
 */
struct OBJRenderState
{
    OBJRenderState()
        : smoothing(0), lod(0), bevelInterp(false), colorInterp(false), dissolveInterp(false)
    {
    
    }

    //--------------------------------------------------------------------

    uint32_t smoothing;                      ///< Smoothing group number. Default/no smoothing group is 0.
    uint32_t lod;                            ///< Level-of-detail displayed. Off is 0. Otherwise, range of [1,100]

    bool bevelInterp;                        ///< Sets bevel interpolation on/off. Default off. Polygons only.
    bool colorInterp;                        ///< Sets color interpolation on/off. Default off. Polygons only.
    bool dissolveInterp;                     ///< Sets dissolve interpolation on/off. Default off. Polygons only.

    std::string material;                    ///< Specifies the material to use. Empty means no material. Once set, material can only be changed.
    std::string textureMap;                  ///< Specifies the texture map to use. Empty or "off" means no map specified.
    std::string shadowObj;                   ///< Specifies the shadow object filename. Empty means no object specified.
    std::string traceObj;                    ///< Specifies the ray tracing object filename. Empty means no object specified.

    OBJCurveTechnique curveTechnique;        ///< Specifies the curve approximation technique. Free-forms only.
    OBJSurfaceTechnique surfaceTechnique;    ///< Specified the surface approximation technique. Free-froms only.
};

//------------------------------------------------------------------------------------------

#endif