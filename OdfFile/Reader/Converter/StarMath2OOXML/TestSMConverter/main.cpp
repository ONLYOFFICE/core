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

TEST(SMConvectorTest,SetOperationUnion)
{
	std::wstring wsString = L"23 union 45";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>23</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u22C3</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>45</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationIntersection)
{
	std::wstring wsString = L"15 intersection 1234";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>15</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u22C2</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>1234</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationSetminus)
{
	std::wstring wsString = L"7 setminus 15745";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>7</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2216</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>15745</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationSetquotient)
{
	std::wstring wsString = L"91 setquotient 45";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>91</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2215</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>45</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationSubset)
{
	std::wstring wsString = L"1 subset 2";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>1</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2282</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>2</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationSubseteq)
{
	std::wstring wsString = L"77subseteq66";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>77</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2286</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>66</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationSupset)
{
	std::wstring wsString = L"11supset22";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>11</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2283</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>22</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationSupseteq)
{
	std::wstring wsString = L"1 supseteq 2";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>1</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2287</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>2</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationNsubset)
{
	std::wstring wsString = L"21nsubset2";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>21</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2284</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>2</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationNsubseteq)
{
	std::wstring wsString = L"782nsubseteq250";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>782</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2288</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>250</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationNsupset)
{
	std::wstring wsString = L"1nsupset2";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>1</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2285</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>2</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationNsupseteq)
{
	std::wstring wsString = L"1nsupseteq2";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>1</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2289</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>2</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationIn)
{
	std::wstring wsString = L"1 in 2";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>1</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2208</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>2</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationNotin)
{
	std::wstring wsString = L"3 notin 4";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>3</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2209</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>4</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,SetOperationOwns)
{
	std::wstring wsString = L"5owns6";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>5</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u220B</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>6</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionDlarrow)
{
	std::wstring wsString = L"7 dlarrow 8";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>7</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u21D0</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>8</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionDlrarrow)
{
	std::wstring wsString = L"9 dlrarrow 10";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>9</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u21D4</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>10</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionDrarrow)
{
	std::wstring wsString = L"11drarrow12";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>11</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u21D2</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>12</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionEquals)
{
	std::wstring wsString = L"13=14";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>13</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>=</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>14</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionNotequals)
{
	std::wstring wsString = L"15 <> 16";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>15</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2260</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>16</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionLearrow)
{
	std::wstring wsString = L"17<18";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>17</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t><</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>18</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionLearrowequals)
{
	std::wstring wsString = L"19<= 20";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>19</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2264</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>20</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionRiarrow)
{
	std::wstring wsString = L"21 >22";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>21</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>></m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>22</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionRiarrowequals)
{
	std::wstring wsString = L"23 >=24";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>23</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2265</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>24</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionDllearrow)
{
	std::wstring wsString = L"25 <<26";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>25</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u226A</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>26</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionDlriarrow)
{
	std::wstring wsString = L"27 >> 28";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>27</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u226B</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>28</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionApprox)
{
	std::wstring wsString = L"29 approx30";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>29</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2248</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>30</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionSim)
{
	std::wstring wsString = L"31sim 32";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>31</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u223C</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>32</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionSimeq)
{
	std::wstring wsString = L"33 simeq 34";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>33</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2243</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>34</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionEquiv)
{
	std::wstring wsString = L"35 equiv 36";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>35</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2261</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>36</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionProp)
{
	std::wstring wsString = L"37 prop 38";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>37</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u221D</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>38</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionParallel)
{
	std::wstring wsString = L"39 parallel 40";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>39</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2225</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>40</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionOrtho)
{
	std::wstring wsString = L"41ortho42";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>41</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u22A5</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>42</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionDivides)
{
	std::wstring wsString = L"43 divides 44";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>43</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2223</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>44</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionNdivides)
{
	std::wstring wsString = L"45 ndivides 46";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>45</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2224</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>46</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionToward)
{
	std::wstring wsString = L"47 toward 48";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>47</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2192</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>48</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionTransl)
{
	std::wstring wsString = L"49 transl 50";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>49</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u22B7</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>50</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionTransr)
{
	std::wstring wsString = L"51 transr 52";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>51</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u22B6</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>52</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionDef)
{
	std::wstring wsString = L"53def54";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>53</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u225D</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>54</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionPrec)
{
	std::wstring wsString = L"55prec56";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>55</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u227A</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>56</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionSucc)
{
	std::wstring wsString = L"57 succ 58";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>57</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u227B</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>58</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionPreccurlyeq)
{
	std::wstring wsString = L"59 preccurlyeq 60";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>59</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u227C</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>60</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionSucccurlyeq)
{
	std::wstring wsString = L"61 succcurlyeq 62";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>61</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u227D</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>62</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionPrecsim)
{
	std::wstring wsString = L"63 precsim 64";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>63</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u227E</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>64</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionSuccsim)
{
	std::wstring wsString = L"65 succsim 66";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>65</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u227F</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>66</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionNprec)
{
	std::wstring wsString = L"67nprec68";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>67</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2280</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>68</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}

TEST(SMConvectorTest,ConnectionNsucc)
{
	std::wstring wsString = L"69nsucc70";
	StarMath::CParserStarMathString oTemp;
	StarMath::CConversionSMtoOOXML oTest;
	oTest.StartConversion(oTemp.Parse(wsString));
	std::wstring wsXmlString = L"<m:oMathPara><m:oMath><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>69</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>\u2281</m:t></m:r><m:r><w:rPr><w:rFonts w:hAnsi=\"Cambria Math\" w:ascii=\"Cambria Math\" /><w:sz w:val=\"40\" /><w:szCs w:val=\"40\" /></w:rPr><m:t>70</m:t></m:r></m:oMath></m:oMathPara>";
	EXPECT_EQ(oTest.GetOOXML(),wsXmlString);
}
