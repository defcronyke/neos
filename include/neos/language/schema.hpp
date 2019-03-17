/*
  schema.hpp

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

#pragma once

#include <neos/neos.hpp>
#include <boost/functional/hash.hpp>
#include <neolib/i_map.hpp>
#include <neolib/i_string.hpp>
#include <neolib/json.hpp>
#include <neos/language/i_concept_library.hpp>
#include <neos/language/schema_atom.hpp>

namespace neos
{
    namespace language
    {
        struct meta
        {
            std::string name;
            std::string description;
            std::string copyright;
            std::string version;
            std::vector<std::string> sourcecodeFileExtension;
            std::vector<std::string> sourcecodeModulePackageSpecificationFileExtension;
            std::vector<std::string> sourcecodeModulePackageImplementationFileExtension;
        };

        enum class schema_keyword
        {
            Invalid,
            Meta,
            Libraries,
            Expect,
            Tokens
        };

        class schema
        {
        public:
            struct unresolved_references : std::runtime_error 
            { 
                std::vector<std::pair<neolib::rjson_string, neolib::rjson_string>> references;
                unresolved_references(std::vector<std::pair<neolib::rjson_string, neolib::rjson_string>>&& aReferences) :
                    std::runtime_error("neos::language::schema::unresolved_references"),
                    references{ std::move(aReferences) }
                {} 
            };
        public:
            typedef neolib::ref_ptr<i_schema_atom> atom_ptr;
            typedef neolib::i_ref_ptr<i_schema_atom>* atom_reference_t;
            typedef std::pair<neolib::rjson_string, neolib::rjson_string> atom_reference_key_t;
            typedef std::unordered_map<atom_reference_key_t, std::vector<atom_reference_t>, boost::hash<atom_reference_key_t>> atom_references_t;
        public:
            schema(neolib::rjson const& aSchema, const concept_libraries_t& aConceptLibraries);
        public:
            language::meta const& meta() const;
        private:
            static schema_keyword keyword(const neolib::rjson_string& aSymbol);
            void parse(neolib::rjson_value const& aNode, atom_ptr aAtom);
            void parse_meta(neolib::rjson_value const& aNode);
            void parse_tokens(neolib::rjson_value const& aNode, atom_ptr aAtom);
            std::string fully_qualified_name(neolib::rjson_value const& aNode, const std::string& aRhs = std::string{}) const;
            const atom_references_t& atom_references() const;
            void add_lhs_atom_reference(neolib::rjson_value const& aNode, atom_ptr aParentAtom, std::remove_pointer<atom_reference_t>::type& aAtom);
            void add_rhs_atom_reference(neolib::rjson_value const& aNode, atom_ptr aParentAtom, std::remove_pointer<atom_reference_t>::type& aAtom);
            void resolve_references();
        private:
            language::meta iMeta;
            const concept_libraries_t& iConceptLibraries;
            atom_ptr iRoot;
            atom_references_t iAtomReferences;
        };
    }
}