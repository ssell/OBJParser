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

#ifndef __H__OBJ_PARSER_STRUCTS__H__
#define __H__OBJ_PARSER_STRUCTS__H__

#include <boost/fusion/adapted.hpp>

#include <string>
#include <cstdint>
#include <vector>

//------------------------------------------------------------------------------------------

struct OBJVector2
{
    float x;
    float y;
};

BOOST_FUSION_ADAPT_STRUCT(OBJVector2, (float, x), (float, y))

struct OBJVector3
{
    float x;
    float y;
    float z;
};

BOOST_FUSION_ADAPT_STRUCT(OBJVector3, (float, x), (float, y), (float, z))

struct OBJFaceIndex
{
    uint32_t indexSpatial;
    uint32_t indexTexture;
    uint32_t indexNormal;
};

BOOST_FUSION_ADAPT_STRUCT(OBJFaceIndex, (uint32_t, indexSpatial), (uint32_t, indexTexture), (uint32_t, indexNormal))

struct OBJGroup
{
    std::string name;
    std::vector<OBJFaceIndex> indices; 

    bool active;
};

//------------------------------------------------------------------------------------------

#endif

