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
        groupPtr->indices.reserve(m_GroupIndicesReservedSize);
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

void OBJState::addFace(OBJFaceIndex const& i0, OBJFaceIndex const& i1, OBJFaceIndex const& i2)
{
    const OBJFaceIndex transformed0 = transformIndex(i0);
    const OBJFaceIndex transformed1 = transformIndex(i1);
    const OBJFaceIndex transformed2 = transformIndex(i2);
    
    for(auto iter = m_ActiveGroups.begin(); iter != m_ActiveGroups.end(); ++iter)
    {
        // Add triangle face to group

        (*iter)->indices.emplace_back(transformed0);
        (*iter)->indices.emplace_back(transformed1);
        (*iter)->indices.emplace_back(transformed2);
    }
}

void OBJState::addFace(OBJFaceIndex const& i0, OBJFaceIndex const& i1, OBJFaceIndex const& i2, OBJFaceIndex const& i3)
{
    const OBJFaceIndex transformed0 = transformIndex(i0);
    const OBJFaceIndex transformed1 = transformIndex(i1);
    const OBJFaceIndex transformed2 = transformIndex(i2);
    const OBJFaceIndex transformed3 = transformIndex(i3);

    for(auto iter = m_ActiveGroups.begin(); iter != m_ActiveGroups.end(); ++iter)
    {
        // Convert quad face to two triangles and add to group

        (*iter)->indices.emplace_back(transformed0);
        (*iter)->indices.emplace_back(transformed1);
        (*iter)->indices.emplace_back(transformed2);

        (*iter)->indices.emplace_back(transformed2);
        (*iter)->indices.emplace_back(transformed3);
        (*iter)->indices.emplace_back(transformed0);
    }
}

//------------------------------------------------------------------------------------------
// Protected Methods
//------------------------------------------------------------------------------------------

OBJFaceIndex OBJState::transformIndex(OBJFaceIndex const& source) const
{
    // Incoming indices are 1-based AND may be negative.
    // We want to transform them so they appropriately match our data containers (0-based positive)

    OBJFaceIndex result = source;

    if(result.indexSpatial < 0)
    {
        result.indexSpatial += static_cast<uint32_t>(m_VertexSpatialData.size());
    }
    else
    {
        result.indexSpatial--; // 0-base
    }

    if(result.indexTexture < 0)
    {
        result.indexTexture += static_cast<uint32_t>(m_VertexTextureData.size());
    }
    else
    {
        result.indexTexture--; // 0-base
    }

    if(result.indexNormal < 0)
    {
        result.indexNormal += static_cast<uint32_t>(m_VertexNormalData.size());
    }
    else
    {
        result.indexNormal--; // 0-base
    }

    return result;
}

//------------------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------------------