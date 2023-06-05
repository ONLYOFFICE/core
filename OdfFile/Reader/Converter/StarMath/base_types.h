#ifndef SM_PARSER_BASETYPES_H
#define SM_PARSER_BASETYPES_H

#include <string>
#include <vector>

namespace StarMathParser
{
    typedef std::string TString;
    typedef char TChar;
    typedef std::pair<TString, int> TParserPriorityPair;
    typedef std::vector<TParserPriorityPair> TVecParserPriorityPair;
}

#endif // SM_PARSER_BASETYPES_H
