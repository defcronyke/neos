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

#include "math.hpp"

namespace neos
{
    namespace concept
    {
        namespace core
        {   
            math::math(const std::string& aLibraryUri) :
                neos::language::concept_library
                { 
                    library_id(), 
                    aLibraryUri, 
                    library_name(), 
                    "Core mathematical concepts", 
                    neolib::version{ 1, 0, 0 }, 
                    "Copyright (c) 2019 Leigh Johnston.  All Rights Reserved."
                }
            {
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
    }
}

