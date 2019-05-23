/*
  string.cpp

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
#include "string.hpp"

namespace neos::concept::core
{   
    template <typename T>
    class string_utf8_character;

    struct any_char {};

    template <>
    class string_utf8_character<any_char> : public neos::language::neos_concept
    {
    public:
        using neos_concept::neos_concept;
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = true;
            return std::next(aSource);
        }
    };

    template <char Char>
    struct single_char {};

    template <char Char>
    class string_utf8_character<single_char<Char>> : public neos::language::neos_concept
    {
    public:
        using neos_concept::neos_concept;
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            if (*aSource == Char)
            {
                aConsumed = true;
                return std::next(aSource);
            }
            aConsumed = false;
            return aSource;
        }
    };

    template <std::size_t N>
    struct multiple_chars : std::array<char, N> {};

    template <std::size_t N>
    class string_utf8_character<multiple_chars<N>> : public neos::language::neos_concept
    {
    public:
        string_utf8_character(i_concept& aParent, const std::string& aName, const multiple_chars<N>& aChars) :
            neos_concept{ aParent, aName }, iChars{ aChars }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            if (std::find(iChars.begin(), iChars.end(), *aSource) != iChars.end())
            {
                aConsumed = true;
                return std::next(aSource);
            }
            aConsumed = false;
            return aSource;
        }
    private:
        const multiple_chars<N> iChars;
    };

    class string_utf8_character_alpha : public neos::language::neos_concept
    {
    public:
        string_utf8_character_alpha(i_concept& aParent) :
            neos_concept{ aParent, "string.utf8.character.alpha" }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            auto const ch = *aSource;
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= 0x80))
            {
                aConsumed = true;
                return std::next(aSource);
            }
            aConsumed = false;
            return aSource;
        }
    };

    string::string(neos::language::i_concept_library& aParent) :
        neos::language::concept_library
        { 
            aParent,
            library_id(), 
            aParent.uri().to_std_string(),
            library_name(), 
            "Core string concepts", 
            neolib::version{ 1, 0, 0 }, 
            "Copyright (c) 2019 Leigh Johnston.  All Rights Reserved."
        }
    {
        /* todo */
        concepts()[neolib::string{ "string" }] = neolib::make_ref<neos::language::unimplemented_concept>("string");
        concepts()[neolib::string{ "string.character" }] = neolib::make_ref<neos::language::unimplemented_concept>("string.character");
        concepts()[neolib::string{ "string.utf8" }] = neolib::make_ref<neos::language::unimplemented_concept>(*concepts()[neolib::string{ "string" }], "string.utf8");
        concepts()[neolib::string{ "string.utf8.character" }] = neolib::make_ref<string_utf8_character<any_char>>(*concepts()[neolib::string{ "string.character" }], "string.utf8.character");
        concepts()[neolib::string{ "string.utf8.character.LF" }] = neolib::make_ref<string_utf8_character<single_char<'\n'>>>(*concepts()[neolib::string{ "string.utf8.character" }], "string.utf8.character.LF");
        concepts()[neolib::string{ "string.utf8.character.CR" }] = neolib::make_ref<string_utf8_character<single_char<'\r'>>>(*concepts()[neolib::string{ "string.utf8.character" }], "string.utf8.character.CR");
        concepts()[neolib::string{ "string.utf8.character.tab" }] = neolib::make_ref<string_utf8_character<single_char<'\t'>>>(*concepts()[neolib::string{ "string.utf8.character" }], "string.utf8.character.tab");
        concepts()[neolib::string{ "string.utf8.character.backslash" }] = neolib::make_ref<string_utf8_character<single_char<'\\'>>>(*concepts()[neolib::string{ "string.utf8.character" }], "string.utf8.character.backslash");
        concepts()[neolib::string{ "string.utf8.character.alpha" }] = neolib::make_ref<string_utf8_character_alpha>(*concepts()[neolib::string{ "string.utf8.character" }]);
        concepts()[neolib::string{ "string.utf8.character.digit" }] = neolib::make_ref<string_utf8_character<multiple_chars<10>>>(*concepts()[neolib::string{ "string.utf8.character" }], "string.utf8.character.digit",
            multiple_chars<10>{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' });
        concepts()[neolib::string{ "string.utf8.character.hexdigit" }] = neolib::make_ref<string_utf8_character<multiple_chars<22>>>(*concepts()[neolib::string{ "string.utf8.character" }], "string.utf8.character.hexdigit",
            multiple_chars<22>{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F' });
        concepts()[neolib::string{ "string.utf8.character.period" }] = neolib::make_ref<string_utf8_character<single_char<'.'>>>(*concepts()[neolib::string{ "string.utf8.character" }], "string.utf8.character.period");
        concepts()[neolib::string{ "string.utf8.character.comma" }] = neolib::make_ref<string_utf8_character<single_char<','>>>(*concepts()[neolib::string{ "string.utf8.character" }], "string.utf8.character.comma");
        concepts()[neolib::string{ "string.utf8.character.underscore" }] = neolib::make_ref<string_utf8_character<single_char<'_'>>>(*concepts()[neolib::string{ "string.utf8.character" }], "string.utf8.character.underscore");
    }

    const std::string& string::library_name()
    {
        static const std::string sName = "neos.string";
        return sName;
    }

    const neolib::uuid& string::library_id()
    {
        static const neolib::uuid sId = neolib::make_uuid("C281D6BD-CBC7-40F4-8E1A-BE4FC38B8F83");
        return sId;
    }
}

