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
    : m_GroupFacesReservedSize(0),
      m_GroupFreeFormReservedSize(0)
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
    m_VertexSpatialData.clear();
    m_VertexTextureData.clear();
    m_VertexNormalData.clear();

    m_ActiveGroups.clear();
    m_GroupMap.clear();
    m_MaterialLibraries.clear();
    m_TextureMapLibraries.clear();

    resetAuxiliaryStates();
}

void OBJState::reserve(uint32_t const spatial, uint32_t const texture, uint32_t const normal, uint32_t const groupIndices, uint32_t const groupFreeForms)
{
    m_VertexSpatialData.reserve(static_cast<std::vector<OBJVector3>::size_type>(spatial));
    m_VertexTextureData.reserve(static_cast<std::vector<OBJVector3>::size_type>(texture));
    m_VertexNormalData.reserve(static_cast<std::vector<OBJVector3>::size_type>(normal));

    m_GroupFacesReservedSize = groupIndices;
    m_GroupFreeFormReservedSize = groupFreeForms;
}

OBJAuxiliary OBJState::getAuxiliaryState(uint32_t index) const
{
    OBJAuxiliary result;

    if(index < m_AuxiliaryStates.size())
    {
        result = m_AuxiliaryStates[index];
    }

    return result;
}

void OBJState::getGroups(std::vector<OBJGroup const*>& groups) const
{
    groups.clear();
    groups.reserve(m_GroupMap.size());

    for(auto iter = m_GroupMap.begin(); iter != m_GroupMap.end(); ++iter)
    {
        groups.emplace_back(&(*iter).second);
    }
}

std::vector<OBJVector4> const* OBJState::getSpatialData() const
{
    return &m_VertexSpatialData;
}

std::vector<OBJVector2> const* OBJState::getTextureData() const
{
    return &m_VertexTextureData;
}

std::vector<OBJVector3> const* OBJState::getNormalData() const
{
    return &m_VertexNormalData;
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
        groupPtr->faces.reserve(m_GroupFacesReservedSize);
    }

    if(groupPtr)
    {
        m_ActiveGroups.push_back(groupPtr);
        groupPtr->active = true;
    }
}

void OBJState::addVertexSpatial(OBJVector4 const& vector)
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

    face.auxState = static_cast<uint32_t>(m_AuxiliaryStates.size() - 1);

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

void OBJState::addPointCollection(std::vector<OBJVertexGroup>& points)
{
    for(auto iter = points.begin(); iter != points.end(); ++iter)
    {
        transformVertexGroup((*iter));
    }

    for(auto iter = m_ActiveGroups.begin(); iter != m_ActiveGroups.end(); ++iter)
    {
        (*iter)->addPointCollection(points);
    }
}

void OBJState::setLevelOfDetail(uint32_t const lod)
{
    OBJAuxiliary auxState = m_AuxiliaryStates.back();
    auxState.lod = lod;

    if(auxState.lod > 100)
    {
        auxState.lod = 100;    // "Specifying an integer between 1 and 100 sets the..."
    }

    m_AuxiliaryStates.emplace_back(auxState);
}

void OBJState::setSmoothingGroup(uint32_t const group)
{
    OBJAuxiliary auxState = m_AuxiliaryStates.back();
    auxState.smoothing = group;

    m_AuxiliaryStates.emplace_back(auxState);
}

void OBJState::setBevelInterp(bool const on)
{
    OBJAuxiliary auxState = m_AuxiliaryStates.back();
    auxState.bevelInterp = on;

    m_AuxiliaryStates.emplace_back(auxState);
}

void OBJState::setColorInterp(bool const on)
{
    OBJAuxiliary auxState = m_AuxiliaryStates.back();
    auxState.colorInterp = on;

    m_AuxiliaryStates.emplace_back(auxState);
}

void OBJState::setDissolveInterp(bool const on)
{
    OBJAuxiliary auxState = m_AuxiliaryStates.back();
    auxState.dissolveInterp = on;

    m_AuxiliaryStates.emplace_back(auxState);
}

void OBJState::setMaterial(std::string const& name)
{
    OBJAuxiliary auxState = m_AuxiliaryStates.back();
    auxState.material = name;

    m_AuxiliaryStates.emplace_back(auxState);
}

void OBJState::addMaterialLibrary(std::string const& name)
{
    m_MaterialLibraries.push_back(name);
}

void OBJState::setTextureMap(std::string const& name)
{
    OBJAuxiliary auxState = m_AuxiliaryStates.back();
    auxState.textureMap = name;

    m_AuxiliaryStates.emplace_back(auxState);
}

void OBJState::addTextureMapLibrary(std::string const& name)
{
    m_TextureMapLibraries.push_back(name);
}

void OBJState::setShadowObject(std::string const& name)
{
    OBJAuxiliary auxState = m_AuxiliaryStates.back();
    auxState.shadowObj = name;

    if(auxState.shadowObj.compare("off") == 0)
    {
        auxState.shadowObj = "";
    }

    m_AuxiliaryStates.emplace_back(auxState);
}

void OBJState::setTracingObject(std::string const& name)
{
    OBJAuxiliary auxState = m_AuxiliaryStates.back();
    auxState.traceObj = name;

    if(auxState.traceObj.compare("off") == 0)
    {
        auxState.traceObj = "";
    }

    m_AuxiliaryStates.emplace_back(auxState);
}

//------------------------------------------------------------------------------------------
// Protected Methods
//------------------------------------------------------------------------------------------

void OBJState::resetAuxiliaryStates() 
{
    m_AuxiliaryStates.clear();
    m_AuxiliaryStates.reserve(50);                   // Arbitrary reserve
    m_AuxiliaryStates.emplace_back(OBJAuxiliary());  // Set initial default state
}

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