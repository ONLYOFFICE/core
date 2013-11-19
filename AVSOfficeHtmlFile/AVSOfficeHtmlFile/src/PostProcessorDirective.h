#pragma once

#include <string>

class PostProcessorDirective
{
    virtual std::string GetDirectiveString() = 0;
};