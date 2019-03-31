/*
  math.cpp

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
#include "math.hpp"

namespace neos::concept::core
{   
    math::math(language::i_concept_library& aParent) :
        neos::language::concept_library
        { 
            aParent,
            library_id(), 
            aParent.uri().to_std_string(),
            library_name(), 
            "Core mathematical concepts", 
            neolib::version{ 1, 0, 0 }, 
            "Copyright (c) 2019 Leigh Johnston.  All Rights Reserved."
        }
    {
        /* todo */
        concepts()[neolib::string{ "math.expression" }] = neolib::make_ref<language::unimplemented_concept>("math.expression");
        concepts()[neolib::string{ "math.expression.operand" }] = neolib::make_ref<language::unimplemented_concept>("math.expression.operand");
        concepts()[neolib::string{ "math.operator" }] = neolib::make_ref<language::unimplemented_concept>("math.operator");
        concepts()[neolib::string{ "math.operator.negate" }] = neolib::make_ref<language::unimplemented_concept>("math.operator.negate");
        concepts()[neolib::string{ "math.operator.addition" }] = neolib::make_ref<language::unimplemented_concept>("math.operator.addition");
        concepts()[neolib::string{ "math.operator.subtraction" }] = neolib::make_ref<language::unimplemented_concept>("math.operator.subtraction");
        concepts()[neolib::string{ "math.operator.multiplication" }] = neolib::make_ref<language::unimplemented_concept>("math.operator.multiplication");
        concepts()[neolib::string{ "math.operator.division" }] = neolib::make_ref<language::unimplemented_concept>("math.operator.division");
    }

    const std::string& math::library_name()
    {
        static const std::string sName = "neos.math";
        return sName;
    }

    const neolib::uuid& math::library_id()
    {
        static const neolib::uuid sId = neolib::make_uuid("9AECA57F-0F8E-450A-BB5C-8AC12BFE0E47");
        return sId;
    }
}

