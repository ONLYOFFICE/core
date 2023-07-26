#include "gtest/gtest.h"
#include <numeric>
#include <string>
#include <algorithm>
#include "../katana-parser/src/katana.h"

#define TEST_DUMP_OUTPUT  0
#define TEST_ERRORS_DEBUG 0

struct CSSRule
{
	std::string m_wsProperty;
	std::string m_wsValue;

	operator std::string() const
	{
		return m_wsProperty + ":" + m_wsValue;
	}

	friend std::ostream& operator<<(std::ostream& os, const CSSRule& obj)
	{
		return os << obj.m_wsProperty << ":" << obj.m_wsValue;
	}

	bool operator==(const CSSRule& oCSSRule) const
	{
		return m_wsProperty == oCSSRule.m_wsProperty && m_wsValue == oCSSRule.m_wsValue;
	}
};

struct CSSStyle
{
	std::vector<std::string> m_arSelectors;
	std::vector<CSSRule> m_arRules;

	friend std::ostream& operator<<(std::ostream& os, const CSSStyle& obj)
	{
		if (!obj.m_arSelectors.empty())
		{
			std::string sSelectors = "\nSelectors:\n";

			for (const std::string& sSelector : obj.m_arSelectors)
				sSelectors += sSelector + ", ";

			sSelectors.pop_back(); sSelectors.pop_back();

			os << sSelectors;
		}

		if (!obj.m_arRules.empty())
		{
			std::string sRules = "\nRules:\n";

			for (const CSSRule& oRule : obj.m_arRules)
			{
				sRules += oRule;
				sRules += '\n';
			}

			os << sRules;
		}

		return os;
	}

	bool operator==(const CSSStyle& oCSSStyle) const
	{
		return m_arSelectors == oCSSStyle.m_arSelectors && m_arRules == oCSSStyle.m_arRules;
	}
};

std::vector<CSSStyle> ParseCSS(const char* CSS, bool bOnlyRules = true)
{
	KatanaOutput *pOutput = katana_parse(CSS, strlen(CSS), KatanaParserModeStylesheet);

	#ifdef TEST_ERRORS_DEBUG
	#if TEST_ERRORS_DEBUG
	KatanaError *pError;
	for (size_t unErrorIndex = 0; unErrorIndex < pOutput->errors.length; ++unErrorIndex)
	{
		pError = (KatanaError*)pOutput->errors.data[unErrorIndex];
		printf("Error in %d.%d - %d.%d : %s\n", pError->first_line, pError->first_column, pError->last_line, pError->last_column, pError->message);
	}
	#endif
	#endif

	if (0 == pOutput->stylesheet->rules.length)
		return {};

	std::vector<CSSStyle> arCSSStyles;

	KatanaStyleRule *pStyleRule;
	for (size_t unRuleIndex = 0; unRuleIndex < pOutput->stylesheet->rules.length; ++unRuleIndex)
	{
		if (KatanaRuleStyle != ((KatanaRule*)pOutput->stylesheet->rules.data[unRuleIndex])->type)
			continue;

		pStyleRule = (KatanaStyleRule*)pOutput->stylesheet->rules.data[unRuleIndex];

		CSSStyle oCSSStyle;

		if (!bOnlyRules && 0 != pStyleRule->selectors->length)
		{
			KatanaSelector *pSelector;
			for (size_t unSelectorIndex = 0; unSelectorIndex < pStyleRule->selectors->length; ++unSelectorIndex)
			{
				pSelector = (KatanaSelector*)pStyleRule->selectors->data[unSelectorIndex];

				switch (pSelector->match)
				{
				case KatanaSelectorMatchTag:
				{
					oCSSStyle.m_arSelectors.push_back(std::string(pSelector->tag->local));
					break;
				}
				case KatanaSelectorMatchClass:
				{
					oCSSStyle.m_arSelectors.push_back('.' + std::string(pSelector->data->value));
					break;
				}
				case KatanaSelectorMatchId:
				{
					oCSSStyle.m_arSelectors.push_back('#' + std::string(pSelector->data->value));
					break;
				}
				default: continue;
				}
			}
		}

		KatanaDeclaration* pDeclaration;
		for (size_t unDeclarationIndex = 0; unDeclarationIndex < pStyleRule->declarations->length; ++unDeclarationIndex)
		{
			pDeclaration = (KatanaDeclaration*)pStyleRule->declarations->data[unDeclarationIndex];
			oCSSStyle.m_arRules.push_back({pDeclaration->property, std::string(pDeclaration->raw)});
		}

		arCSSStyles.push_back(oCSSStyle);
	}

	#ifdef TEST_DUMP_OUTPUT
	#if TEST_DUMP_OUTPUT
	katana_dump_output(pOutput);
	#endif
	#endif

	katana_destroy_output(pOutput);

	return arCSSStyles;
}

TEST(KatanaParserTest, TestStandartRule)
{
	const char* CSS = R"(
		.test {
			color: white;
			text-decoration: underline;
		}
	)";

	std::vector<CSSStyle> arStyles;

	CSSStyle oReferenceStyle;
	oReferenceStyle.m_arSelectors.push_back(".test");
	oReferenceStyle.m_arRules.push_back({"color", "white"});
	oReferenceStyle.m_arRules.push_back({"text-decoration", "underline"});

	arStyles.push_back(oReferenceStyle);

	EXPECT_EQ(arStyles, ParseCSS(CSS, false));
}

TEST(KatanaParserTest, TestSkipRule)
{
	const char* CSS = R"(
		.
		test {
			color: red;
			font-size: 24px;
		}
	)";

	std::vector<CSSStyle> arStyles;

	CSSStyle oReferenceStyle;
	oReferenceStyle.m_arRules.push_back({"color", "red"});
	oReferenceStyle.m_arRules.push_back({"font-size", "24px"});

	arStyles.push_back(oReferenceStyle);

	EXPECT_EQ(arStyles, ParseCSS(CSS));
}

TEST(KatanaParserTest, TestPageRule)
{
	const char* CSS = R"(
		@page {
			color: white;
			text-decoration: underline;
		}
	)";

	std::vector<CSSStyle> arStyles;

	CSSStyle oReferenceStyle;
	oReferenceStyle.m_arRules.push_back({"color", "white"});
	oReferenceStyle.m_arRules.push_back({"text-decoration", "underline"});

	arStyles.push_back(oReferenceStyle);

	EXPECT_EQ(arStyles, ParseCSS(CSS));
}

TEST(KatanaParserTest, TestCrash)
{
	const char* CSS = R"(
		div | p | span {
			color: blue;
			font-family: Arial;
		}
	)";

	std::vector<CSSStyle> arStyles;

	CSSStyle oReferenceStyle;

	oReferenceStyle.m_arRules.push_back({"color", "blue"});
	oReferenceStyle.m_arRules.push_back({"font-family", "Arial"});

	arStyles.push_back(oReferenceStyle);

	EXPECT_EQ(arStyles, ParseCSS(CSS));
}

