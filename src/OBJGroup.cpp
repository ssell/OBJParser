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

#include "OBJGroup.hpp"

//------------------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------------------

OBJGroup::OBJGroup()
{

}

OBJGroup::~OBJGroup()
{

}

//------------------------------------------------------------------------------------------
// Public Methods
//------------------------------------------------------------------------------------------

void OBJGroup::addLine(OBJLine const& line)
{
    lines.emplace_back(OBJLine());

    OBJLine* newLine = &lines[(lines.size() - 1)];
    newLine->segments.reserve(line.segments.size());
    newLine->segments.insert(newLine->segments.begin(), line.segments.begin(), line.segments.end());
}

void OBJGroup::addPointCollection(OBJPoint const& points)
{
    this->points.emplace_back(OBJPoint());

    OBJPoint* newPoints = &(this->points[(this->points.size() - 1)]);
    newPoints->points.reserve(points.points.size());
    newPoints->points.insert(newPoints->points.begin(), points.points.begin(), points.points.end());

    // using points as three different names was a great choice
}

//------------------------------------------------------------------------------------------
// Protected Methods
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// Private Methods
//------------------------------------------------------------------------------------------