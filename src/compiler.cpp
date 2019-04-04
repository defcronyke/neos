/*
  compiler.cpp

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

#include <neolib/neolib.hpp>
#include <iostream>
#include <neolib/recursion.hpp>
#include <neolib/string_utf.hpp>
#include <neos/bytecode/opcodes.hpp>
#include <neos/bytecode/text.hpp>
#include <neos/language/compiler.hpp>

namespace neos::language
{
    compiler::compiler() :
        iTrace{ false }, iTraceEmits{ false }, iStartTime{ std::chrono::steady_clock::now() }, iEndTime{ std::chrono::steady_clock::now() }
    {
    }

    bool compiler::trace() const
    {
        return iTrace;
    }

    void compiler::set_trace(bool aTrace)
    {
        iTrace = aTrace;
    }

    bool compiler::trace_emits() const
    {
        return iTraceEmits;
    }

    void compiler::set_trace_emits(bool aTraceEmits)
    {
        iTraceEmits = aTraceEmits;
    }

    const std::chrono::steady_clock::time_point& compiler::start_time() const
    {
        return iStartTime;
    }

    const std::chrono::steady_clock::time_point& compiler::end_time() const
    {
        return iEndTime;
    }

    void compiler::compile(program& aProgram)
    {
        iStartTime = std::chrono::steady_clock::now();

        try
        {
            for (auto const& unit : aProgram.translationUnits)
            {
                iWhitespaceConcept = unit.schema->find_concept("language.whitespace");
                auto source = unit.source.begin();
                while (source != unit.source.end())
                {
                    iDeepestProbe = std::nullopt;
                    auto result = parse(compiler_pass::Emit, aProgram, unit, unit.schema->root(), source);
                    if (!result.sourceParsed || result.sourceParsed == source)
                        throw_error(unit, iDeepestProbe ? *iDeepestProbe : source, "syntax error");
                    source = *result.sourceParsed;
                }
            }
        }
        catch(...)
        {
            iEndTime = std::chrono::steady_clock::now();
            throw;
        }

        iEndTime = std::chrono::steady_clock::now();
    }

    compiler::parse_result compiler::parse(compiler_pass aPass, program& aProgram, const translation_unit& aUnit, const i_schema_node_atom& aAtom, source_iterator aSource)
    {
        _limit_recursion_to_(compiler, aUnit.schema->meta().parserRecursionLimit);
        if (aPass == compiler_pass::Emit)
        {
            auto probeResult = parse(compiler_pass::Probe, aProgram, aUnit, aAtom, aSource);
            if (!probeResult.sourceParsed || probeResult.sourceParsed == aSource)
                return probeResult;
        }
        if (trace())
            std::cout << std::string(_compiler_recursion_limiter_.depth(), ' ') << "parse(" << aAtom.symbol() << ")" << std::endl;
        bool const expectingToken = !aAtom.expects().empty();
        if (aSource != aUnit.source.end())
        {
            if (expectingToken)
            {
                for (auto const& expect : aAtom.expects())
                {
                    auto result = parse_expect(aPass, aProgram, aUnit, aAtom, *expect, aSource);
                    if (result.sourceParsed && result.sourceParsed != aSource)
                        return result;
                }
                return parse_result{};
            }
            return parse_tokens(aPass, aProgram, aUnit, aAtom, aSource);
        }
        else
            return expectingToken ? parse_result{} : parse_result{ aSource };
        //auto loop = emit(aProgram.text, bytecode::opcode::ADD, bytecode::registers::R1, bytecode::u64{ 10 });
        //emit(aProgram.text, bytecode::opcode::ADD, bytecode::registers::R1, bytecode::i8{ -1 });
        //emit(aProgram.text, bytecode::opcode::B, loop);
    }

    compiler::parse_result compiler::parse_expect(compiler_pass aPass, program& aProgram, const translation_unit& aUnit, const i_schema_node_atom& aAtom, const i_atom& aExpectedToken, source_iterator aSource)
    {
        _limit_recursion_to_(compiler, aUnit.schema->meta().parserRecursionLimit);
        if (aPass == compiler_pass::Emit)
        {
            auto probeResult = parse_expect(compiler_pass::Probe, aProgram, aUnit, aAtom, aExpectedToken, aSource);
            if (!probeResult.sourceParsed || probeResult.sourceParsed == aSource)
                return probeResult;
        }
        if (trace())
            std::cout << std::string(_compiler_recursion_limiter_.depth(), ' ') << "parse_expect(" << aAtom.symbol() << ")" << std::endl;
        if (aExpectedToken.is_schema_atom() && aExpectedToken.as_schema_atom().is_schema_node_atom())
        {
            auto result = parse(aPass, aProgram, aUnit, aExpectedToken.as_schema_atom().as_schema_node_atom(), aSource);
            if (result.sourceParsed && result.sourceParsed != aSource)
                return parse_tokens(aPass, aProgram, aUnit, aAtom, aExpectedToken, *result.sourceParsed, false);
        }
        else if (aExpectedToken.is_schema_atom() && aExpectedToken.as_schema_atom().is_schema_terminal_atom())
            return parse_token(aPass, aProgram, aUnit, aAtom, aExpectedToken, aSource);
        return parse_result{ aSource };
    }

    compiler::parse_result compiler::parse_tokens(compiler_pass aPass, program& aProgram, const translation_unit& aUnit, const i_schema_node_atom& aAtom, source_iterator aSource)
    {
        _limit_recursion_to_(compiler, aUnit.schema->meta().parserRecursionLimit);
        if (aPass == compiler_pass::Emit)
        {
            auto probeResult = parse_tokens(compiler_pass::Probe, aProgram, aUnit, aAtom, aSource);
            if (!probeResult.sourceParsed || probeResult.sourceParsed == aSource)
                return probeResult;
        }
        if (trace())
            std::cout << std::string(_compiler_recursion_limiter_.depth(), ' ') << "parse_tokens(" << aAtom.symbol() << ")" << std::endl;
        auto currentSource = aSource;
        for (auto iterToken = aAtom.tokens().begin(); currentSource != aUnit.source.end() && iterToken != aAtom.tokens().end();)
        {
            auto const& token = *iterToken->first();
            auto const& tokenValue = *iterToken->second();
            auto result = parse_token(aPass, aProgram, aUnit, aAtom, token, currentSource);
            bool const ateSome = (result.sourceParsed && result.sourceParsed != currentSource);
            if (ateSome)
            {
                auto const& matchedTokenValue = tokenValue;
                if (!matchedTokenValue.is_schema_atom() || matchedTokenValue.as_schema_atom().is_schema_node_atom())
                {
                    auto trySource = result.sourceParsed;
                    if (aAtom.is_parent_of(matchedTokenValue))
                        result = parse_token(aPass, aProgram, aUnit, aAtom, matchedTokenValue, *trySource);
                    bool const ateSome = (result.sourceParsed && result.sourceParsed != trySource);
                    if (ateSome || (result.sourceParsed && !aAtom.is_parent_of(matchedTokenValue)))
                    {
                        if (matchedTokenValue.is_schema_atom())
                            result = parse_tokens(aPass, aProgram, aUnit, matchedTokenValue.as_schema_atom().as_schema_node_atom(), token, *result.sourceParsed, false);
                        if (result.sourceParsed)
                            result = parse_tokens(aPass, aProgram, aUnit, aAtom, matchedTokenValue, *result.sourceParsed, false);
                        if (result.sourceParsed)
                        {
                            currentSource = *result.sourceParsed;
                            iterToken = aAtom.tokens().begin();
                        }
                        else
                            return result;
                    }
                    else if (result.sourceParsed)
                        ++iterToken;
                    else
                        return result;
                }
                else if (matchedTokenValue.is_schema_atom() && matchedTokenValue.as_schema_atom().is_schema_terminal_atom())
                {
                    result = parse_token(aPass, aProgram, aUnit, aAtom, matchedTokenValue, *result.sourceParsed);
                    if (result.action == parse_result::Ignored)
                    {
                        currentSource = *result.sourceParsed;
                        iterToken = aAtom.tokens().begin();
                    }
                    else
                        return result;
                }
                else
                    return result;
            }
            else
                ++iterToken;
            if (iterToken == aAtom.tokens().end())
            {
                if (token.is_schema_atom() && token.as_schema_atom().is_schema_terminal_atom() &&
                    token.as_schema_atom().as_schema_terminal_atom().type() == schema_terminal::Default)
                {
                    auto result = parse_token(aPass, aProgram, aUnit, aAtom, tokenValue, currentSource);
                    if (result.sourceParsed)
                    {
                        if (result.sourceParsed != currentSource)
                            currentSource = *result.sourceParsed;
                        else if (result.action == parse_result::Ignored && !ateSome)
                            ++currentSource;
                    }
                }
            }
        }
        return parse_result{ currentSource };
    }

    compiler::parse_result compiler::parse_tokens(compiler_pass aPass, program& aProgram, const translation_unit& aUnit, const i_schema_node_atom& aAtom, const i_atom& aMatchedTokenValue, source_iterator aSource, bool aSelf)
    {
        _limit_recursion_to_(compiler, aUnit.schema->meta().parserRecursionLimit);
        if (aPass == compiler_pass::Emit)
        {
            auto probeResult = parse_tokens(compiler_pass::Probe, aProgram, aUnit, aAtom, aMatchedTokenValue, aSource);
            if (!probeResult.sourceParsed || probeResult.sourceParsed == aSource)
                return probeResult;
        }
        if (trace())
            std::cout << std::string(_compiler_recursion_limiter_.depth(), ' ') << "parse_tokens(" << aAtom.symbol() << ":" << aMatchedTokenValue.symbol() << ")" << std::endl;
        auto currentSource = aSource;
        auto matchedToken = aAtom.find_token(aMatchedTokenValue);
        if (matchedToken != nullptr)
        {
            parse_result result{ currentSource };
            if (!matchedToken->is_concept_atom())
                result = parse_token(aPass, aProgram, aUnit, aAtom, *matchedToken, currentSource);
            if (result.sourceParsed)
                return parse_tokens(aPass, aProgram, aUnit, aAtom, *matchedToken, *result.sourceParsed);
            return result;
        }
        return parse_result{ currentSource };
    }

    compiler::parse_result compiler::parse_token(compiler_pass aPass, program& aProgram, const translation_unit& aUnit, const i_schema_node_atom& aAtom, const i_atom& aToken, source_iterator aSource)
    {
        _limit_recursion_to_(compiler, aUnit.schema->meta().parserRecursionLimit);
        if (aPass == compiler_pass::Emit)
        {
            auto probeResult = parse_token(compiler_pass::Probe, aProgram, aUnit, aAtom, aToken, aSource);
            if (!probeResult.sourceParsed || probeResult.action == parse_result::Error)
                return probeResult;
        }
        if (trace())
            std::cout << std::string(_compiler_recursion_limiter_.depth(), ' ') << "parse_token(" << aAtom.symbol() << ":" << aToken.symbol() << ")" << std::endl;
        if (iDeepestProbe == std::nullopt || *iDeepestProbe < aSource)
            iDeepestProbe = aSource;
        if (aToken.is_schema_atom())
        {
            if (aToken.as_schema_atom().is_schema_node_atom())
                return parse(aPass, aProgram, aUnit, aToken.as_schema_atom().as_schema_node_atom(), aSource);
            else if (aToken.as_schema_atom().is_schema_terminal_atom())
            {
                auto const& terminal = aToken.as_schema_atom().as_schema_terminal_atom();
                switch (terminal.type())
                {
                case schema_terminal::Default:
                    break;
                case schema_terminal::Error:
                    return parse_result{ aSource, parse_result::Error };
                case schema_terminal::Ignore:
                    return parse_result{ aSource, parse_result::Ignored };
                case schema_terminal::Next:
                    {
                        auto result = consume_token(aPass, aProgram, aUnit, aAtom, aSource);
                        if (result.action == parse_result::Consumed)
                            result.action = parse_result::ForNext;
                        return result;
                    }
                case schema_terminal::Done:
                    return consume_token(aPass, aProgram, aUnit, aAtom, aSource);
                case schema_terminal::String:
                    if (std::distance(aSource, aUnit.source.end()) >= terminal.symbol().size() &&
                        std::equal(terminal.symbol().begin(), terminal.symbol().end(), aSource))
                    {
                        return parse_result{ aSource + terminal.symbol().size() };
                    }
                    return parse_result{ aSource };
                default:
                    // do nothing
                    break;
                }
            }
        }
        else if (aToken.is_concept_atom())
            return consume_token(aPass, aProgram, aUnit, aToken, aSource);
        return parse_result{ aSource };
    }

    compiler::parse_result compiler::consume_token(compiler_pass aPass, program& aProgram, const translation_unit& aUnit, const i_atom& aToken, source_iterator aSource)
    {
        _limit_recursion_to_(compiler, aUnit.schema->meta().parserRecursionLimit);
        if (aToken.is_concept_atom())
            return consume_concept_token(aPass, aProgram, aUnit, aToken.as_concept_atom().concept(), aSource);
        else if (aToken.is_schema_atom() && aToken.as_schema_atom().is_schema_node_atom())
        {
            parse_result result{ aSource };
            for (auto& concept : aToken.as_schema_atom().as_schema_node_atom().is_a())
                if (result.sourceParsed && result.action != parse_result::Error)
                    result = consume_concept_atom(aPass, aProgram, aUnit, aToken, *concept, *result.sourceParsed);
            return result;
        }
        return parse_result{ aSource };
    }

    compiler::parse_result compiler::consume_concept_token(compiler_pass aPass, program& aProgram, const translation_unit& aUnit, const i_concept& aConcept, source_iterator aSource)
    {
        _limit_recursion_to_(compiler, aUnit.schema->meta().parserRecursionLimit);
        auto result = aConcept.consume_token(aPass, aSource, aUnit.source.end());
        if (trace_emits() && aPass == compiler_pass::Emit && result.consumed)
            std::cout << "{" << _compiler_recursion_limiter_.depth() << "}: " << aConcept.name() << std::endl;
        return parse_result{ result.sourceParsed, result.consumed ? parse_result::Consumed : parse_result::Error };
    }

    compiler::parse_result compiler::consume_concept_atom(compiler_pass aPass, program& aProgram, const translation_unit& aUnit, const i_atom& aAtom, const i_concept& aConcept, source_iterator aSource)
    {
        _limit_recursion_to_(compiler, aUnit.schema->meta().parserRecursionLimit);
        auto result = aConcept.consume_atom(aPass, aAtom, aSource, aUnit.source.end());
        if (trace_emits() && aPass == compiler_pass::Emit && result.consumed)
            std::cout << "{" << _compiler_recursion_limiter_.depth() << "}: " << aConcept.name() << std::endl;
        return parse_result{ result.sourceParsed, result.consumed ? parse_result::Consumed : parse_result::Error };
    }

    void compiler::throw_error(const translation_unit& aUnit, source_iterator aSourcePos, const std::string& aError)
    {
        uint32_t line = 1;
        uint32_t col = 1;
        for (auto pos = aUnit.source.begin(); pos != aSourcePos; ++pos)
        {
            if (*pos == '\n')
            {
                ++line;
                col = 1;
            }
            else
                ++col;
        }
        throw std::runtime_error("(" + aError + ") line " + boost::lexical_cast<std::string>(line) + ", col " + boost::lexical_cast<std::string>(col));
    }
}
