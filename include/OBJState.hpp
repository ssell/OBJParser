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

#ifndef __H__OBJ_PARSER_STATE__H__
#define __H__OBJ_PARSER_STATE__H__

#include "OBJGroup.hpp"
#include <unordered_map>

//------------------------------------------------------------------------------------------

/**
 * \class OBJState
 *
 * OBJ is a state-based file format, and this class represents that state.
 * It keeps track of all groups and their associated data.
 *
 * After parsing, this data can then be retrieved from the state object.
 */
class OBJState
{
public:

    OBJState();
    ~OBJState();
    
    void clearState();
    void reserve(uint32_t spatial, uint32_t texture, uint32_t normal, uint32_t groupIndices);

    void clearActiveGroups();
    void addActiveGroup(std::string const& name);

    void addVertexSpatial(OBJVector3 const& vector);
    void addVertexTexture(OBJVector2 const& vector);
    void addVertexNormal(OBJVector3 const& vector);
    
    void addFace(OBJFace face);
    void addLine(std::vector<OBJVertexGroup>& line);
    void addPointCollection(OBJPoint& points);

protected:

    void transformVertexGroup(OBJVertexGroup& source) const;

    //--------------------------------------------------------------------

    uint32_t m_GroupIndicesReservedSize;

    std::unordered_map<std::string, OBJGroup> m_GroupMap;
    std::vector<OBJGroup*> m_ActiveGroups;

    std::vector<OBJVector3> m_VertexSpatialData;
    std::vector<OBJVector2> m_VertexTextureData;    
    std::vector<OBJVector3> m_VertexNormalData;

private:
};

//------------------------------------------------------------------------------------------

#endif