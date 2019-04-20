/*
  language.cpp

  Copyright (c) 2019 Leigh Johnston.  All Rights Reserved.

  This program is free software: you can redistribute it and / or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <neos/language/concept.hpp>
#include "language.hpp"

namespace neos::concept::core
{   
    class language_whitespace : public neos::language::neos_concept
    {
    public:
        language_whitespace() :
            neos_concept{ "language.whitespace", neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    class language_comment : public neos::language::neos_concept
    {
    public:
        language_comment() :
            neos_concept{ "language.comment", neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    class language_keyword : public neos::language::neos_concept
    {
    public:
        language_keyword() :
            neos_concept{ "language.keyword", neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    language::language(neos::language::i_concept_library& aParent) :
        neos::language::concept_library
        { 
            aParent,
            library_id(), 
            aParent.uri().to_std_string(),
            library_name(), 
            "Core language concepts", 
            neolib::version{ 1, 0, 0 }, 
            "Copyright (c) 2019 Leigh Johnston.  All Rights Reserved."
        }
    {
        /* todo */
        concepts()[neolib::string{ "language.statement" }] = neolib::make_ref<neos::language::unimplemented_concept>("language.statement");
        concepts()[neolib::string{ "language.keyword" }] = neolib::make_ref<language_keyword>();
        concepts()[neolib::string{ "language.whitespace" }] = neolib::make_ref<language_whitespace>();
        concepts()[neolib::string{ "language.comment" }] = neolib::make_ref<language_comment>();
    }

    const std::string& language::library_name()
    {
        static const std::string sName = "neos.language";
        return sName;
    }

    const neolib::uuid& language::library_id()
    {
        static const neolib::uuid sId = neolib::make_uuid("C33C5C53-937B-434B-A3F9-C2A1BB71D391");
        return sId;
    }
}

