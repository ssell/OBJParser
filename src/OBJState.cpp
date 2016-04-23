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

#include "OBJState.hpp"

//------------------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------------------

OBJState::OBJState()
    : m_GroupIndicesReservedSize(0)
{

}

OBJState::~OBJState()
{
    clearState();
}

//------------------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------------------

void OBJState::clearState()
{
    m_ActiveGroups.clear();
    m_GroupMap.clear();
}

void OBJState::reserve(uint32_t const spatial, uint32_t const texture, uint32_t const normal, uint32_t const groupIndices)
{
    m_VertexSpatialData.reserve(static_cast<std::vector<OBJVector3>::size_type>(spatial));
    m_VertexTextureData.reserve(static_cast<std::vector<OBJVector3>::size_type>(texture));
    m_VertexNormalData.reserve(static_cast<std::vector<OBJVector3>::size_type>(normal));

    m_GroupIndicesReservedSize = groupIndices;
}

void OBJState::clearActiveGroups()
{
    for(auto iter = m_ActiveGroups.begin(); iter != m_ActiveGroups.end(); ++iter)
    {
        (*iter)->active = false;
    }

    m_ActiveGroups.clear();
}

void OBJState::addActiveGroup(std::string const& name)
{
    OBJGroup* groupPtr = nullptr;
    auto findGroup = m_GroupMap.find(name);

    if(findGroup != m_GroupMap.end())
    {
        groupPtr = &(*findGroup).second;

        if(groupPtr->active)
        {
            // Already marked as active
            groupPtr = nullptr;
        }
    }
    else
    {
        m_GroupMap[name] = OBJGroup();
        groupPtr = &(*m_GroupMap.find(name)).second;
        groupPtr->name = name;
        groupPtr->faces.reserve(m_GroupIndicesReservedSize);
    }

    if(groupPtr)
    {
        m_ActiveGroups.push_back(groupPtr);
        groupPtr->active = true;
    }
}

void OBJState::addVertexSpatial(OBJVector3 const& vector)
{
    m_VertexSpatialData.emplace_back(vector);
}

void OBJState::addVertexTexture(OBJVector2 const& vector)
{
    m_VertexTextureData.emplace_back(vector);
}

void OBJState::addVertexNormal(OBJVector3 const& vector)
{
    m_VertexNormalData.emplace_back(vector);
}

void OBJState::addFace(OBJFace face)
{
    transformVertexGroup(face.group0);
    transformVertexGroup(face.group1);
    transformVertexGroup(face.group2);
    transformVertexGroup(face.group3);

    for(auto iter = m_ActiveGroups.begin(); iter != m_ActiveGroups.end(); ++iter)
    {
        (*iter)->faces.emplace_back(face);
    }
}

void OBJState::addLine(std::vector<OBJVertexGroup>& line)
{
    for(auto iter = line.begin(); iter != line.end(); ++iter)
    {
        transformVertexGroup((*iter));
    }

    for(auto iter = m_ActiveGroups.begin(); iter != m_ActiveGroups.end(); ++iter)
    {
        (*iter)->addLine(line);
    }
}

void OBJState::addPointCollection(OBJPoint& points)
{
    for(auto iter = points.points.begin(); iter != points.points.end(); ++iter)
    {
        transformVertexGroup((*iter));
    }

    for(auto iter = m_ActiveGroups.begin(); iter != m_ActiveGroups.end(); ++iter)
    {
        (*iter)->addPointCollection(points);
    }
}

//------------------------------------------------------------------------------------------
// Protected Methods
//------------------------------------------------------------------------------------------

void OBJState::transformVertexGroup(OBJVertexGroup& source) const
{
    // Incoming indices may be negative.
    // We want to transform them so they are positive.

    if(source.indexSpatial < 0)
    {
        source.indexSpatial += static_cast<int32_t>(m_VertexSpatialData.size()) + 1;  // +1 to maintain index 1-base
    }

    if(source.indexTexture < 0)
    {
        source.indexTexture += static_cast<int32_t>(m_VertexTextureData.size()) + 1;  // +1 to maintain index 1-base
    }

    if(source.indexNormal < 0)
    {
        source.indexNormal += static_cast<int32_t>(m_VertexNormalData.size()) + 1;    // +1 to maintain index 1-base
    }
}

//------------------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------------------