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

    parser.parseString(
        "# OBJ Parser Test\n"
        "mtllib testlib othertestlib\n"
        "maplib testMapLib othertestmaplib\n"
        "g default\n"
        "v 0.0 0.0 0.0\n"
        "v 1.0 0.0 0.0\n"
        "v 1.0 1.0 0.0\n"
        "v 0.0 1.0 0.0\n"
        "vt 0.0 1.0\n"
        "vn 1.0 1.0 1.0\n"
        "g pCube1 pSphere1\n"
        "f 1/2/3 2/3/4 3/4/5 4/5/6\n"
        "f 1/2/3 2/3/4 3/4/5\n"
        "usemtl testmtl\n"
        "f 1//3 2//4 3//5\n"
        "f 1/3 2/4 3/5\n"
        "f 1 2 3 4\n"
        "usemtl anothermtl\n"
        "f -1 -2 -3 -4\n"
        "g lineTest\n"
        "l 1/2 2/3 3/4 4/5 5/6\n"
        "l 1 2 3 4\n"
        "g pointTest\n"
        "p 1 2 3 4 5 6\n"
        "p 1 2 3 4 -1 -2 -3 -4\n"
        "bevel on\n"
        "bevel off\n"
        "c_interp on\n"
        "c_interp off\n"
        "d_interp on\n"
        "d_interp off\n"
        "lod 50\n"
        "lod 1000\n"
        "lod 0\n"
        "shadow_obj testshadowobj\n"
        "shadow_obj off\n"
        "trace_obj testtracerobj\n"
        "trace_obj off\n"
        "usemap texturemap\n"
        "usemap off\n"
        "s 5\n"
        "s off");

    return 0;
}