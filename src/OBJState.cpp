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

#include "OBJState.hpp"

//------------------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------------------

OBJState::OBJState()
    : m_GroupFacesReservedSize(0),
      m_GroupFreeFormReservedSize(0),
      m_FreeFormRational(false)
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
    m_FreeFormState.vertexParameterData.clear();

    m_ActiveGroups.clear();
    m_GroupMap.clear();
    m_MaterialMap.clear();
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

OBJFreeFormState* OBJState::getFreeFormState()
{
    return &m_FreeFormState;
}

OBJRenderState OBJState::getRenderState(uint32_t index) const
{
    OBJRenderState result;

    if(index < m_RenderStates.size())
    {
        result = m_RenderStates[index];
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

std::vector<std::string> const* OBJState::getMaterialLibraries() const
{
    return &m_MaterialLibraries;
}

void OBJState::getMaterials(std::vector<OBJMaterial const*>& materials) const
{
    materials.clear();
    materials.reserve(m_MaterialMap.size());

    for(auto iter = m_MaterialMap.begin(); iter != m_MaterialMap.end(); ++iter)
    {
        materials.push_back(&(*iter).second);
    }
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

//------------------------------------------------------------------------------------------
// Vertex Data Methods
//------------------------------------------------------------------------------------------

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

void OBJState::addVertexParameter(OBJVector3 const& vector)
{
    m_FreeFormState.vertexParameterData.emplace_back(vector);
}

//------------------------------------------------------------------------------------------
// Geometry Creation Methods
//------------------------------------------------------------------------------------------

void OBJState::addFace(OBJFace face)
{
    transformVertexGroup(face.group0);
    transformVertexGroup(face.group1);
    transformVertexGroup(face.group2);
    transformVertexGroup(face.group3);

    face.renderState = static_cast<uint32_t>(m_RenderStates.size() - 1);

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

void OBJState::addFreeFormCurve(OBJCurve const& curve)
{
    std::vector<OBJVertexGroup> transformed;
    transformed.reserve(curve.controlPoints.size());
    transformed.insert(transformed.begin(), curve.controlPoints.begin(), curve.controlPoints.end());

    for(auto iter = transformed.begin(); iter != transformed.end(); ++iter)
    {
        transformVertexGroup((*iter));
    }

    const uint32_t state = static_cast<uint32_t>(m_FreeFormState.attributeStates.size());
    m_FreeFormState.addCurve(state, curve.startParam, curve.endParam, transformed);
}

void OBJState::addFreeFormCurve2D(std::vector<int32_t> const& points)
{
    std::vector<int32_t> transformed;
    transformed.reserve(points.size());
    transformed.insert(transformed.begin(), points.begin(), points.end());

    for(auto iter = transformed.begin(); iter != transformed.end(); ++iter)
    {
        if((*iter) < 0)
        {
            (*iter) += static_cast<int32_t>(m_FreeFormState.vertexParameterData.size()) + 1;  // +1 to maintain index 1-base
        }
    }

    const uint32_t state = static_cast<uint32_t>(m_FreeFormState.attributeStates.size());
    m_FreeFormState.addCurve2D(state, transformed);
}

void OBJState::addFreeFormSurface(OBJSurface const& surface)
{
    std::vector<OBJVertexGroup> transformed;
    transformed.reserve(surface.controlPoints.size());
    transformed.insert(transformed.begin(), surface.controlPoints.begin(), surface.controlPoints.end());

    for(auto iter = transformed.begin(); iter != transformed.end(); ++iter)
    {
        transformVertexGroup((*iter));
    }

    const uint32_t state = static_cast<uint32_t>(m_FreeFormState.attributeStates.size());
    m_FreeFormState.addSurface(state, surface.startParamU, surface.endParamU, surface.startParamV, surface.endParamV, transformed);
}

void OBJState::addFreeFormConnection(OBJSurfaceConnection connection)
{
    //--------------------------------------------------------------------
    // Transform indicies to positive 0-base
    //--------------------------------------------------------------------

    if(connection.surfaceIndex0 < 0)
    {
        connection.surfaceIndex0 += static_cast<uint32_t>(m_FreeFormState.surfaces.size());
    }
    else
    {
        connection.surfaceIndex0--;
    }

    if(connection.curve2DIndex0 < 0)
    {
        connection.curve2DIndex0 += static_cast<uint32_t>(m_FreeFormState.curves2D.size());
    }
    else
    {
        connection.curve2DIndex0--;
    }

    if(connection.surfaceIndex1 < 0)
    {
        connection.surfaceIndex1 += static_cast<uint32_t>(m_FreeFormState.surfaces.size());
    }
    else
    {
        connection.surfaceIndex1--;
    }

    if(connection.curve2DIndex1 < 0)
    {
        connection.curve2DIndex1 += static_cast<uint32_t>(m_FreeFormState.curves2D.size());
    }
    else
    {
        connection.curve2DIndex1--;
    }

    //--------------------------------------------------------------------

    m_FreeFormState.connections.push_back(connection);
}

//------------------------------------------------------------------------------------------
// Free-Form Body Methods
//------------------------------------------------------------------------------------------

void OBJState::addFreeFormParameterU(std::vector<float> const& parameters)
{
    m_FreeFormState.addParameterU(parameters);
}

void OBJState::addFreeFormParameterV(std::vector<float> const& parameters)
{
    m_FreeFormState.addParameterV(parameters);
}
void OBJState::addFreeFormTrim(OBJSimpleCurve const& trim)
{
    m_FreeFormState.addTrim(trim);
}

void OBJState::addFreeFormHole(OBJSimpleCurve const& hole)
{
    m_FreeFormState.addHole(hole);
}

void OBJState::addFreeFormSpecialCurve(OBJSimpleCurve const& scurve)
{
    m_FreeFormState.addSpecialCurve(scurve);
}

void OBJState::addFreeFormSpecialPoints(std::vector<int32_t> const& points)
{
    m_FreeFormState.addSpecialPoints(points);
}

//------------------------------------------------------------------------------------------
// Render State Setting Methods
//------------------------------------------------------------------------------------------

void OBJState::setLevelOfDetail(uint32_t const lod)
{
    OBJRenderState renderState = m_RenderStates.back();
    renderState.lod = lod;

    if(renderState.lod > 100)
    {
        renderState.lod = 100;    // "Specifying an integer between 1 and 100 sets the..."
    }

    m_RenderStates.push_back(renderState);
}

void OBJState::setSmoothingGroup(uint32_t const group)
{
    OBJRenderState renderState = m_RenderStates.back();
    renderState.smoothing = group;

    m_RenderStates.push_back(renderState);
}

void OBJState::setBevelInterp(bool const on)
{
    OBJRenderState renderState = m_RenderStates.back();
    renderState.bevelInterp = on;

    m_RenderStates.push_back(renderState);
}

void OBJState::setColorInterp(bool const on)
{
    OBJRenderState renderState = m_RenderStates.back();
    renderState.colorInterp = on;

    m_RenderStates.push_back(renderState);
}

void OBJState::setDissolveInterp(bool const on)
{
    OBJRenderState renderState = m_RenderStates.back();
    renderState.dissolveInterp = on;

    m_RenderStates.push_back(renderState);
}

void OBJState::setMaterial(std::string const& name)
{
    OBJRenderState renderState = m_RenderStates.back();
    renderState.material = name;

    m_RenderStates.push_back(renderState);
}

void OBJState::setMaterial(std::string const& name, OBJMaterial const& material)
{
    if(!name.empty())
    {
        m_MaterialMap[name] = material;
    }
}

void OBJState::addMaterialLibrary(std::string const& path)
{
    m_MaterialLibraries.push_back(path);
}

void OBJState::setTextureMap(std::string const& name)
{
    OBJRenderState renderState = m_RenderStates.back();
    renderState.textureMap = name;

    m_RenderStates.push_back(renderState);
}

void OBJState::addTextureMapLibrary(std::string const& path)
{
    m_TextureMapLibraries.push_back(path);
}

void OBJState::setShadowObject(std::string const& name)
{
    OBJRenderState renderState = m_RenderStates.back();
    renderState.shadowObj = name;

    if(renderState.shadowObj.compare("off") == 0)
    {
        renderState.shadowObj = "";
    }

    m_RenderStates.push_back(renderState);
}

void OBJState::setTracingObject(std::string const& name)
{
    OBJRenderState renderState = m_RenderStates.back();
    renderState.traceObj = name;

    if(renderState.traceObj.compare("off") == 0)
    {
        renderState.traceObj = "";
    }

    m_RenderStates.push_back(renderState);
}

//------------------------------------------------------------------------------------------
// Free-Form Technique Methods
//------------------------------------------------------------------------------------------

void OBJState::setTechniqueParametric(float const res)
{
    OBJRenderState renderState = m_RenderStates.back();

    renderState.curveTechnique.technique = OBJSubdivision::Parametric;
    renderState.curveTechnique.res = res;
    
    renderState.surfaceTechnique.technique = OBJSubdivision::None;        // Disable surface technique

    m_RenderStates.push_back(renderState);
}

void OBJState::setTechniqueParametricA(OBJVector2 const& vec)
{
    OBJRenderState renderState = m_RenderStates.back();

    renderState.surfaceTechnique.technique = OBJSubdivision::ParametricA;
    renderState.surfaceTechnique.resU = vec.x;
    renderState.surfaceTechnique.resV = vec.y;
    
    renderState.curveTechnique.technique = OBJSubdivision::None;          // Disable curve technique

    m_RenderStates.push_back(renderState);
}

void OBJState::setTechniqueParametricB(float const res)
{
    OBJRenderState renderState = m_RenderStates.back();

    renderState.surfaceTechnique.technique = OBJSubdivision::ParametricB;
    renderState.surfaceTechnique.resU = res;
    renderState.surfaceTechnique.resV = res;
    
    renderState.curveTechnique.technique = OBJSubdivision::None;          // Disable curve technique

    m_RenderStates.push_back(renderState);
}

void OBJState::setTechniqueSpatialCurve(float const length)
{
    OBJRenderState renderState = m_RenderStates.back();

    renderState.curveTechnique.technique = OBJSubdivision::Spatial;
    renderState.curveTechnique.maxLength = length;
    
    renderState.surfaceTechnique.technique = OBJSubdivision::None;        // Disable surface technique

    m_RenderStates.push_back(renderState);
}

void OBJState::setTechniqueSpatialSurface(float const length)
{
    OBJRenderState renderState = m_RenderStates.back();

    renderState.surfaceTechnique.technique = OBJSubdivision::Spatial;
    renderState.surfaceTechnique.maxLength = length;
    
    renderState.curveTechnique.technique = OBJSubdivision::None;          // Disable curve technique

    m_RenderStates.push_back(renderState);
}

void OBJState::setTechniqueCurvatureCurve(OBJVector2 const& vec)
{
    OBJRenderState renderState = m_RenderStates.back();

    renderState.curveTechnique.technique = OBJSubdivision::Curvature;
    renderState.curveTechnique.maxDistance = vec.x;
    renderState.curveTechnique.maxAngle = vec.y;
    
    renderState.surfaceTechnique.technique = OBJSubdivision::None;        // Disable surface technique

    m_RenderStates.push_back(renderState);
}

void OBJState::setTechniqueCurvatureSurface(OBJVector2 const& vec)
{
    OBJRenderState renderState = m_RenderStates.back();

    renderState.surfaceTechnique.technique = OBJSubdivision::Curvature;
    renderState.surfaceTechnique.maxDistance = vec.x;
    renderState.surfaceTechnique.maxAngle = vec.y;
    
    renderState.curveTechnique.technique = OBJSubdivision::None;          // Disable curve technique

    m_RenderStates.push_back(renderState);
}

//------------------------------------------------------------------------------------------
// Free-Form Attribute State Methods
//------------------------------------------------------------------------------------------

void OBJState::setFreeFormType(OBJFreeFormType const type)
{
    OBJFreeFormAttributeState state;
    state.type = type;
    state.rational = m_FreeFormRational;

    m_FreeFormState.attributeStates.push_back(state);

    m_FreeFormRational = false;
}

void OBJState::setFreeFormRational(bool rational)
{
    m_FreeFormRational = rational;
}

void OBJState::setFreeFormDegreeU(int32_t const degree)
{
    m_FreeFormState.attributeStates.back().degreeU = degree;
}

void OBJState::setFreeFormDegreeV(int32_t const degree)
{
    m_FreeFormState.attributeStates.back().degreeV = degree;
}

void OBJState::setFreeFormStepU(int32_t const step)
{
    m_FreeFormState.attributeStates.back().stepU = step;
}

void OBJState::setFreeFormStepV(int32_t const step)
{
    m_FreeFormState.attributeStates.back().stepV = step;
}

void OBJState::setFreeFormBasisMatrixU(std::vector<float> const& matrix)
{
    OBJFreeFormAttributeState* state = &m_FreeFormState.attributeStates.back();

    state->basisMatrixU.reserve(state->basisMatrixU.size() + matrix.size());
    state->basisMatrixU.insert(state->basisMatrixU.end(), matrix.begin(), matrix.end());
}

void OBJState::setFreeFormBasisMatrixV(std::vector<float> const& matrix)
{
    OBJFreeFormAttributeState* state = &m_FreeFormState.attributeStates.back();

    state->basisMatrixV.reserve(state->basisMatrixV.size() + matrix.size());
    state->basisMatrixV.insert(state->basisMatrixV.end(), matrix.begin(), matrix.end());
}

void OBJState::setFreeFormMergeGroupNumber(int32_t const id)
{
    m_FreeFormState.attributeStates.back().mergeGroupNumber = id;
}

void OBJState::setFreeFormMergeGroupResolution(float const res)
{
    m_FreeFormState.attributeStates.back().mergeGroupResolution = res;
}

//------------------------------------------------------------------------------------------
// Protected Methods
//------------------------------------------------------------------------------------------

void OBJState::resetAuxiliaryStates() 
{
    m_RenderStates.clear();
    m_RenderStates.reserve(50);                  // Arbitrary reserve
    m_RenderStates.push_back(OBJRenderState());  // Set initial default state

    m_FreeFormState.attributeStates.clear();
    m_RenderStates.reserve(50);
    m_FreeFormState.attributeStates.push_back(OBJFreeFormAttributeState());
}

void OBJState::transformVertexGroup(OBJVertexGroup& source) const
{
    // Incoming indices may be negative.
    // We want to transform them so they are positive.

    // They are also 1-based (no incoming index is 0), but 
    // all standard containers are 0-based, so we also 
    // transform them to be 0-base.

    if(source.indexSpatial < 0)
    {
        source.indexSpatial += static_cast<int32_t>(m_VertexSpatialData.size());
    }
    else
    {
        source.indexSpatial--;
    }

    if(source.indexTexture < 0)
    {
        source.indexTexture += static_cast<int32_t>(m_VertexTextureData.size());
    }
    else
    {
        source.indexTexture--;
    }

    if(source.indexNormal < 0)
    {
        source.indexNormal += static_cast<int32_t>(m_VertexNormalData.size());
    }
    else
    {
        source.indexNormal--;
    }
}

//------------------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------------------