// core.cpp : Defines the exported functions for the DLL application.
//

#include <neolib/neolib.hpp>
#include <neolib/i_application.hpp>
#include <neolib/i_plugin.hpp>
#include <neos/concept_library.hpp>
#include <neos/context.hpp>

auto const sId = neolib::make_uuid("C5A28968-B129-4199-8AD6-3E499D9A8AE7");

API void entry_point(neolib::i_application& aApplication, const neolib::i_string& aPluginFolder, neolib::i_plugin*& aPluginInstance)
{
    aPluginInstance = new neos::concept_library{ sId, "neos.core" };
}

