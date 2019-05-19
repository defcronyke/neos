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

    class language_identifier : public neos::language::neos_concept
    {
    public:
        language_identifier() :
            neos_concept{ "language.identifier", neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    class language_function : public neos::language::neos_concept
    {
    public:
        language_function() :
            neos_concept{ "language.function", neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    class language_function_parameters : public neos::language::neos_concept
    {
    public:
        language_function_parameters() :
            neos_concept{ "language.function.parameters", neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    class language_function_parameter : public neos::language::neos_concept
    {
    public:
        language_function_parameter() :
            neos_concept{ "language.function.parameter", neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    class language_function_return : public neos::language::neos_concept
    {
    public:
        language_function_return() :
            neos_concept{ "language.function.return", neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    class language_function_signature : public neos::language::neos_concept
    {
    public:
        language_function_signature() :
            neos_concept{ "language.function.signature", neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    class language_function_import : public neos::language::neos_concept
    {
    public:
        language_function_import() :
            neos_concept{ "language.function.import", neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    class language_type : public neos::language::neos_concept
    {
    public:
        language_type() :
            neos_concept{ "language.type", neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    template <typename Float>
    class language_float_type : public neos::language::neos_concept
    {
    public:
        language_float_type(i_concept& aParent, const std::string& aName) :
            neos_concept{ aParent, aName, neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    template <typename Integer>
    class language_integer_type : public neos::language::neos_concept
    {
    public:
        language_integer_type(i_concept& aParent, const std::string& aName) :
            neos_concept{ aParent, aName, neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    template <typename Character>
    class language_string_type : public neos::language::neos_concept
    {
    public:
        language_string_type(i_concept& aParent, const std::string& aName) :
            neos_concept{ aParent, aName, neos::language::emit_type::Infix }
        {
        }
    public:
        source_iterator consume_token(neos::language::compiler_pass aPass, source_iterator aSource, source_iterator aSourceEnd, bool& aConsumed) const override
        {
            aConsumed = false;
            return aSource;
        }
    };

    class language_custom_type : public neos::language::neos_concept
    {
    public:
        language_custom_type(i_concept& aParent) :
            neos_concept{ aParent, "language.type.custom", neos::language::emit_type::Infix }
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
        concepts()[neolib::string{ "language.identifier" }] = neolib::make_ref<language_identifier>();
        concepts()[neolib::string{ "language.function" }] = neolib::make_ref<language_function>();
        concepts()[neolib::string{ "language.function.parameters" }] = neolib::make_ref<language_function_parameters>();
        concepts()[neolib::string{ "language.function.parameter" }] = neolib::make_ref<language_function_parameter>();
        concepts()[neolib::string{ "language.function.return" }] = neolib::make_ref<language_function_return>();
        concepts()[neolib::string{ "language.function.signature" }] = neolib::make_ref<language_function_signature>();
        concepts()[neolib::string{ "language.function.import" }] = neolib::make_ref<language_function_import>();
        concepts()[neolib::string{ "language.type" }] = neolib::make_ref<language_type>();
        concepts()[neolib::string{ "language.type.f32" }] = neolib::make_ref<language_float_type<float>>(*concepts()[neolib::string{ "language.type" }], "language.type.f32");
        concepts()[neolib::string{ "language.type.f64" }] = neolib::make_ref<language_float_type<double>>(*concepts()[neolib::string{ "language.type" }], "language.type.f64");
        concepts()[neolib::string{ "language.type.i8" }] = neolib::make_ref<language_integer_type<int8_t>>(*concepts()[neolib::string{ "language.type" }], "language.type.i8");
        concepts()[neolib::string{ "language.type.u8" }] = neolib::make_ref<language_integer_type<uint8_t>>(*concepts()[neolib::string{ "language.type" }], "language.type.i8");
        concepts()[neolib::string{ "language.type.i16" }] = neolib::make_ref<language_integer_type<int16_t>>(*concepts()[neolib::string{ "language.type" }], "language.type.i16");
        concepts()[neolib::string{ "language.type.u16" }] = neolib::make_ref<language_integer_type<uint16_t>>(*concepts()[neolib::string{ "language.type" }], "language.type.i16");
        concepts()[neolib::string{ "language.type.i32" }] = neolib::make_ref<language_integer_type<int32_t>>(*concepts()[neolib::string{ "language.type" }], "language.type.i32");
        concepts()[neolib::string{ "language.type.u32" }] = neolib::make_ref<language_integer_type<uint32_t>>(*concepts()[neolib::string{ "language.type" }], "language.type.i32");
        concepts()[neolib::string{ "language.type.i64" }] = neolib::make_ref<language_integer_type<int64_t>>(*concepts()[neolib::string{ "language.type" }], "language.type.i64");
        concepts()[neolib::string{ "language.type.u64" }] = neolib::make_ref<language_integer_type<uint64_t>>(*concepts()[neolib::string{ "language.type" }], "language.type.i64");
        concepts()[neolib::string{ "language.type.string" }] = neolib::make_ref<language_string_type<char>>(*concepts()[neolib::string{ "language.type" }], "language.type.string");
        concepts()[neolib::string{ "language.type.custom" }] = neolib::make_ref<language_custom_type>(*concepts()[neolib::string{ "language.type" }]);
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

