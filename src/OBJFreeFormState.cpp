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

#include "OBJFreeFormState.hpp"

//------------------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------------------

OBJFreeFormState::OBJFreeFormState()
    : m_LatestFreeForm(FreeFormType::None)
{

}

OBJFreeFormState::~OBJFreeFormState()
{

}

//------------------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------------------

void OBJFreeFormState::addCurve(uint32_t const state, float const startParam, float const endParam, std::vector<OBJVertexGroup> const& controlPoints)
{
    m_LatestFreeForm = FreeFormType::Curve;
    curves.emplace_back(startParam, endParam, controlPoints);
    curves.back().attributeState = state;
}

void OBJFreeFormState::addCurve2D(uint32_t const state, std::vector<int32_t> const& controlPoints)
{
    m_LatestFreeForm = FreeFormType::Curve2D;
    curves2D.emplace_back(controlPoints);
    curves2D.back().attributeState = state;
}

void OBJFreeFormState::addSurface(uint32_t const state, float const startU, float const endU, float const startV, float const endV, std::vector<OBJVertexGroup> const& controlPoints)
{
    m_LatestFreeForm = FreeFormType::Surface;
    surfaces.emplace_back(startU, endU, startV, endV, controlPoints);
    surfaces.back().attributeState = state;
}

void OBJFreeFormState::addParameterU(std::vector<float> const& parameters)
{
    OBJFreeForm* freeform = getLatestFreeForm();

    if(freeform)
    {
        freeform->parametersU.reserve(freeform->parametersU.size() + parameters.size());
        freeform->parametersU.insert(freeform->parametersU.end(), parameters.begin(), parameters.end());
    }
}

void OBJFreeFormState::addParameterV(std::vector<float> const& parameters)
{
    OBJFreeForm* freeform = getLatestFreeForm();

    if(freeform)
    {
        freeform->parametersV.reserve(freeform->parametersV.size() + parameters.size());
        freeform->parametersV.insert(freeform->parametersV.end(), parameters.begin(), parameters.end());
    }
}

void OBJFreeFormState::addTrim(OBJSimpleCurve const& trim)
{
    OBJFreeForm* freeform = getLatestFreeForm();

    if(freeform)
    {
        freeform->trims.push_back(trim);
    }
}

void OBJFreeFormState::addHole(OBJSimpleCurve const& hole)
{
    OBJFreeForm* freeform = getLatestFreeForm();

    if(freeform)
    {
        freeform->holes.push_back(hole);
    }
}

void OBJFreeFormState::addSpecialCurve(OBJSimpleCurve const& scurve)
{
    OBJFreeForm* freeform = getLatestFreeForm();

    if(freeform)
    {
        freeform->specialCurves.push_back(scurve);
    }
}

void OBJFreeFormState::addSpecialPoints(std::vector<int32_t> const& points)
{
    OBJFreeForm* freeform = getLatestFreeForm();

    if(freeform)
    {
        freeform->specialPoints.reserve(freeform->specialPoints.size() + points.size());
        freeform->specialPoints.insert(freeform->specialPoints.end(), points.begin(), points.end());
    }
}

//------------------------------------------------------------------------------------------
// Protected Methods
//------------------------------------------------------------------------------------------

OBJFreeForm* OBJFreeFormState::getLatestFreeForm()
{
    OBJFreeForm* result = nullptr;

    switch(m_LatestFreeForm)
    {
    case FreeFormType::Curve:
        if(curves.size())
        {
            result = &(curves.back());
        }
        break;

    case FreeFormType::Curve2D:
        if(curves2D.size())
        {
            result = &(curves2D.back());
        }
        break;

    case FreeFormType::Surface:
        if(surfaces.size())
        {
            result = &(surfaces.back());
        }
        break;

    default:
        break;
    }

    return result;
}

//------------------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------------------