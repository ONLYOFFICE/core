#ifndef SM_PARSER_PARCE_HELPERS_H
#define SM_PARSER_PARCE_HELPERS_H

#include <memory>

#include "base_types.h"

namespace StarMathParser::Parser::Helpers
{

bool IsAllowedAlnum(const TString& input, int index, int& len);
bool IsAllowedSequence(const std::shared_ptr<TVecParserPriorityPair>& allowed_sequence, const TString& input, int index, int& len);
bool IsOneOf(const TString& expr, const std::vector<TString>& samples);

}

#endif // SM_PARSER_PARCE_HELPERS_H
