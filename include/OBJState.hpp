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
#include "OBJAuxillary.hpp"

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
    
    //--------------------------------------------------------------------

    /**
     * Resets the state back to a default setting.
     * Typically called automatically prior to starting a new parse.
     */
    void clearState();

    /**
     * Allows the ability to specify the amount of space to reserve for the
     * various containers used by the state. If one knows in advance the general
     * size of the parsed objects, then reserving an adequate amount of space
     * in advance may produce performance improvements.
     *
     * Reserved size can only ever be increased and may not be decreased.
     *
     * By default, the state will not reserve any space in advance.
     *
     * \param[in] spatial        Number of spatial vertex elements to reserve for.
     * \param[in] texture        Number of texture vertex elements to reserve for.
     * \param[in] normal         Number of normals vertex elements to reserve for.
     * \param[in] groupFaces     Number of faces to reserve in each new group.
     * \param[in] groupFreeForms Number of free-forms to reserve in each new group.
     */
    void reserve(uint32_t spatial, uint32_t texture = 0, uint32_t normal = 0, uint32_t groupFaces = 0, uint32_t groupFreeForms = 0);

    /**
     * Retrieves the specified auxiliary state.
     *
     * Each face and free-form has an associated auxiliary state detailing extra state 
     * settings that were active upon it's creation (such as material, lod, smoothing, etc.).
     *
     * \param[in] index Auxiliary state index. If invalid, will return the default state.
     * \return The specified auxiliary state.
     */
    OBJAuxiliary getAuxiliaryState(uint32_t index) const;

    /**
     * Fills a vector with pointers to all OBJGroups stored in the state.
     *
     * If the state is cleared (whether explicitly or upon a new parse action), then
     * all group pointers will become invalidated.
     *
     * \note The provided vector is cleared prior to filling with groups.
     *
     * \param[out] groups
     */
    void getGroups(std::vector<OBJGroup const*>& groups) const;

    //--------------------------------------------------------------------
    // Parser/Grammar Methods
    //--------------------------------------------------------------------

    /**
     * Clears all active groups.
     *
     * \note Typically should only be used by the OBJGrammar class.
     */
    void clearActiveGroups();

    /**
     * Adds an additional active group.
     * If the group has not been added before, then a new group is created.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] name Group name.
     */
    void addActiveGroup(std::string const& name);

    /**
     * Adds a new spatial (x, y, z, w) vertex element.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] vector Spatial vertex to add.
     */
    void addVertexSpatial(OBJVector4 const& vector);

    /**
     * Adds a new texture (u, v) vertex element.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] vector Texture vertex to add.
     */
    void addVertexTexture(OBJVector2 const& vector);

    /**
     * Adds a new normal (x, y, z) vertex element.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] vector Normal vertex to add.
     */
    void addVertexNormal(OBJVector3 const& vector);

    /**
     * Adds a new face element.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] face Face to add.
     */
    void addFace(OBJFace face);

    /**
     * Adds a new line element.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] line Line to add.
     */
    void addLine(std::vector<OBJVertexGroup>& line);

    /**
     * Adds a new point element.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] points Points to add.
     */
    void addPointCollection(std::vector<OBJVertexGroup>& points);
    
    void setSmoothingGroup(uint32_t group);
    void setLevelOfDetail(uint32_t lod);
    void setBevelInterp(bool on);
    void setColorInterp(bool on);
    void setDissolveInterp(bool on);
    void setMaterial(std::string const& name);
    void addMaterialLibrary(std::string const& name);
    void setTextureMap(std::string const& name);
    void addTextureMapLibrary(std::string const& name);
    void setShadowObject(std::string const& name);
    void setTracingObject(std::string const& name);

protected:

    void resetAuxiliaryStates();
    void transformVertexGroup(OBJVertexGroup& source) const;
    
    //--------------------------------------------------------------------

    uint32_t m_GroupFacesReservedSize;
    uint32_t m_GroupFreeFormReservedSize;

    std::unordered_map<std::string, OBJGroup> m_GroupMap;
    std::vector<OBJGroup*> m_ActiveGroups;

    std::vector<OBJVector4> m_VertexSpatialData;
    std::vector<OBJVector2> m_VertexTextureData;    
    std::vector<OBJVector3> m_VertexNormalData;
    
    std::vector<std::string> m_MaterialLibraries;
    std::vector<std::string> m_TextureMapLibraries;

    std::vector<OBJAuxiliary> m_AuxiliaryStates;

private:
};

//------------------------------------------------------------------------------------------

#endif