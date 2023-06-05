#ifndef SM_PARSER_PARSER_H
#define SM_PARSER_PARSER_H

#include "attributes.h"
#include "base_types.h"
#include "token_with_attributes.h"
#include "versioning.h"

namespace StarMathParser::Interfaces
{

class Parser : public Versioning
{
public:
    virtual std::vector<TokenWithAttributes> SplitByTokens(const TString& expression) = 0;
    virtual const std::vector<std::vector<Attributes>>& GetAttributesStorage() const = 0;
};

}

#endif // SM_PARSER_PARSER_H
