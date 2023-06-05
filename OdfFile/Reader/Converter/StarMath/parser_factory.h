#ifndef PARSERFACTORY_H
#define PARSERFACTORY_H

#include <memory>

#include "parser.h"

namespace StarMathParser::Factory
{

std::shared_ptr<StarMathParser::Interfaces::Parser> createParser(const std::string& version);

}

#endif // PARSERFACTORY_H
