//
// Created by Antonin Fruchet on 27/05/2026.
//

#include "AnimatorGraph.h"

std::variant<bool, float> Parameter::getValue()
{
    auto ret_value = value;
    if (type==Trigger)
    {
        value = false;
    }
    return ret_value;
}

bool Condition::evaluate(std::unordered_map<std::string, Parameter>& parameters) const
{
    Parameter& param{parameters.at(parameterName)};
    if (param.type == Float)
    {
        switch (cmp)
        {
        case Equals : return std::get<float>(param.getValue()) == std::get<float>(cmp_val);
        case Less : return std::get<bool>(param.getValue()) < std::get<bool>(cmp_val);
        case Greater : return std::get<bool>(param.getValue()) > std::get<bool>(cmp_val);
        default: return false;
        }
    } else
    {
        return std::get<bool>(param.getValue()) == std::get<bool>(cmp_val);
    }
}

bool Transition::check(std::unordered_map<std::string, Parameter>& parameters) const
{
    if (parameters.empty()) return true;
    return std::ranges::all_of(conditions, [&](const Condition& c){return c.evaluate(parameters);});
}

