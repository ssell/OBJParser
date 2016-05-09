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

#ifndef __H__OBJ_PARSER_FREE_FORM_STATE__H__
#define __H__OBJ_PARSER_FREE_FORM_STATE__H__

#include "OBJStructs.hpp"
#include "OBJFreeFormAttributeState.hpp"

//------------------------------------------------------------------------------------------

/**
 * \class OBJFreeFormState
 */
class OBJFreeFormState
{
    enum class FreeFormType
    {
        None = 0,
        Curve,
        Curve2D,
        Surface
    };

public:

    OBJFreeFormState();
    ~OBJFreeFormState();

    void addCurve(uint32_t state, float startParam, float endParam, std::vector<OBJVertexGroup> const& controlPoints);
    void addCurve2D(uint32_t state, std::vector<int32_t> const& controlPoints);
    void addSurface(uint32_t state, float startU, float endU, float startV, float endV, std::vector<OBJVertexGroup> const& controlPoints);

    void addParameterU(std::vector<float> const& parameters);
    void addParameterV(std::vector<float> const& parameters);
    void addTrim(OBJSimpleCurve const& trim);
    void addHole(OBJSimpleCurve const& hole);
    void addSpecialCurve(OBJSimpleCurve const& scurve);
    void addSpecialPoints(std::vector<int32_t> const& points);

    //--------------------------------------------------------------------
    
    std::vector<OBJFreeFormAttributeState> attributeStates;     ///< Collection of all attribute states. Each state represents a 'ctype' statement and the following state settings.

    std::vector<OBJVector3> vertexParameterData;                ///< Collection of all data specified by 'vp' statements. These are referenced by OBJCurve2D::parameterVertexIndices.

    std::vector<OBJCurve> curves;                               ///< Collection of all Curves specified by 'curv' statements.
    std::vector<OBJCurve2D> curves2D;                           ///< Collection of all Curve2Ds specified by 'curv2' statements.
    std::vector<OBJSurface> surfaces;                           ///< Collection of all Surfaces specified by 'surf' statements.

    std::vector<OBJSurfaceConnection> connections;              ///< Collection of all Surface connections specified by 'con' statements.

protected:

    OBJFreeForm* getLatestFreeForm();

    //--------------------------------------------------------------------

    FreeFormType m_LatestFreeForm;

private:
};

//------------------------------------------------------------------------------------------

#endif

