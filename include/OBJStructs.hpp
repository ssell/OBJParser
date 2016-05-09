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
    OBJVector2();
    OBJVector2 const& operator=(OBJVector2 const& rhs);

    union { float x, r, u, s; };
    union { float y, g, v, t; };
};

BOOST_FUSION_ADAPT_STRUCT(OBJVector2, (float, x), (float, y))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJVector3
 * \brief Simple three-component vector struct
 */
struct OBJVector3
{
    OBJVector3();
    OBJVector3 const& operator=(OBJVector3 const& rhs);

    union { float x, r, u, s; };
    union { float y, g, v, t; };
    union { float z, b, w; };
};

BOOST_FUSION_ADAPT_STRUCT(OBJVector3, (float, x), (float, y), (float, z))

//------------------------------------------------------------------------------------------

/**
 * \stuct OBJVector4
 * \brief Simple four-component vector struct
 */
 struct OBJVector4
 {
    OBJVector4();
    OBJVector4 const& operator=(OBJVector4 const& rhs);

    union { float x, r, u, s; };
    union { float y, g, v, t; };
    union { float z, b, p; };
    union { float w, a, q; };
 };

BOOST_FUSION_ADAPT_STRUCT(OBJVector4, (float, x), (float, y), (float, z), (float, w))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJVertexGroup
 * \brief Index pairing comprising a single vertex of a face.
 *
 * Raw OBJ vertex indices may be negative and are 1-based. However, to keep use of 
 * the API easier and more compliant with standards, all indices are transformed to
 * be positive only and 0-based.
 *
 * With these changes, indices may be used directly within vertex data containers
 * to retrieve the associated vertex element.
 *
 * A negative vertex indicates that it is not in use.
 */
struct OBJVertexGroup
{ 
    OBJVertexGroup();

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
 * A vertex group is in use if it's indexSpatial element is positive.
 * 
 * If all groups are in use, then the face is a quad. <br/>
 * If group3 is not in use, then the face is a triangle.
 */
struct OBJFace
{
    OBJFace();

    //--------------------------------------------------------------------

    OBJVertexGroup group0;  ///< First vertex for the face
    OBJVertexGroup group1;  ///< Second vertex for the face
    OBJVertexGroup group2;  ///< Third vertex for the face
    OBJVertexGroup group3;  ///< Fourth vertex for the face. Used only for Quad faces.

    uint32_t renderState;   ///< The active render attribute state when this face was specified. See OBJState::getRenderState
};

BOOST_FUSION_ADAPT_STRUCT(OBJFace, (OBJVertexGroup, group0), (OBJVertexGroup, group1), (OBJVertexGroup, group2), (OBJVertexGroup, group3), (uint32_t, renderState))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJLine
 * \brief Collection of vertex groups comprising a line.
 *
 * A line may have a variable number of segments, but will always have a minimum of two.
 */
struct OBJLine
{
    OBJLine();
    OBJLine(std::vector<OBJVertexGroup> const& vector);

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
    OBJPoint(); 
    OBJPoint(std::vector<OBJVertexGroup> const& vector);

    std::vector<OBJVertexGroup> points;
};

BOOST_FUSION_ADAPT_STRUCT(OBJPoint, (std::vector<OBJVertexGroup>, points))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJSimpleCurve
 * \brief Individual curve definition comprising a larger free-form object
 */
struct OBJSimpleCurve
{
    OBJSimpleCurve();

    float startParam;             ///< Starting parameter value for the trimming curve
    float endParam;               ///< Ending parameter value for the trimming curve

    int32_t curve2DIndex;         ///< Index of the OBJCurve2D special curve lying in the parameter space of the surface
};

BOOST_FUSION_ADAPT_STRUCT(OBJSimpleCurve, (float, startParam), (float, endParam), (int32_t, curve2DIndex))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJFreeForm
 * \brief A free-form object in the form of a curve or surface
 */
struct OBJFreeForm
{
    OBJFreeForm();

    //--------------------------------------------------------------------

    uint32_t attributeState;                    ///< The active free-form attribute state when this object was created. See OBJState::getFreeFormState

    std::vector<float> parametersU;             ///< Parameter values for the U direction
    std::vector<float> parametersV;             ///< Parameter vlaues for the V direction
    
    std::vector<OBJSimpleCurve> trims;          ///< A sequence of curves to build a single outer trimming loop
    std::vector<OBJSimpleCurve> holes;          ///< A sequence of curves to build a single inner trimming loop (hole)
    std::vector<OBJSimpleCurve> specialCurves;  ///< A sequence of curves to build a single special curve

    std::vector<int32_t> specialPoints;         ///< Special geometric points to be associated with a curve or surface
};

//------------------------------------------------------------------------------------------

/**
 * \struct OBJCurve
 * \brief A standard curve object.
 */
struct OBJCurve : public OBJFreeForm
{
    OBJCurve();
    OBJCurve(float start, float end, std::vector<OBJVertexGroup> const& points);

    //--------------------------------------------------------------------

    float startParam;
    float endParam;

    std::vector<OBJVertexGroup> controlPoints;
};

BOOST_FUSION_ADAPT_STRUCT(OBJCurve, (float, startParam), (float, endParam), (std::vector<OBJVertexGroup>, controlPoints))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJCurve2D
 * \brief A 2D curve on a surface
 */
struct OBJCurve2D : public OBJFreeForm
{
    OBJCurve2D(); 
    OBJCurve2D(std::vector<int32_t> const& points);
    
    //--------------------------------------------------------------------

    std::vector<int32_t> parameterVertexIndices;
};

BOOST_FUSION_ADAPT_STRUCT(OBJCurve2D, (std::vector<int32_t>, parameterVertexIndices))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJSurface
 */
struct OBJSurface : public OBJFreeForm
{
    OBJSurface(); 
    OBJSurface(float startU, float endU, float startV, float endV, std::vector<OBJVertexGroup> const& points);

    //--------------------------------------------------------------------

    float startParamU;
    float endParamU;

    float startParamV;
    float endParamV;

    std::vector<OBJVertexGroup> controlPoints;
};

BOOST_FUSION_ADAPT_STRUCT(OBJSurface, (float, startParamU), (float, endParamU), (float, startParamV), (float, endParamV), (std::vector<OBJVertexGroup>, controlPoints))

//------------------------------------------------------------------------------------------

/**
 * \struct OBJSurfaceConnection
 * \brief Specifies connectivity between two different surfaces.
 */
struct OBJSurfaceConnection
{
    OBJSurfaceConnection();
    OBJSurfaceConnection(int32_t surf1, float startParam1, float endParam1, int32_t curv2D1, int32_t surf2, float startParam2, float endParam2, int32_t curv2D2);

    //--------------------------------------------------------------------

    int32_t surfaceIndex0;        ///< Index of the first surface to be connected. See OBJFreeFormState::surfaces member.
    int32_t surfaceIndex1;        ///< Index of the second surface to be connected. See OBJFreeFormState::surfaces member.

    int32_t curve2DIndex0;        ///< Index of the Curve2D on the first surface that is part of the connection.
    int32_t curve2DIndex1;        ///< Index of the Curve2D on the second surface that is part of the connection.

    float startParam0;            ///< Starting parameter for the first Curve2D.
    float endParam0;              ///< Ending parameter for the first Curve2D.

    float startParam1;            ///< Starting parameter for the second Curve2D.
    float endParam1;              ///< Ending parameter for the second Curve2D.
};

BOOST_FUSION_ADAPT_STRUCT(OBJSurfaceConnection, 
    (int32_t, surfaceIndex0), (float, startParam0), (float, endParam0), (int32_t, curve2DIndex0), 
    (int32_t, surfaceIndex1), (float, startParam1), (float, endParam1), (int32_t, curve2DIndex1))

//------------------------------------------------------------------------------------------

#endif

