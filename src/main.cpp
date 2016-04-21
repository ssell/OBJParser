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

#include <iostream>
#include <string>

#include "OBJParser.hpp"

//------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    OBJParser parser;
    OBJState* state = parser.getOBJState();

    parser.parseString("# OBJ Parser Test\n"
        "g default\n"
        "v 0.0 0.0 0.0\n"
        "v 1.0 0.0 0.0\n"
        "v 1.0 1.0 0.0\n"
        "v 0.0 1.0 0.0\n"
        "vt 0.0 1.0\n"
        "vn 1.0 1.0 1.0\n"
        "g pCube1 pSphere1\n"
        "f 1/1/1 2/1/1 3/1/1 4/1/1\n");

    return 0;
}