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

#ifndef __H__OBJ_PARSER_GRAMMAR_SKIPPER__H__
#define __H__OBJ_PARSER_GRAMMAR_SKIPPER__H__

#pragma warning (disable:4348)

#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit;

//------------------------------------------------------------------------------------------

/**
 * \class OBJCommentSkipper
 * 
 * Grammar defining how to skip OBJ comments.
 * A comment is started with the '#' character and continues until the end of the line.
 */
template<typename Iterator>
class OBJCommentSkipper : public boost::spirit::qi::grammar<Iterator>
{
public:

    OBJCommentSkipper()
        : OBJCommentSkipper::base_type(ruleSkip)
    {
        ruleSkip = (qi::char_('#') >> *(qi::char_ - qi::eol) >> qi::eol);
    }

    boost::spirit::qi::rule<Iterator> ruleSkip;

protected:

private:
};

//------------------------------------------------------------------------------------------

#endif
