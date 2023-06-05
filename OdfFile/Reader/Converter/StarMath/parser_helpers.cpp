#include "parser_helpers.h"

#include <algorithm>

namespace StarMathParser::Parser::Helpers
{

std::vector<TChar> allowed =
{
	'.',
	'_',
	'%',
};

bool IsAllowedAlnum(const TString& input, int index, int& len)
{
	len = 0;
	TChar ch = input[index];
	auto res = std::find(std::begin(allowed), std::end(allowed), ch);
	if ((res != std::end(allowed)) || (bool)std::isalnum(ch))
	{
		len = 1;
		return true;
	}
	return false;
}

bool IsAllowedSequence(const std::shared_ptr<TVecParserPriorityPair>& allowed_sequence, const TString& input, int index, int& len)
{
	len = 0;
	for (const auto& pair : *allowed_sequence)
	{
		auto res = input.find(pair.first, index);
		if (res != TString::npos && res == index)
		{
			len = pair.first.length();
			return true;
		}
	}

	return false;
}


bool IsOneOf(const TString& expr, const std::vector<TString>& samples)
{
	return std::find(std::begin(samples), std::end(samples), expr) != std::end(samples);
}

}
