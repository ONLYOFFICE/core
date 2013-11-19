#include "TextAutonumberScheme.h"

namespace PPTX
{
	namespace Limit
	{
		TextAutonumberScheme::TextAutonumberScheme()
		{
			add("alphaLcParenBoth");
			add("alphaLcParenR");
			add("alphaLcPeriod");
			add("alphaUcParenBoth");
			add("alphaUcParenR");
			add("alphaUcPeriod");
			add("arabic1Minus");
			add("arabic2Minus");
			add("arabicDbPeriod");
			add("arabicDbPlain");
			add("arabicParenBoth");
			add("arabicParenR");
			add("arabicPeriod");
			add("arabicPlain");
			add("circleNumDbPlain");
			add("circleNumWdBlackPlain");
			add("circleNumWdWhitePlain");
			add("ea1ChsPeriod");
			add("ea1ChsPlain");
			add("ea1ChtPeriod");
			add("ea1ChtPlain");
			add("ea1JpnChsDbPeriod");
			add("ea1JpnKorPeriod");
			add("ea1JpnKorPlain");
			add("hebrew2Minus");
			add("hindiAlpha1Period");
			add("hindiAlphaPeriod");
			add("hindiNumParenR");
			add("hindiNumPeriod");
			add("romanLcParenBoth");
			add("romanLcParenR");
			add("romanLcPeriod");
			add("romanUcParenBoth");
			add("romanUcParenR");
			add("romanUcPeriod");
			add("thaiAlphaParenBoth");
			add("thaiAlphaParenR");
			add("thaiAlphaPeriod");
			add("thaiNumParenBoth");
			add("thaiNumParenR");
			add("thaiNumPeriod");
		}

		const std::string TextAutonumberScheme::no_find() const
		{
			return "arabicPlain";
		}
	} // namespace Limit
} // namespace PPTX