#include "../conversionmathformula.h"
#include "gtest/gtest.h"


TEST(SMConvectorTest,IndexUpperLower)
{
	std::wstring wsString = L"2^{ 1} _{ 3}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:sSubSup><m:sSubSupPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:sSubSupPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:e><m:sub><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>3</m:t></m:r></m:sub><m:sup><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:sup></m:sSubSup></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,LsupLsub)
{
	std::wstring wsString = L"{ 1} LSUP { 2}  LSUB { 3}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:sPre><m:sPrePr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:sPrePr><m:sub><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>3</m:t></m:r></m:sub><m:sup><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:sup><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:e></m:sPre></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,UnderOver)
{
	std::wstring wsString = L"UNDEROVER { 5} _{ 6} ^{ 7}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:limLow><m:limLowPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limLowPr><m:e><m:limUpp><m:limUppPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limUppPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>5</m:t></m:r></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>7</m:t></m:r></m:lim></m:limUpp></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>6</m:t></m:r></m:lim></m:limLow></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,VecUnder)
{
	std::wstring wsString = L"vec { 1} under { 2}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:acc><m:accPr><m:chr m:val=\"⃗\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:accPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:e></m:acc><m:acc><m:accPr><m:chr m:val=\"̲\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:accPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:e></m:acc></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,BarHat)
{
	std::wstring wsString = L"bar { 5} hat { 6}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:acc><m:accPr><m:chr m:val=\"̄\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:accPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>5</m:t></m:r></m:e></m:acc><m:acc><m:accPr><m:chr m:val=\"̂\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:accPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>6</m:t></m:r></m:e></m:acc></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,SqrtRoot)
{
	std::wstring wsString = L"sqrt {2} root {5} of {10}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:rad><m:radPr><m:degHide m:val=\"1\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:radPr><m:deg /><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:e></m:rad><m:rad><m:radPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:radPr><m:deg><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>5</m:t></m:r></m:deg><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>10</m:t></m:r></m:e></m:rad></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,Over)
{
	std::wstring wsString = L"{ 5} over {10 }";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:f><m:fPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:fPr><m:num><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>5</m:t></m:r></m:num><m:den><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>10</m:t></m:r></m:den></m:f></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,SumProdCoprod)
{
	std::wstring wsString = L"sum _{ 1} ^{ 2}  3 PROD _{ 4} ^{5 } 6 COPROD _{ 7} ^{ 8} 9";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:nary><m:naryPr><m:chr m:val=\"∑\" /><m:limLoc m:val=\"undOvr\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:naryPr><m:sub><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:sub><m:sup><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:sup><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>3</m:t></m:r></m:e></m:nary><m:nary><m:naryPr><m:chr m:val=\"∏\" /><m:limLoc m:val=\"undOvr\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:naryPr><m:sub><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>4</m:t></m:r></m:sub><m:sup><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>5</m:t></m:r></m:sup><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>6</m:t></m:r></m:e></m:nary><m:nary><m:naryPr><m:chr m:val=\"∐\" /><m:limLoc m:val=\"undOvr\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:naryPr><m:sub><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>7</m:t></m:r></m:sub><m:sup><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>8</m:t></m:r></m:sup><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>9</m:t></m:r></m:e></m:nary></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,BigPlusBigUplus)
{
	std::wstring wsString = L"BIGOPLUS _{ 1} ^{2 } 3 BIGUPLUS _{ 5} ^{10 } 15";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:limLow><m:limLowPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limLowPr><m:e><m:limUpp><m:limUppPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limUppPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>⊕</m:t></m:r></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:lim></m:limUpp></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:lim></m:limLow><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>3</m:t></m:r><m:limLow><m:limLowPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limLowPr><m:e><m:limUpp><m:limUppPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limUppPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>⊎</m:t></m:r></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>10</m:t></m:r></m:lim></m:limUpp></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>5</m:t></m:r></m:lim></m:limLow><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>15</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,Tint)
{
	std::wstring wsString = L"tint _{{2} over {3}} ^{4} {root {5} of {10}}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:nary><m:naryPr><m:chr m:val=\"∭\" /><m:limLoc m:val=\"subSup\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:naryPr><m:sub><m:f><m:fPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:fPr><m:num><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:num><m:den><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>3</m:t></m:r></m:den></m:f></m:sub><m:sup><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>4</m:t></m:r></m:sup><m:e><m:rad><m:radPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:radPr><m:deg><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>5</m:t></m:r></m:deg><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>10</m:t></m:r></m:e></m:rad></m:e></m:nary></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,Otint)
{
	std::wstring wsString = L"otint _{ 1} ^{ 2} { 3}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:nary><m:naryPr><m:chr m:val=\"∰\" /><m:limLoc m:val=\"subSup\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:naryPr><m:sub><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:sub><m:sup><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:sup><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>3</m:t></m:r></m:e></m:nary></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,RelAndBuildrelArrow)
{
	std::wstring wsString = L"REL LRARROW { 1} { 2} REL EXARROW { 3} { 4} BUILDREL rarrow { 5} { 6}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:limLow><m:limLowPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limLowPr><m:e><m:limUpp><m:limUppPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limUppPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>⇔</m:t></m:r></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:lim></m:limUpp></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:lim></m:limLow><m:limLow><m:limLowPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limLowPr><m:e><m:limUpp><m:limUppPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limUppPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>⇄</m:t></m:r></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>3</m:t></m:r></m:lim></m:limUpp></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>4</m:t></m:r></m:lim></m:limLow><m:limUpp><m:limUppPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limUppPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>→</m:t></m:r></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>5</m:t></m:r></m:lim></m:limUpp><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>6</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,LimLimlow)
{
	std::wstring wsString = L"Lim _{ ->inf} { 2} lim _{ ->0} { 5}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:limLow><m:limLowPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limLowPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>Lim</m:t></m:r></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>→</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>∞</m:t></m:r></m:lim></m:limLow><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r><m:limLow><m:limLowPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limLowPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>lim</m:t></m:r></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>→</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>0</m:t></m:r></m:lim></m:limLow><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>5</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,LfloorSquare)
{
	std::wstring wsString = L"LEFT [ { 1} over { 2} RIGHT ] LFLOOR sum _{ 1} ^{ 2} RFLOOR";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:d><m:dPr><m:begChr m:val=\"[\" /><m:endChr m:val=\"]\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:dPr><m:e><m:f><m:fPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:fPr><m:num><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:num><m:den><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:den></m:f></m:e></m:d><m:d><m:dPr><m:begChr m:val=\"⌊\" /><m:endChr m:val=\"⌋\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:dPr><m:e><m:nary><m:naryPr><m:chr m:val=\"∑\" /><m:limLoc m:val=\"undOvr\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:naryPr><m:sub><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:sub><m:sup><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:sup><m:e /></m:nary></m:e></m:d></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,Cases)
{
	std::wstring wsString = L"cases{ 1& 2# 3& 4}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:d><m:dPr><m:begChr m:val=\"{\" /><m:endChr m:val=\"\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:dPr><m:e><m:m><m:mPr><m:mcs><m:mc><m:mcPr><m:count m:val=\"1\" /><m:mcJc m:val=\"center\" /></m:mcPr></m:mc></m:mcs><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:mPr><m:mr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:e><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:e></m:mr><m:mr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>3</m:t></m:r></m:e><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>4</m:t></m:r></m:e></m:mr></m:m></m:e></m:d></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,Lline)
{
	std::wstring wsString = L"LEFT | 1 RIGHT | 2 + 3";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:d><m:dPr><m:begChr m:val=\"|\" /><m:endChr m:val=\"|\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:dPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:e></m:d><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>+</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>3</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,Arrow)
{
	std::wstring wsString = L"larrow UPARROW NWARROW MAPSTO DLINE";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>←</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>⇑</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>↖</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>↦</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>‖</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,Overbrace)
{
	std::wstring wsString = L"OVERBRACE { 1} { 2}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:limUpp><m:limUppPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limUppPr><m:e><m:groupChr><m:groupChrPr><m:chr m:val=\"⏞\" /><m:pos m:val=\"top\" /><m:vertJc m:val=\"bot\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:groupChrPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:e></m:groupChr></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:lim></m:limUpp></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,Underbrace)
{
	std::wstring wsString = L"UNDERBRACE { 5} {10 }";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:limLow><m:limLowPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:limLowPr><m:e><m:groupChr><m:groupChrPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:groupChrPr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>10</m:t></m:r></m:e></m:groupChr></m:e><m:lim><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>5</m:t></m:r></m:lim></m:limLow></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,Pile)
{
	std::wstring wsString = L"pile{  1 over  2 # 2}";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:m><m:mPr><m:mcs><m:mc><m:mcPr><m:count m:val=\"1\" /><m:mcJc m:val=\"center\" /></m:mcPr></m:mc></m:mcs><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:mPr><m:mr><m:e><m:f><m:fPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:fPr><m:num><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:num><m:den><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:den></m:f></m:e></m:mr><m:mr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:e></m:mr></m:m></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,Pmatrix)
{
	std::wstring wsString = L"pmatrix { 1& 2#3 &4 }";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:d><m:dPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:dPr><m:e><m:m><m:mPr><m:mcs><m:mc><m:mcPr><m:count m:val=\"1\" /><m:mcJc m:val=\"center\" /></m:mcPr></m:mc></m:mcs><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:mPr><m:mr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:e><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:e></m:mr><m:mr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>3</m:t></m:r></m:e><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>4</m:t></m:r></m:e></m:mr></m:m></m:e></m:d></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,Bmatrix)
{
	std::wstring wsString = L"bmatrix { 1&2 &3 # 4& 6&5 # 7& 8&9 }";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:d><m:dPr><m:begChr m:val=\"[\" /><m:endChr m:val=\"]\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:dPr><m:e><m:m><m:mPr><m:mcs><m:mc><m:mcPr><m:count m:val=\"1\" /><m:mcJc m:val=\"center\" /></m:mcPr></m:mc></m:mcs><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr></m:ctrlPr></m:mPr><m:mr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>1</m:t></m:r></m:e><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>2</m:t></m:r></m:e><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>3</m:t></m:r></m:e></m:mr><m:mr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>4</m:t></m:r></m:e><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>6</m:t></m:r></m:e><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>5</m:t></m:r></m:e></m:mr><m:mr><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>7</m:t></m:r></m:e><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>8</m:t></m:r></m:e><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>9</m:t></m:r></m:e></m:mr></m:m></m:e></m:d></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,GreekSymbols)
{
	std::wstring wsString = L"ALPHA ETA NU TAU PSI";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>Α</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>Η</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>Ν</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>Τ</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>Ψ</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,GreekSymbolsSmall)
{
	std::wstring wsString = L"epsilon iota nu rho phi";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>ε</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>ι</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>ν</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>ρ</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>φ</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

TEST(SMConvectorTest,MathOperatorSymbols)
{
	std::wstring wsString = L"+- DEG BECAUSE REIMAGE ASYMP FORALL LNOT DAGGER";
	StarMath::CStarMathConverter oConverterStarMath;
	std::wstring wsXmlString = L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\" /></m:oMathParaPr><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>±</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>°</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>∵</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>≓</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>≍</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>∀</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>¬</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"30\" /><w:szCs w:val=\"30\" /></w:rPr><m:t>†</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
}

// TEST(SMConvectorTest,AttributeMatrix)
// {
// 	std::wstring wsString = L"";
// 	StarMath::CStarMathConverter oConverterStarMath;
//
//
// 	std::wstring wsXmlString = L"";
// 	EXPECT_EQ(oConverterStarMath.ConvertEQNToOOXml(wsString),wsXmlString);
// }

