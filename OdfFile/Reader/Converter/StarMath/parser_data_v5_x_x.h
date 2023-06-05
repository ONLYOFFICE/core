#ifndef SM_PARSER_PARSERDATA_V5_X_X_H
#define SM_PARSER_PARSERDATA_V5_X_X_H

#include <memory>

#include "base_types.h"

namespace StarMathParser::v5_x_x
{

extern const std::shared_ptr<TVecParserPriorityPair> vDividers;
extern const std::shared_ptr<TVecParserPriorityPair> vOperators;
//  список функций для разбора
extern const std::vector<std::pair<TString, int>> vFunctions;

}

#endif // SM_PARSER_PARSERDATA_V5_X_X_H
