#include "../cstarmathpars.h"
#include "../cconversionsmtoooxml.h"
#include "gtest/gtest.h"



TEST(SMConvectorTest, BinOperatorPlus)
{
	std::wstring wsBinOperator = L"2 + 3";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsBinOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>2</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>+3</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,BinOperatorOver)
{
	std::wstring wsBinOperator = L"2 over 3";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsBinOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:f><m:fPr><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:i/><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr></m:ctrlPr></m:fPr><m:num><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>2</m:t></m:r></m:num><m:den><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>3</m:t></m:r></m:den></m:f></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,BinOperatorCdot)
{
	std::wstring wsBinOperator = L"5 cdot 8";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsBinOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>5</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u00B78</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,BinOperatorTimes)
{
	std::wstring wsBinOperator = L"5 times 8";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsBinOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>5</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u00D78</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,BinOperatorMultipl)
{
	std::wstring wsBinOperator = L"4 * 2";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsBinOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>4</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>*2</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,BinOperatorDiv)
{
	std::wstring wsBinOperator = L"4div2";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsBinOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>4</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u00F72</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,BinOperatorDivision)
{
	std::wstring wsBinOperator = L"4/2";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsBinOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:f><m:fPr><m:type m:val=\"lin\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:i/><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr></m:ctrlPr></m:fPr><m:num><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>4</m:t></m:r></m:num><m:den><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>2</m:t></m:r></m:den></m:f></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,BinOperatorOplus)
{
	std::wstring wsBinOperator = L"226oplus179";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsBinOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>226</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2295179</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,BinOperatorOdot)
{
	std::wstring wsBinOperator = L"226 odot 179";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsBinOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>226</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2299179</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,BinOperatorOtimes)
{
	std::wstring wsBinOperator = L"226 otimes 179";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsBinOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>226</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2297179</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,OperatorSum)
{
	std::wstring wsOperator = L"sum 5";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:nary><m:naryPr><m:chr m:val=\"\u2211\" /><m:limLoc m:val=\"undOvr\" /><m:subHide m:val=\"1\" /><m:supHide m:val=\"1\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:i/></w:rPr></m:ctrlPr></m:naryPr><m:sub/><m:sup/><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>5</m:t></m:r></m:e></m:nary></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,OperatorSumFrom)
{
	std::wstring wsOperator = L"sum from 10 5";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:nary><m:naryPr><m:chr m:val=\"\u2211\" /><m:limLoc m:val=\"undOvr\" /><m:supHide m:val=\"1\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:i/></w:rPr></m:ctrlPr></m:naryPr><m:sub><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>10</m:t></m:r></m:sub><m:sup/><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>5</m:t></m:r></m:e></m:nary></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,OperatorSumTo)
{
	std::wstring wsOperator = L"sum to 10 5";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:nary><m:naryPr><m:chr m:val=\"\u2211\" /><m:limLoc m:val=\"undOvr\" /><m:subHide m:val=\"1\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:i/></w:rPr></m:ctrlPr></m:naryPr><m:sub/><m:sup><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>10</m:t></m:r></m:sup><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>5</m:t></m:r></m:e></m:nary></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}

TEST(SMConvectorTest,OperatorSumFromTo)
{
	std::wstring wsOperator = L"sum from 666 to 777 567";
	StarMath::CParserStarMathString m_oTempO;
	StarMath::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(m_oTempO.Parse(wsOperator));
	std::wstring XmlString = L"<m:oMathPara><m:oMath><m:nary><m:naryPr><m:chr m:val=\"\u2211\" /><m:limLoc m:val=\"undOvr\" /><m:ctrlPr><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:i/></w:rPr></m:ctrlPr></m:naryPr><m:sub><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>666</m:t></m:r></m:sub><m:sup><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>777</m:t></m:r></m:sup><m:e><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>567</m:t></m:r></m:e></m:nary></m:oMath></m:oMathPara>";
	EXPECT_EQ(m_oTest.GetOOXML(),XmlString);
}
