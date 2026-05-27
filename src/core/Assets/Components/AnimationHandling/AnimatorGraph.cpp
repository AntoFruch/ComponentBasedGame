//
// Created by Antonin Fruchet on 27/05/2026.
//

#include "AnimatorGraph.h"

bool Condition::evaluate(const std::unordered_map<std::string, bool>& parameters) const
{
    return parameters.at(parameterName) == expected_val;
}

bool Transition::check(const std::unordered_map<std::string, bool>& parameters) const
{
    return std::ranges::all_of(conditions, [&](const Condition& c){return c.evaluate(parameters);});
}

