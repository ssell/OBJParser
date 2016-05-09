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

#ifndef __H__OBJ_PARSER_FREE_FORM_ATTRIBUTE_STATE__H__
#define __H__OBJ_PARSER_FREE_FORM_ATTRIBUTE_STATE__H__

#include <boost/fusion/adapted.hpp>
#include <cstdint>
#include <vector>

//------------------------------------------------------------------------------------------

enum class OBJFreeFormType
{
    None = 0, 
    BasisMatrix,
    Bezier,
    BSpline,
    Cardinal,
    Taylor
};

/**
 * \struct OBJFreeFormAttributeState
 */
struct OBJFreeFormAttributeState
{
    OBJFreeFormAttributeState()
        : type(OBJFreeFormType::None),
          rational(false),
          degreeU(0),
          degreeV(0),
          stepU(0),
          stepV(0),
          mergeGroupNumber(0),
          mergeGroupResolution(0.0f)
    {
    
    }

    //--------------------------------------------------------------------

    OBJFreeFormType type;                   ///< 
    bool rational;                          ///< 

    int32_t degreeU;                        ///< 
    int32_t degreeV;                        ///< 

    int32_t stepU;                          ///< 
    int32_t stepV;                          ///< 

    int32_t mergeGroupNumber;               ///< Merging group number. A value of 0 indicates no adjacency detection.
    float mergeGroupResolution;             ///< Maximum distance between two merged surfaces. Must be greater than 0.0 if merging is on.

    std::vector<float> basisMatrixU;        ///< 
    std::vector<float> basisMatrixV;        ///< 
};

//------------------------------------------------------------------------------------------

#endif