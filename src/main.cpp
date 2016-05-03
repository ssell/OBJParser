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

int main(int argc, char** argv)
{
    OBJParser parser;
    OBJState* state = parser.getOBJState();

    //--------------------------------------------------------------------

    const std::string path = "test/testcube.obj";

    auto start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    const bool result = parser.parseOBJFile(path);
    auto end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    const auto elapsed = end - start;
    const double elapsedf = static_cast<double>(end - start) / static_cast<double>(1e9);

    std::cout << "Finished parsing '" << path << "' in " << elapsedf << " seconds. [" << (result ? "Success" : "Failed") << "][" << elapsed << "]" << std::endl;

    std::cout << "# S: " << state->getSpatialData()->size() << "\n"
              << "# T: " << state->getTextureData()->size() << "\n"
              << "# N: " << state->getNormalData()->size() << std::endl;

    return 0;
}