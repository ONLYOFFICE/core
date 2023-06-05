#ifndef PARSER_H
#define PARSER_H

#include <memory>

#include "attributes.h"
#include "base_types.h"
#include "iterator.h"
#include "token_with_attributes.h"
#include "parser.h"

namespace StarMathParser
{

class Parser_v5_x_x : public Interfaces::Parser
{
	std::vector<std::vector<Attributes>> m_vAttributes;
public:
	Parser_v5_x_x();
	virtual ~Parser_v5_x_x();

	std::vector<TokenWithAttributes> SplitByTokens(const TString& expression) override;
	Interfaces::Versioning::Version GetVersion() const override;
	const std::vector<std::vector<Attributes>>& GetAttributesStorage() const override;
private:
	uint16_t MakeGroupFlag(const TString& expr);
	bool IsAttribute(const TString& expr, int& chain_size);
	bool IsAttributeParam(const TString& attribute, const TString& param);
};

}

#endif // PARSER_H
