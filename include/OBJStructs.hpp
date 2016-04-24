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

/**
 * \struct OBJVector2
 * \brief Simple two-component vector struct
 */
struct OBJVector2
{
    OBJVector2() : x(0.0f), y(0.0f) { }

    float x;
    float y;
};

BOOST_FUSION_ADAPT_STRUCT(OBJVector2, (float, x), (float, y))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJVector3
 * \brief Simple three-component vector struct
 */
struct OBJVector3
{
    OBJVector3() : x(0.0f), y(0.0f), z(0.0f) { }

    float x;
    float y;
    float z;
};

BOOST_FUSION_ADAPT_STRUCT(OBJVector3, (float, x), (float, y), (float, z))

//------------------------------------------------------------------------------------------

/**
 * \stuct OBJVector4
 * \brief Simple four-component vector struct
 */
 struct OBJVector4
 {
     OBJVector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) { }

     float x;
     float y;
     float z;
     float w;
 };

BOOST_FUSION_ADAPT_STRUCT(OBJVector4, (float, x), (float, y), (float, z), (float, w))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJVertexGroup
 * \brief Index pairing comprising a single vertex of a face.
 *
 * Raw OBJ vertex indices are 1-based and that is maintained inside of this struct.
 *
 * An index with value 0 indicates that the index is unspecified and thus unused.
 * If the spatial index is 0, then the entire grouping is unused. This may occur
 * if face data is provided as triangles and thus the fourth group in the OBJFace
 * struct is not needed.
 *
 * OBJ vertex indices may also be negative, but all indices in this struct have
 * been transformed and are positive.
 */
struct OBJVertexGroup
{ 
    OBJVertexGroup() : indexSpatial(0), indexTexture(0), indexNormal(0) { }

    int32_t indexSpatial;
    int32_t indexTexture;
    int32_t indexNormal;
};

BOOST_FUSION_ADAPT_STRUCT(OBJVertexGroup, (int32_t, indexSpatial), (int32_t, indexTexture), (int32_t, indexNormal))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJFace
 * \brief Collection of vertex groups comprising a single face.
 *
 * A face may represent one of the following:
 *
 *    - Triangle
 *    - Quad
 *
 * You can check what is represented by seeing which vertex groups are in use.
 * A vertex group is in use if it's indexSpatial element is not 0.
 * 
 * If all groups are in use, then the face is a quad. <br/>
 * If group3 is not in use, then the face is a triangle.
 */
struct OBJFace
{
    OBJFace()
        : auxState(0)
    {
    
    }

    //--------------------------------------------------------------------

    OBJVertexGroup group0;  ///< First vertex for the face
    OBJVertexGroup group1;  ///< Second vertex for the face
    OBJVertexGroup group2;  ///< Third vertex for the face
    OBJVertexGroup group3;  ///< Fourth vertex for the face. Used only for Quad faces.

    uint32_t auxState;      ///< The active auxiliary state when this face was specified. See OBJState::getAuxiliaryState
};

BOOST_FUSION_ADAPT_STRUCT(OBJFace, (OBJVertexGroup, group0), (OBJVertexGroup, group1), (OBJVertexGroup, group2), (OBJVertexGroup, group3), (uint32_t, auxState))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJLine
 * \brief Collection of vertex groups comprising a line.
 *
 * A line may have a variable number of segments, but will always have a minimum of two.
 */
struct OBJLine
{
    OBJLine() 
    { 

    }

    OBJLine(std::vector<OBJVertexGroup> const& vector)
    {
        segments.reserve(segments.size() + vector.size());
        segments.insert(segments.end(), vector.begin(), vector.end());
    }

    std::vector<OBJVertexGroup> segments;
};

BOOST_FUSION_ADAPT_STRUCT(OBJLine, (std::vector<OBJVertexGroup>, segments))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJPoint
 * \brief Collection of vertex groups comprising a point collection.
 *
 * A point collection may have a variable number of points, but will always have a minimum of one.
 */
struct OBJPoint
{
    OBJPoint() 
    { 

    }

    OBJPoint(std::vector<OBJVertexGroup> const& vector)
    {
        points.reserve(points.size() + vector.size());
        points.insert(points.end(), vector.begin(), vector.end());
    }

    std::vector<OBJVertexGroup> points;
};

BOOST_FUSION_ADAPT_STRUCT(OBJPoint, (std::vector<OBJVertexGroup>, points))

//------------------------------------------------------------------------------------------

#endif

