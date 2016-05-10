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

#include "OBJFreeFormState.hpp"
#include "OBJGroup.hpp"
#include "OBJRenderState.hpp"
#include "OBJMaterial.hpp"

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
     * Returns a pointer to the internal OBJFreeForm state. 
     * This state defines all free-form geometries, connections, and most of their attributes.
     *
     * \note This pointer will never be invalid, but the state will be cleared when clearState is called.
     */
    OBJFreeFormState* getFreeFormState();

    /**
     * Retrieves the specified state of the render attributes.
     *
     * Each face and free-form has an associated render attributes state detailing extra state 
     * settings that were active upon it's creation (such as material, lod, smoothing, etc.).
     *
     * \param[in] index Render attribute state index. If invalid, will return the default state.
     * \return The specified render attribute state.
     */
    OBJRenderState getRenderState(uint32_t index) const;

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

    /**
     * Returns a pointer to the container of all parsed spatial vertex data.
     * \note Keep in mind that OBJ indices are 1-based while the data container indices are 0-based.
     */
    std::vector<OBJVector4> const* getSpatialData() const;

    /**
     * Returns a pointer to the container of all parsed texture coordinate vertex data.
     * \note Keep in mind that OBJ indices are 1-based while the data container indices are 0-based.
     */
    std::vector<OBJVector2> const* getTextureData() const;

    /**
     * Returns a pointer to the container of all parsed normal vertex data.
     * \note Keep in mind that OBJ indices are 1-based while the data container indices are 0-based.
     */
    std::vector<OBJVector3> const* getNormalData() const;

    /**
     * Returns a pointer to the container of all material libraries (accompanying .mtl files).
     */
    std::vector<std::string> const* getMaterialLibraries() const;

    void getMaterials(std::vector<OBJMaterial const*>& materials) const;

    //--------------------------------------------------------------------
    // OBJ Parser/Grammar Methods
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

    //--------------------------------------------------------------------
    // Vertex Data Methods
    //--------------------------------------------------------------------

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
     * Adds a new free-form parameter (u, v, w) vertex element.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] vector Free-Form parameter vertex to add.
     */
    void addVertexParameter(OBJVector3 const& vector);

    //--------------------------------------------------------------------
    // Geometry Creation Methods
    //--------------------------------------------------------------------

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

    /**
     * Adds a new OBJCurve to the internal OBJFreeFormState.
     * \param[in] curve
     */
    void addFreeFormCurve(OBJCurve const& curve);

    /**
     * Adds a new OBJCurve2D to the internal OBJFreeFormState.
     * \param[in] points
     */
    void addFreeFormCurve2D(std::vector<int32_t> const& points);

    /**
     * Adds a new OBJSurface to the internal OBJFreeFormState.
     * \param[in] surface
     */
    void addFreeFormSurface(OBJSurface const& surface);

    /**
     * Adds a new OBJSurfaceConnection to the internal OBJFreeFormState.
     * \param[in] connection
     */
    void addFreeFormConnection(OBJSurfaceConnection connection);

    //--------------------------------------------------------------------
    // Free-Form Body Methods
    //--------------------------------------------------------------------

    /**
     * Adds parameter u values to the newest OBJFreeForm in the internal OBJFreeFormState.
     * \param[in] parameters
     */
    void addFreeFormParameterU(std::vector<float> const& parameters);

    /**
     * Adds parameter v values to the newest OBJFreeForm in the internal OBJFreeFormState.
     * \param[in] parameters
     */
    void addFreeFormParameterV(std::vector<float> const& parameters);

    /**
     * Adds trim values to the newest OBJFreeForm in the internal OBJFreeFormState.
     * \param[in] trim
     */
    void addFreeFormTrim(OBJSimpleCurve const& trim);

    /**
     * Adds hole values to the newest OBJFreeForm in the internal OBJFreeFormState.
     * \param[in] hole
     */
    void addFreeFormHole(OBJSimpleCurve const& hole);

    /**
     * Adds scurve values to the newest OBJFreeForm in the internal OBJFreeFormState.
     * \param[in] scurve
     */
    void addFreeFormSpecialCurve(OBJSimpleCurve const& scurve);

    /**
     * Adds points values to the newest OBJFreeForm in the internal OBJFreeFormState.
     * \param[in] points
     */
    void addFreeFormSpecialPoints(std::vector<int32_t> const& points);

    //--------------------------------------------------------------------
    // Render State Setting Methods
    //--------------------------------------------------------------------

    /**
     * Sets the smoothing group for the current auxiliary state.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] group
     */
    void setSmoothingGroup(uint32_t group);
    
    /**
     * Sets the level-of-detail for the current auxiliary state.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] lod
     */
    void setLevelOfDetail(uint32_t lod);
    
    /**
     * Enables/disables bevel interpolation for the current auxiliary state.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] on
     */
    void setBevelInterp(bool on);
    
    /**
     * Enables/disables color interpolation for the current auxiliary state.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] on
     */
    void setColorInterp(bool on);
    
    /**
     * Enables/disables dissolve interpolation for the current auxiliary state.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] on
     */
    void setDissolveInterp(bool on);
    
    /**
     * Sets the bound material for the current auxiliary state.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] name
     */
    void setMaterial(std::string const& name);

    /**
     * Sets the material data for the specified material name.
     *
     * \note Typically should only be used by the MTLGrammar class.
     *
     * \param[in] name
     * \param[in] material
     */
    void setMaterial(std::string const& name, OBJMaterial const& material);
    
    /**
     * Adds a new material library to create materials from.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] path Relative path, including extension, from the current OBJ file. 
     */
    void addMaterialLibrary(std::string const& path);
    
    /**
     * Sets the bound texture for the current auxiliary state.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] name
     */
    void setTextureMap(std::string const& name);
    
    /**
     * Adds a new texture library to create textures from.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] path Relative path, including extension, from the current OBJ file. 
     */
    void addTextureMapLibrary(std::string const& path);
    
    /**
     * Sets the bound shadow object for the current auxiliary state.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] name
     */
    void setShadowObject(std::string const& name);
    
    /**
     * Sets the bound tracing object for the current auxiliary state.
     *
     * \note Typically should only be used by the OBJGrammar class.
     *
     * \param[in] name
     */
    void setTracingObject(std::string const& name);

    //--------------------------------------------------------------------
    // Free-Form Technique Methods
    //--------------------------------------------------------------------

    /**
     * Sets a technique attribute on the current RenderState.
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] res
     */
    void setTechniqueParametric(float res);

    /**
     * Sets a technique attribute on the current RenderState.
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] vec
     */
    void setTechniqueParametricA(OBJVector2 const& vec);

    /**
     * Sets a technique attribute on the current RenderState.
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] res
     */
    void setTechniqueParametricB(float res);

    /**
     * Sets a technique attribute on the current RenderState.
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] length
     */
    void setTechniqueSpatialCurve(float length);

    /**
     * Sets a technique attribute on the current RenderState.
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] length
     */
    void setTechniqueSpatialSurface(float length);

    /**
     * Sets a technique attribute on the current RenderState.
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] vec
     */
    void setTechniqueCurvatureCurve(OBJVector2 const& vec);

    /**
     * Sets a technique attribute on the current RenderState.
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] vec
     */
    void setTechniqueCurvatureSurface(OBJVector2 const& vec);

    //--------------------------------------------------------------------
    // Free-Form Attribute State Methods
    //--------------------------------------------------------------------

    /**
     * Sets an attribute on the current OBJFreeFormAttributeState
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] type
     */
    void setFreeFormType(OBJFreeFormType type);

    /**
     * Sets an attribute on the current OBJFreeFormAttributeState
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] rational
     */
    void setFreeFormRational(bool rational);

    /**
     * Sets an attribute on the current OBJFreeFormAttributeState
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] degree
     */
    void setFreeFormDegreeU(int32_t degree);

    /**
     * Sets an attribute on the current OBJFreeFormAttributeState
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] degree
     */
    void setFreeFormDegreeV(int32_t degree);

    /**
     * Sets an attribute on the current OBJFreeFormAttributeState
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] step
     */
    void setFreeFormStepU(int32_t step);

    /**
     * Sets an attribute on the current OBJFreeFormAttributeState
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] step
     */
    void setFreeFormStepV(int32_t step);

    /**
     * Sets an attribute on the current OBJFreeFormAttributeState
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] matrix
     */
    void setFreeFormBasisMatrixU(std::vector<float> const& matrix);

    /**
     * Sets an attribute on the current OBJFreeFormAttributeState
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] matrix
     */
    void setFreeFormBasisMatrixV(std::vector<float> const& matrix);

    /**
     * Sets an attribute on the current OBJFreeFormAttributeState
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] id
     */
    void setFreeFormMergeGroupNumber(int32_t id);

    /**
     * Sets an attribute on the current OBJFreeFormAttributeState
     * \note Typically should only be used by the OBJGrammar class.
     * \param[in] res
     */
    void setFreeFormMergeGroupResolution(float res);

protected:

    void resetAuxiliaryStates();
    void transformVertexGroup(OBJVertexGroup& source) const;
    
    //--------------------------------------------------------------------

    uint32_t m_GroupFacesReservedSize;
    uint32_t m_GroupFreeFormReservedSize;
    
    OBJFreeFormState m_FreeFormState;
    bool m_FreeFormRational;

    std::unordered_map<std::string, OBJGroup> m_GroupMap;
    std::unordered_map<std::string, OBJMaterial> m_MaterialMap;

    std::vector<OBJGroup*> m_ActiveGroups;

    std::vector<OBJVector4> m_VertexSpatialData;
    std::vector<OBJVector2> m_VertexTextureData;    
    std::vector<OBJVector3> m_VertexNormalData;  
    
    std::vector<std::string> m_MaterialLibraries;
    std::vector<std::string> m_TextureMapLibraries;

    std::vector<OBJRenderState> m_RenderStates;

private:
};

//------------------------------------------------------------------------------------------

#endif