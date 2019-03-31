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
    template <typename T>
    class language_character_utf8;

    template <char Char>
    struct single_char {};

    template <char Char>
    class language_character_utf8<single_char<Char>> : public neos::language::neos_concept
    {
    public:
        language_character_utf8(const std::string& aName) :
            neos_concept{ aName }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd) const override
        {
            if (*aSource == Char)
                return std::next(aSource);
            return aSource;
        }
    };

    template <std::size_t N>
    struct multiple_chars : std::array<char, N> {};

    template <std::size_t N>
    class language_character_utf8<multiple_chars<N>> : public neos::language::neos_concept
    {
    public:
        language_character_utf8(const std::string& aName, const multiple_chars<N>& aChars) :
            neos_concept{ aName }, iChars{ aChars }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd) const override
        {
            if (std::find(iChars.begin(), iChars.end(), *aSource) != iChars.end())
                return std::next(aSource);
            return aSource;
        }
    private:
        const multiple_chars<N> iChars;
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
        concepts()[neolib::string{ "language.whitespace" }] = neolib::make_ref<neos::language::unimplemented_concept>("language.whitespace");
        concepts()[neolib::string{ "language.character" }] = neolib::make_ref<neos::language::unimplemented_concept>("language.character");
        /* todo: make next concepts child of above concept */
        concepts()[neolib::string{ "language.character.utf8" }] = neolib::make_ref<neos::language::unimplemented_concept>("language.character.utf8");
        /* todo: make next concepts children of above concept */
        concepts()[neolib::string{ "language.character.utf8.LF" }] = neolib::make_ref<language_character_utf8<single_char<'\n'>>>("language.character.utf8.LF");
        concepts()[neolib::string{ "language.character.utf8.CR" }] = neolib::make_ref<language_character_utf8<single_char<'\r'>>>("language.character.utf8.CR");
        concepts()[neolib::string{ "language.character.utf8.TAB" }] = neolib::make_ref<language_character_utf8<single_char<'\t'>>>("language.character.utf8.TAB");
        concepts()[neolib::string{ "language.character.utf8.digit" }] = neolib::make_ref<language_character_utf8<multiple_chars<10>>>("language.character.utf8.digit", 
            multiple_chars<10>{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' });
        concepts()[neolib::string{ "language.character.utf8.hexdigit" }] = neolib::make_ref<language_character_utf8<multiple_chars<22>>>("language.character.utf8.hexdigit",
            multiple_chars<22>{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F' });
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

