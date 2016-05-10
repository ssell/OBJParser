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
#include <chrono>

#include "OBJParser.hpp"

//------------------------------------------------------------------------------------------

void OutputState(OBJState* state)
{
    std::cout << "++++++++++++++++++++++++++++++++++++" << std::endl;
    
    //--------------------------------------------------------------------
    // Print out vertex counts
    //--------------------------------------------------------------------

    std::cout << "- Vertex Data" << "\n"
              << "    Spatial Count: " << state->getSpatialData()->size() << "\n"
              << "    Texture Count: " << state->getTextureData()->size() << "\n"
              << "    Normals Count: " << state->getNormalData()->size()  << "\n"
              << "     Params Count: " << state->getFreeFormState()->vertexParameterData.size() << std::endl;
    
    //--------------------------------------------------------------------
    // Print out basic group information
    //--------------------------------------------------------------------

    std::vector<OBJGroup const*> groups;
    state->getGroups(groups);

    std::cout << "- Group Data [" << groups.size() << "]" << std::endl;

    for(auto group : groups)
    {
        std::cout << "    Group:" << "\n"
                  << "             Name: " << group->name << "\n"
                  << "         # Points: " << group->points.size() << "\n"
                  << "          # Lines: " << group->lines.size() << "\n"
                  << "          # Faces: " << group->faces.size() << std::endl;
    }
    
    //--------------------------------------------------------------------
    // Print out basic material information
    //--------------------------------------------------------------------

    std::vector<OBJMaterial const*> materials;
    state->getMaterials(materials);

    std::cout << "- Material Data [" << materials.size() << "]" << std::endl;

    for(auto material : materials)
    {
        std::cout << "    Material:" << "\n"
                  << "             Name: " << material->getName() << std::endl;
    }

    //--------------------------------------------------------------------

    std::cout << "++++++++++++++++++++++++++++++++++++" << std::endl;
}

void ParsePath(std::string const& path, OBJParser& parser)
{
    OBJState* state = parser.getOBJState();

    const auto start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    const auto result = parser.parseOBJFile(path);
    const auto end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    if(result != OBJParser::Result::Success)
    {
        std::cout << "... Failed!\n\nError: " <<  parser.getLastError() << std::endl;
    }
    else
    {
        const auto elapsed = end - start;
        const double elapsedd = static_cast<double>(end - start) / static_cast<double>(1e9);

        std::cout << "... Complete! [elapsed " << elapsedd << " s]\n" << std::endl;

        OutputState(state);
    }
}

void Loop()
{
    OBJParser parser;

    std::cout << "------------------------------------------------------\n"
                 "- OBJParser Sample Application\n"
                 "------------------------------------------------------" << std::endl;

    while(true)
    {
        std::string path;

        std::cout << "\nPlease enter relative path to OBJ file to parse:\n\n> ";
        std::getline(std::cin, path);

        std::cout << "\nParsing ...\n";

        ParsePath(path, parser);
    }
}

//------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    Loop();

    return 0;
}