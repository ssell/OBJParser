/**
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

#ifndef __H__OBJ_PARSER_MTL_GRAMMAR_SKIPPER__H__
#define __H__OBJ_PARSER_MTL_GRAMMAR_SKIPPER__H__

#include "OBJCommon.hpp"

//------------------------------------------------------------------------------------------

/**
 * \class MTLGrammarSkipper
 * 
 * Grammar defining how to skip MTL comments and whitespace.
 * A comment is started with the '#' character and continues until the end of the line.
 */
class MTLGrammarSkipper : public boost::spirit::qi::grammar<OBJIterator>
{
public:

    MTLGrammarSkipper();

protected:

private:

    boost::spirit::qi::rule<OBJIterator> ruleSkip;
};

//------------------------------------------------------------------------------------------

#endif
