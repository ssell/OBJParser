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

#include "OBJStructs.hpp"

//------------------------------------------------------------------------------------------
// OBJVector2
//------------------------------------------------------------------------------------------

OBJVector2::OBJVector2() 
    : x(0.0f), 
      y(0.0f) 
{ 

}

OBJVector2 const& OBJVector2::operator=(OBJVector2 const& rhs) 
{ 
    x = rhs.x; 
    y = rhs.y; 
    
    return (*this); 
}

//------------------------------------------------------------------------------------------
// OBJVector3
//------------------------------------------------------------------------------------------

OBJVector3::OBJVector3() 
    : x(0.0f), 
      y(0.0f),
      z(0.0f)
{ 

}

OBJVector3 const& OBJVector3::operator=(OBJVector3 const& rhs) 
{ 
    x = rhs.x; 
    y = rhs.y; 
    z = rhs.z;

    return (*this); 
}

//------------------------------------------------------------------------------------------
// OBJVector4
//------------------------------------------------------------------------------------------

OBJVector4::OBJVector4() 
    : x(0.0f), 
      y(0.0f),
      z(0.0f),
      w(0.0f)
{ 

}

OBJVector4 const& OBJVector4::operator=(OBJVector4 const& rhs) 
{ 
    x = rhs.x; 
    y = rhs.y; 
    z = rhs.z;
    w = rhs.w;

    return (*this); 
}

//------------------------------------------------------------------------------------------
// OBJVertexGroup
//------------------------------------------------------------------------------------------

OBJVertexGroup::OBJVertexGroup()
    : indexSpatial(-1), 
      indexTexture(-1), 
      indexNormal(-1)
{

}

//------------------------------------------------------------------------------------------
// OBJFace
//------------------------------------------------------------------------------------------

OBJFace::OBJFace()
    : renderState(0)
{
    
}

//------------------------------------------------------------------------------------------
// OBJLine
//------------------------------------------------------------------------------------------

OBJLine::OBJLine()
{

}

OBJLine::OBJLine(std::vector<OBJVertexGroup> const& vector)
{
    segments.reserve(segments.size() + vector.size());
    segments.insert(segments.end(), vector.begin(), vector.end());
}

//------------------------------------------------------------------------------------------
// OBJPoint
//------------------------------------------------------------------------------------------

OBJPoint::OBJPoint()
{

}

OBJPoint::OBJPoint(std::vector<OBJVertexGroup> const& vector)
{
    points.reserve(points.size() + vector.size());
    points.insert(points.end(), vector.begin(), vector.end());
}

//------------------------------------------------------------------------------------------
// OBJSimpleCurve
//------------------------------------------------------------------------------------------

OBJSimpleCurve::OBJSimpleCurve()
    : startParam(0.0f),
      endParam(0.0f),
      curve2DIndex(-1)
{

}

//------------------------------------------------------------------------------------------
// OBJFreeForm
//------------------------------------------------------------------------------------------

OBJFreeForm::OBJFreeForm()
    : attributeState(0)
{

}

//------------------------------------------------------------------------------------------
// OBJCurve
//------------------------------------------------------------------------------------------

OBJCurve::OBJCurve()
    : OBJFreeForm(),
      startParam(0.0f),
      endParam(0.0f)
{

}

OBJCurve::OBJCurve(float const start, float const end, std::vector<OBJVertexGroup> const& points)
    : OBJFreeForm(),
      startParam(start),
      endParam(end)
{
    controlPoints.reserve(points.size());
    controlPoints.insert(controlPoints.begin(), points.begin(), points.end());
}

//------------------------------------------------------------------------------------------
// OBJCurve2D
//------------------------------------------------------------------------------------------

OBJCurve2D::OBJCurve2D()
    : OBJFreeForm()
{

}

OBJCurve2D::OBJCurve2D(std::vector<int32_t> const& points)
    : OBJFreeForm()
{
    parameterVertexIndices.reserve(points.size());
    parameterVertexIndices.insert(parameterVertexIndices.begin(), points.begin(), points.end());
}

//------------------------------------------------------------------------------------------
// OBJSurface
//------------------------------------------------------------------------------------------

OBJSurface::OBJSurface()
    : OBJFreeForm(),
      startParamU(0.0f),
      endParamU(0.0f),
      startParamV(0.0f),
      endParamV(0.0f)
{

}

OBJSurface::OBJSurface(float const startU, float const endU, float const startV, float const endV, std::vector<OBJVertexGroup> const& points)
    : OBJFreeForm(),
      startParamU(startU),
      endParamU(endU),
      startParamV(startV),
      endParamV(endV)
{
    controlPoints.reserve(points.size());
    controlPoints.insert(controlPoints.begin(), points.begin(), points.end());
}

//------------------------------------------------------------------------------------------
// OBJSurfaceConnection
//------------------------------------------------------------------------------------------

OBJSurfaceConnection::OBJSurfaceConnection()
    : surfaceIndex0(-1),
      surfaceIndex1(-1),
      curve2DIndex0(-1),
      curve2DIndex1(-1),
      startParam0(0.0f),
      endParam0(0.0f),
      startParam1(0.0f),
      endParam1(0.0f)
{

}