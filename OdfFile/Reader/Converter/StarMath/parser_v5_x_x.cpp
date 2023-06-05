#include "parser_v5_x_x.h"

#include <stack>

#include "parser_iterator.h"
#include "parser_data_v5_x_x.h"
#include "parser_helpers.h"

namespace StarMathParser
{

Parser_v5_x_x::Parser_v5_x_x()
{
    //ctor
}

Parser_v5_x_x::~Parser_v5_x_x()
{
    //dtor
}

std::vector<TokenWithAttributes> Parser_v5_x_x::SplitByTokens(const TString& expression)
{
	auto iter = std::make_shared<ParserIterator>(v5_x_x::vDividers, v5_x_x::vOperators, expression);
	std::vector<TokenWithAttributes> res;
	std::vector<TString> groups;
	int level = 0;
	uint16_t group_flag = 0;
	int active_attribute_index = -1;

	struct AttrsForLevel {
			int m_nAttrIndex;
			int m_nNestedLevel;
	};

	std::vector<AttrsForLevel> vAttributesStack;
	// последовательно идущие атрибуты для одного элемента
	std::vector<Attributes> vConsecutiveAttributesChain;

	while (iter->HasNext())
	{
		auto token = iter->GetNext();

		if (StarMathParser::Parser::Helpers::IsOneOf(token, {"{", "("})) // group symbol
		{
			groups.push_back(token);
			group_flag = MakeGroupFlag(token);

			if (!vConsecutiveAttributesChain.empty()) {
					// помещаем набранные атрибуты в общий список атрибутов
				m_vAttributes.emplace_back(vConsecutiveAttributesChain);
				vConsecutiveAttributesChain.clear();
				vAttributesStack.push_back({m_vAttributes.size()-1, groups.size()}); // кладем в стек индекс списка атрибутов для группы
			}
		}
		else if (StarMathParser::Parser::Helpers::IsOneOf(token, {"}", ")"})) // ungroup symbol
		{
			group_flag = MakeGroupFlag("");
			if (!groups.empty())
			{
				group_flag = MakeGroupFlag(groups.back());
			}

			if (!vAttributesStack.empty() && vAttributesStack.back().m_nNestedLevel == groups.size()) {
				vAttributesStack.pop_back(); // извлекаем список атрибутов для группы
			}

			if (!groups.empty())
			{
				groups.pop_back();
			}
		}
		else
		{
			int chain_elements = 0;
			if (IsAttribute(token, chain_elements))
			{
				auto attribute_name = token;
				bool skipped = false;
				Attributes attr;
				attr.m_vAttributes.emplace_back(attribute_name);
				while (chain_elements > 1 && iter->HasNext())   // так как мы уже прочитали первый элемент цепочки
				{
					auto param = iter->GetNext();
					if (IsAttributeParam(attribute_name, param))
					{
						attr.m_vAttributes[attr.m_vAttributes.size()-1]+=" " + param;
					}
					else
					{
						// skip attribute with wrong params
						skipped = true;
						break;
					}
					chain_elements--;
				}

				if (!skipped)
				{
					vConsecutiveAttributesChain.emplace_back(attr); // сюда набираем подряд идущие атрибуты
				}
			}
			else
			{
				level = groups.size();

				auto vActiveAttributesSet = vAttributesStack;
				if (!vConsecutiveAttributesChain.empty()) {
					// помещаем набранные атрибуты в общий список атрибутов
					m_vAttributes.emplace_back(vConsecutiveAttributesChain);
					vConsecutiveAttributesChain.clear();
					vActiveAttributesSet.push_back({m_vAttributes.size()-1, 0});
				}

				std::vector<int> vAttributes;
				for (auto pair: vActiveAttributesSet) {
						vAttributes.emplace_back(pair.m_nAttrIndex);
				}

				res.emplace_back(TokenWithAttributes{token, level, group_flag, vAttributes});
			}
		}
	}
	return res;
}

uint16_t Parser_v5_x_x::MakeGroupFlag(const TString& expr)
{
    if (expr == "{")
    {
        return '{}';
    }
    else if (expr == "(")
    {
        return '()';
    }

    return 0;
}

bool Parser_v5_x_x::IsAttribute(const TString& expr, int& chain_size)
{
    if (expr == "color")
    {
        return (chain_size = 2)!=0;
    } else if (expr == "bold")
    {
        return (chain_size = 1)!=0;
    }

    return 0;
}

bool Parser_v5_x_x::IsAttributeParam(const TString& attribute, const TString& param)
{
    if (attribute == "color")
    {
        if (StarMathParser::Parser::Helpers::IsOneOf(param, {"lime", "navy", "fuchsia"}))
        {
            return true;
        }
    } else if (attribute == "bold") {
        return true;
    }
    return false;
}

Interfaces::Versioning::Version Parser_v5_x_x::GetVersion() const {
    return Interfaces::Versioning::Version{"5", "0", "x"};
}

const std::vector<std::vector<Attributes>>& Parser_v5_x_x::GetAttributesStorage() const {
    return m_vAttributes;
}

}
