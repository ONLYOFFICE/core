/* SPDX-License-Identifier: AGPL-3.0-only; see LICENSE for additional terms. */
#include "../../XlsxFormat/Drawing/WpsCellImages.h"
#include <gtest/gtest.h>

TEST(WpsCellImageFormula, RecognizesOnlyCompleteLiteralPhotoCalls)
{
	std::wstring id;
	for (const auto& formula : {L"DISPIMG(\"ID_PHOTO\",1)", L"_xlfn.DISPIMG(\"ID_PHOTO\",1)",
		L" = @_xlfn.dispimg ( \"ID_PHOTO\" ; 1 ) "})
	{
		ASSERT_TRUE(OOX::Spreadsheet::WpsCellImages::GetImageId(formula, id));
		EXPECT_EQ(L"ID_PHOTO", id);
	}
	for (const auto& formula : {L"SUM(A1:A2)", L"IF(A1,DISPIMG(\"ID_PHOTO\",1),0)",
		L"DISPIMG(A1,1)", L"DISPIMG(\"ID_PHOTO\",2)", L"DISPIMG(\"\",1)",
		L"DISPIMG(\"ID_PHOTO\",1)+1", L"DISPIMG(\"ID_PHOTO\",1", L"\"DISPIMG(\"ID_PHOTO\",1)\""})
		EXPECT_FALSE(OOX::Spreadsheet::WpsCellImages::GetImageId(formula, id));
}
