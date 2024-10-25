#include "gtest/gtest.h"
#include "../../../OdfFile/Reader/Converter/StarMath2OOXML/cooxml2odf.h"
#include "../../../DesktopEditor/xml/include/xmlutils.h"
#include "SmartArt.h"

struct StMath
{
	StMath(): m_wsSemantic(L""),m_wsAnnotation(L"")
	{}
	std::wstring m_wsSemantic;
	std::wstring m_wsAnnotation;
	bool operator==(const StMath& stExample) const 
	{
		return(this->m_wsSemantic == stExample.m_wsSemantic) && (this->m_wsAnnotation == stExample.m_wsAnnotation);
	}
};

TEST(OOXml2OdfTest,MathPara)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::COMathPara();
	if(oReader.FromString(L"<m:oMathPara><m:oMathParaPr><m:jc m:val=\"center\"/></m:oMathParaPr><m:oMath><m:r><w:rPr><w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/><w:lang w:val=\"en-US\"/></w:rPr><m:t>1</m:t></m:r></m:oMath></m:oMathPara>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.StartConversion(pElement);
	std::wstring wsOdf = L"<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\"block\"><semantics><mn>1</mn><annotation encoding=\"StarMath 5.0\">1 </annotation></semantics></math>";
	EXPECT_EQ(oTest.GetOdf(),wsOdf);
}
TEST(OOXml2OdfTest,Math)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::COMath();
	if(oReader.FromString(L"<m:oMath><m:r><w:rPr><w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/><w:lang w:val=\"en-US\"/></w:rPr><m:t>1</m:t></m:r></m:oMath>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath stCorrect,stResult;
	stResult.m_wsSemantic = oTest.GetOdf();
	stResult.m_wsAnnotation = oTest.GetAnnotation();
	stCorrect.m_wsSemantic = L"<mn>1</mn>";
	stCorrect.m_wsAnnotation = L"1 ";
	EXPECT_EQ(stResult,stCorrect);
}
TEST(OOXml2OdfTest,MText)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::COMath* pElement = new OOX::Logic::COMath();
	if(oReader.FromString(L"<m:oMath><m:r><w:rPr><w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/><w:lang w:val=\"en-US\"/></w:rPr><m:t>10</m:t></m:r></m:oMath>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.ConversionVectorWritingElement(pElement->m_arrItems);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mn>10</mn>";
	StCorrect.m_wsAnnotation = L"10 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,MTextHard)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::COMath* pElement = new OOX::Logic::COMath();
	if(oReader.FromString(L"<m:oMath> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>20 2+3</m:t> </m:r> </m:oMath>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.ConversionVectorWritingElement(pElement->m_arrItems);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mn>20</mn><mspace /><mrow><mn>2</mn><mo stretchy=\"false\">+</mo><mn>3</mn></mrow>";
	StCorrect.m_wsAnnotation = L"20 ` 2 + 3 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,MTextHardAttribute)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::COMath* pElement = new OOX::Logic::COMath();
	if(oReader.FromString(L"<m:oMath> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:color w:val=\"FF0000\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>20</m:t> </m:r> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t xml:space=\"preserve\"> </m:t> </m:r> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:color w:val=\"ED7D31\" w:themeColor=\"accent2\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:color w:val=\"70AD47\" w:themeColor=\"accent6\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>-</m:t> </m:r> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:color w:val=\"1F4E79\" w:themeColor=\"accent5\" w:themeShade=\"80\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>3</m:t> </m:r> </m:oMath>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.ConversionVectorWritingElement(pElement->m_arrItems);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mstyle mathcolor=\"#FF0000\"><mn>20</mn></mstyle><mspace /><mstyle mathcolor=\"#ED7D31\"><mn>2</mn></mstyle><mrow><mstyle mathcolor=\"#70AD47\"><mo stretchy=\"false\">-</mo><mstyle mathcolor=\"#1F4E79\"><mn>3</mn></mstyle></mstyle></mrow>";
	StCorrect.m_wsAnnotation = L"color hex FF0000 { 20 } ` color hex ED7D31 2 color hex 70AD47 - color hex 1F4E79 3 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,FractionOver)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::CFraction* pElement = new OOX::Logic::CFraction();
	if(oReader.FromString(L"<m:f> <m:fPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:fPr> <m:num> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:num> <m:den> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:den> </m:f>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mfrac><mn>1</mn><mn>2</mn></mfrac>";
	StCorrect.m_wsAnnotation = L"{ 1 } over { 2 } ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,FractionWideslash)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::CFraction* pElement = new OOX::Logic::CFraction();
	if(oReader.FromString(L"<m:f> <m:fPr> <m:type m:val=\"skw\"/> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:fPr> <m:num> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:num> <m:den> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:den> </m:f>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mfrac bevelled=\"true\" /><mn>1</mn><mn>2</mn></mfrac>";
	StCorrect.m_wsAnnotation = L"{ 1 } wideslash { 2 } ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,FractionSlash)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::CFraction* pElement = new OOX::Logic::CFraction();
	if(oReader.FromString(L"<m:f> <m:fPr> <m:type m:val=\"lin\"/> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:fPr> <m:num> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:num> <m:den> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:den> </m:f>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mrow><mn>1</mn><mo stretchy=\"false\">/</mo><mn>2</mn></mrow>";
	StCorrect.m_wsAnnotation = L"1 / 2 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,Delimiter)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::CDelimiter* pElement = new OOX::Logic::CDelimiter();
	if(oReader.FromString(L"<m:d> <m:dPr> <m:begChr m:val=\"[\"/> <m:endChr m:val=\"]\"/> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:dPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2-3</m:t> </m:r> </m:e> </m:d>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mrow><mo fence=\"true\" form=\"prefix\" stretchy=\"true\" />[<mrow><mrow><mn>2</mn><mo stretchy=\"false\">-</mo><mn>3</mn></mrow></mrow><mo fence=\"true\" form=\"postfix\" stretchy=\"true\" />]</mrow>";
	StCorrect.m_wsAnnotation = L"left [ 2 - 3 right ] ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,NaryFromTo)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::CNary* pElement = new OOX::Logic::CNary();
	if(oReader.FromString(L"<m:nary> <m:naryPr> <m:chr m:val=\"\u2211\"/> <m:limLoc m:val=\"undOvr\"/> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:naryPr> <m:sub> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:sub> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:sup> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>3</m:t> </m:r> </m:e> </m:nary>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<munderover><mo stretchy=\"false\">\u2211</mo><mn>2</mn><mn>1</mn></munderover><mn>3</mn>";
	StCorrect.m_wsAnnotation = L"sum from {2 } to {1 } 3 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,NaryFrom)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::CNary* pElement = new OOX::Logic::CNary();
	if(oReader.FromString(L"<m:nary> <m:naryPr> <m:chr m:val=\"\u2210\"/> <m:limLoc m:val=\"undOvr\"/> <m:supHide m:val=\"1\"/> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:naryPr> <m:sub> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:sub> <m:sup/> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:e> </m:nary>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<munder><mo stretchy=\"false\">\u2210</mo><mn>1</mn></munder><mn>2</mn>";
	StCorrect.m_wsAnnotation = L"coprod from {1 } 2 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,NaryOper)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::CNary* pElement = new OOX::Logic::CNary();
	if(oReader.FromString(L"<m:nary> <m:naryPr> <m:chr m:val=\"\u22C0\"/> <m:limLoc m:val=\"undOvr\"/> <m:subHide m:val=\"1\"/> <m:supHide m:val=\"1\"/> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:naryPr> <m:sub/> <m:sup/> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>10</m:t> </m:r> </m:e> </m:nary>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mrow><mo stretchy=\"false\">\u22C0</mo></mrow><mn>10</mn>";
	StCorrect.m_wsAnnotation = L"oper \u22C0 10 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,AccDdot)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::CAcc* pElement = new OOX::Logic::CAcc();
	if(oReader.FromString(L"<m:acc> <m:accPr> <m:chr m:val=\"\u0308\"/> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:accPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:e> </m:acc> ") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mover accent=\"true\"><mn>1</mn><mo stretchy=\"false\">\u0308</mo></mover>";
	StCorrect.m_wsAnnotation = L"ddot 1 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,Matrix)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::CMatrix* pElement = new OOX::Logic::CMatrix();
	if(oReader.FromString(L"<m:m> <m:mPr> <m:mcs> <m:mc> <m:mcPr> <m:count m:val=\"2\"/> <m:mcJc m:val=\"center\"/> </m:mcPr> </m:mc> </m:mcs> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:mPr> <m:mr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:e> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:e> </m:mr> <m:mr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>3</m:t> </m:r> </m:e> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>4</m:t> </m:r> </m:e> </m:mr> </m:m>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mtable><mtr><mtd><mn>1</mn></mtd><mtd><mn>2</mn></mtd></mtr><mtr><mtd><mn>3</mn></mtd><mtd><mn>4</mn></mtd></mtr></mtable>";
	StCorrect.m_wsAnnotation = L"matrix{1 # 2 ## 3 # 4 } ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,MatrixWithOneColumn)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::CMatrix* pElement = new OOX::Logic::CMatrix();
	if(oReader.FromString(L"<m:m> <m:mPr> <m:mcs> <m:mc> <m:mcPr> <m:count m:val=\"1\"/> <m:mcJc m:val=\"center\"/> </m:mcPr> </m:mc> </m:mcs> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:mPr> <m:mr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:e> </m:mr> <m:mr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:e> </m:mr> <m:mr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>3</m:t> </m:r> </m:e> </m:mr> </m:m>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mtable><mtr><mtd><mn>1</mn></mtd></mtr><mtr><mtd><mn>2</mn></mtd></mtr><mtr><mtd><mn>3</mn></mtd></mtr></mtable>";
	StCorrect.m_wsAnnotation = L"matrix{1 ## 2 ## 3 } ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,Box)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::Logic::CBox* pElement = new OOX::Logic::CBox();
	if(oReader.FromString(L"<m:box> <m:boxPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:boxPr> <m:e> <m:argPr> <m:argSz m:val=\"-1\"/> </m:argPr> <m:f> <m:fPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:fPr> <m:num> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:num> <m:den> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:den> </m:f> </m:e> </m:box>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mfrac><mn>1</mn><mn>2</mn></mfrac>";
	StCorrect.m_wsAnnotation = L"{ 1 } over { 2 } ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,sSup)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::CSSup();
	if(oReader.FromString(L"<m:sSup> <m:sSupPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> </w:rPr> </m:ctrlPr> </m:sSupPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:e> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:sup> </m:sSup>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<msup><mn>1</mn><mn>2</mn></msup>";
	StCorrect.m_wsAnnotation = L"1 ^ {2 } ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,sSub)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::CSSub();
	if(oReader.FromString(L"<m:sSub> <m:sSubPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> </w:rPr> </m:ctrlPr> </m:sSubPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> </w:rPr> <m:t>3</m:t> </m:r> </m:e> <m:sub> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> </w:rPr> <m:t>4</m:t> </m:r> </m:sub> </m:sSub>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<msub><mn>3</mn><mn>4</mn></msub>";
	StCorrect.m_wsAnnotation = L"3 _ {4 } ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,sSubSup)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::CSSubSup();
	if(oReader.FromString(L"<m:sSubSup> <m:sSubSupPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> </w:rPr> </m:ctrlPr> </m:sSubSupPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> </w:rPr> <m:t>5</m:t> </m:r> </m:e> <m:sub> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> </w:rPr> <m:t>7</m:t> </m:r> </m:sub> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> </w:rPr> <m:t>6</m:t> </m:r> </m:sup> </m:sSubSup>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<msubsup><mn>5</mn><mn>7</mn><mn>6</mn></msubsup>";
	StCorrect.m_wsAnnotation = L"5 _ 7 ^ 6 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,sPre)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::CSPre();
	if(oReader.FromString(L"<m:sPre> <m:sPrePr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsiaTheme=\"minorEastAsia\" w:hAnsi=\"Cambria Math\"/> <w:i/> </w:rPr> </m:ctrlPr> </m:sPrePr> <m:sub> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsiaTheme=\"minorEastAsia\" w:hAnsi=\"Cambria Math\"/> </w:rPr> <m:t>8</m:t> </m:r> </m:sub> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsiaTheme=\"minorEastAsia\" w:hAnsi=\"Cambria Math\"/> </w:rPr> <m:t>9</m:t> </m:r> </m:sup> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsiaTheme=\"minorEastAsia\" w:hAnsi=\"Cambria Math\"/> </w:rPr> <m:t>1</m:t> </m:r> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsiaTheme=\"minorEastAsia\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>0</m:t> </m:r> </m:e> </m:sPre>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mmultiscripts><mn>10</mn><mprescripts /><mn>9</mn><mn>8</mn></mmultiscripts>";
	StCorrect.m_wsAnnotation = L"10 lsup 9 lsub 8 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,EqArr)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::CEqArr();
	if(oReader.FromString(L"<m:eqArr> <m:eqArrPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:eqArrPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:e> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:e> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>3</m:t> </m:r> </m:e> </m:eqArr>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mtable><mtr><mtd><mn>1</mn></mtd></mtr><mtr><mtd><mn>2</mn></mtd></mtr><mtr><mtd><mn>3</mn></mtd></mtr></mtable>";
	StCorrect.m_wsAnnotation = L"stack{1 # 2 # 3 } ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,NumBinom)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::CFraction();
	if(oReader.FromString(L"<m:f> <m:fPr> <m:type m:val=\"noBar\"/> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:fPr> <m:num> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>4</m:t> </m:r> </m:num> <m:den> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>5</m:t> </m:r> </m:den> </m:f>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mtable><mtr><mtd><mn>4</mn></mtd></mtr><mtr><mtd><mn>5</mn></mtd></mtr></mtable>";
	StCorrect.m_wsAnnotation = L"binom { 4 } { 5 } ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,RelationsAndOperations)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::COMath();
	if(oReader.FromString(L"<m:oMath> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>\u225D</m:t> </m:r> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>3</m:t> </m:r> </m:oMath>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mrow><mrow><mn>2</mn><mo stretchy=\"false\">\u225D</mo><mn>3</mn></mrow></mrow>";
	StCorrect.m_wsAnnotation = L"2 def  3 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,Special)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::COMath();
	if(oReader.FromString(L"<m:oMath> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>\u2135</m:t> </m:r> </m:oMath>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mi mathvariant=\"normal\">\u2135</mi>";
	StCorrect.m_wsAnnotation = L"aleph ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,LimLow)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::CFunc();
	if(oReader.FromString(L"<m:func> <m:funcPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:funcPr> <m:fName> <m:limLow> <m:limLowPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:limLowPr> <m:e> <m:r> <m:rPr> <m:sty m:val=\"p\"/> </m:rPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>lim</m:t> </m:r> </m:e> <m:lim> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:lim> </m:limLow> </m:fName> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:e> </m:func>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mrow><munder><mi mathvariant=\"italic\">lim</mi><mn>1</mn></munder><mn>2</mn></mrow>";
	StCorrect.m_wsAnnotation = L"lim csub 1 2 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,Func)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::CFunc();
	if(oReader.FromString(L"<m:func> <m:funcPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:funcPr> <m:fName> <m:r> <m:rPr> <m:sty m:val=\"p\"/> </m:rPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>coth</m:t> </m:r> </m:fName> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>5</m:t> </m:r> </m:e> </m:func>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mrow><mi mathvariant=\"italic\">coth</mi><mn>5</mn></mrow>";
	StCorrect.m_wsAnnotation = L"coth 5 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,LimUpp)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::CLimUpp;
	if(oReader.FromString(L"<m:limUpp> <m:limUppPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:limUppPr> <m:e> <m:groupChr> <m:groupChrPr> <m:chr m:val=\"\u23DE\"/> <m:pos m:val=\"top\"/> <m:vertJc m:val=\"bot\"/> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:groupChrPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:e> </m:groupChr> </m:e> <m:lim> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:lim> </m:limUpp>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mover><mover><mn>1</mn><mo stretchy=\"false\">\u23DE</mo></mover><mn>2</mn></mover>";
	StCorrect.m_wsAnnotation = L"1 overbrace 2 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,LimLowWithGroup)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::CLimLow();
	if(oReader.FromString(L"<m:limLow> <m:limLowPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:limLowPr> <m:e> <m:groupChr> <m:groupChrPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:groupChrPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>3</m:t> </m:r> </m:e> </m:groupChr> </m:e> <m:lim> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>4</m:t> </m:r> </m:lim> </m:limLow>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.NodeDefinition(pElement);
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<munder><munder><mn>3</mn><mo stretchy=\"false\">\u23DF</mo></munder><mn>4</mn></munder>";
	StCorrect.m_wsAnnotation = L"3 underbrace 4 ";
	EXPECT_EQ(StResult,StCorrect);
}
TEST(OOXml2OdfTest,Example2)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::COMathPara();
	if(oReader.FromString(L"<m:oMathPara> <m:oMath> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>x</m:t> </m:r> <m:r> <m:rPr> <m:sty m:val=\"p\"/> </m:rPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>=</m:t> </m:r> <m:f> <m:fPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:fPr> <m:num> <m:r> <m:rPr> <m:sty m:val=\"p\"/> </m:rPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>-</m:t> </m:r> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>b</m:t> </m:r> <m:r> <m:rPr> <m:sty m:val=\"p\"/> </m:rPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>\u00B1</m:t> </m:r> <m:rad> <m:radPr> <m:degHide m:val=\"1\"/> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:radPr> <m:deg/> <m:e> <m:sSup> <m:sSupPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:sSupPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>b</m:t> </m:r> </m:e> <m:sup> <m:r> <m:rPr> <m:sty m:val=\"p\"/> </m:rPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:sup> </m:sSup> <m:r> <m:rPr> <m:sty m:val=\"p\"/> </m:rPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>-4</m:t> </m:r> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>ac</m:t> </m:r> </m:e> </m:rad> </m:num> <m:den> <m:r> <m:rPr> <m:sty m:val=\"p\"/> </m:rPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>a</m:t> </m:r> </m:den> </m:f> </m:oMath> </m:oMathPara>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.StartConversion(pElement);
	std::wstring wsOdf = L"<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\"block\"><semantics><mrow><mrow><mi mathvariant=\"italic\">x</mi><mo stretchy=\"false\">=</mo></mrow><mfrac><mrow><mrow><mo stretchy=\"false\">-</mo><mi mathvariant=\"italic\">b</mi></mrow><mo stretchy=\"false\">\u00B1</mo></mrow><sqrt><msup><mi mathvariant=\"italic\">b</mi><mn>2</mn></msup><mrow><mo stretchy=\"false\">-</mo><mn>4</mn></mrow><mi mathvariant=\"italic\">ac</mi></sqrt><mn>2a</mn></mfrac></mrow><annotation encoding=\"StarMath 5.0\">x =  { - b +- sqrt { b ^ {2 } - 4 ac } } over { 2a } </annotation></semantics></math>";
	EXPECT_EQ(oTest.GetOdf(),wsOdf);
}
//TEST(OOXml2OdfTest,Example5)
//{
//	XmlUtils::CXmlLiteReader oReader;
//	OOX::WritingElement* pElement = new OOX::Logic::COMathPara();
//	if(oReader.FromString(L"<m:oMathPara> <m:oMath> <m:sSup> <m:sSupPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:sSupPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>e</m:t> </m:r> </m:e> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>x</m:t> </m:r> </m:sup> </m:sSup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>=1+</m:t> </m:r> <m:f> <m:fPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:fPr> <m:num> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>x</m:t> </m:r> </m:num> <m:den> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>1\u0021</m:t> </m:r> </m:den> </m:f> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>+</m:t> </m:r> <m:f> <m:fPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:fPr> <m:num> <m:sSup> <m:sSupPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:sSupPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>x</m:t> </m:r> </m:e> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:sup> </m:sSup> </m:num> <m:den> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2\u0021</m:t> </m:r> </m:den> </m:f> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>+</m:t> </m:r> <m:f> <m:fPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:fPr> <m:num> <m:sSup> <m:sSupPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:sSupPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>x</m:t> </m:r> </m:e> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>3</m:t> </m:r> </m:sup> </m:sSup> </m:num> <m:den> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>3\u0021</m:t> </m:r> </m:den> </m:f> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>+\u2026,</m:t> </m:r> <m:r> <m:rPr> <m:sty m:val=\"p\"/> </m:rPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t xml:space=\"preserve\"> </m:t> </m:r> <m:r> <m:rPr> <m:sty m:val=\"p\"/> </m:rPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t xml:space=\"preserve\"> </m:t> </m:r> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>-\u221E\u003Cx\u003C\u221E</m:t> </m:r> </m:oMath> </m:oMathPara> ") && oReader.ReadNextNode())
//		pElement->fromXML(oReader);
//	StarMath::COOXml2Odf oTest;
//	oTest.StartConversion(pElement);
//	std::wstring wsOdf = L"<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\"block\"><semantics><mrow><msup><mi mathvariant=\"italic\">e</mi><mi mathvariant=\"italic\">x</mi></msup><mrow><mrow><mo stretchy=\"false\">=</mo><mn>1</mn></mrow><mo stretchy=\"false\">+</mo></mrow><mfrac><mi mathvariant=\"italic\">x</mi><mn>1</mn><mtext>\u0021</mtext></mfrac><mrow><mo stretchy=\"false\">+</mo></mrow><mfrac><msup><mi mathvariant=\"italic\">x</mi><mn>2</mn></msup><mn>2</mn><mtext>\u0021</mtext></mfrac><mrow><mo stretchy=\"false\">+</mo></mrow><mfrac><msup><mi mathvariant=\"italic\">x</mi><mn>3</mn></msup><mn>3</mn><mtext>\u0021</mtext></mfrac><mrow><mo stretchy=\"false\">+</mo><mi mathvariant=\"normal\">\u2026</mi></mrow><mtext>,</mtext><mspace /><mspace /><mrow><mrow><mrow><mo stretchy=\"false\">-</mo><mi mathvariant=\"normal\">\u221E</mi></mrow><mo stretchy=\"false\">&lt;</mo><mi mathvariant=\"italic\">x</mi></mrow><mo stretchy=\"false\">&lt;</mo><mi mathvariant=\"normal\">\221E</mi></mrow></mrow><annotation encoding=\"StarMath 5.0\">e ^ {x } =  1 + { x } over { 1 &quot;\u0021&quot; } + { x ^ {2 } } over { 2 &quot;\u0021&quot; } + { x ^ {3 } } over { 3 &quot;\u0021&quot; } + dotslow &quot;,&quot; ` ` - infinity &lt; x &lt; infinity </annotation></semantics></math>";
//	EXPECT_EQ(oTest.GetOdf(),wsOdf);
//}
TEST(OOXml2OdfTest,Example1)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::COMathPara();
	if(oReader.FromString(L"<m:oMathPara> <m:oMath> <m:sSup> <m:sSupPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:sSupPr> <m:e> <m:d> <m:dPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:dPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>x+a</m:t> </m:r> </m:e> </m:d> </m:e> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>n</m:t> </m:r> </m:sup> </m:sSup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsia=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>=</m:t> </m:r> <m:nary> <m:naryPr> <m:chr m:val=\"\u2211\"/> <m:grow m:val=\"1\"/> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:naryPr> <m:sub> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsia=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>k=0</m:t> </m:r> </m:sub> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsia=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>n</m:t> </m:r> </m:sup> <m:e> <m:d> <m:dPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:dPr> <m:e> <m:f> <m:fPr> <m:type m:val=\"noBar\"/> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:fPr> <m:num> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsia=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>n</m:t> </m:r> </m:num> <m:den> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsia=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>k</m:t> </m:r> </m:den> </m:f> </m:e> </m:d> <m:sSup> <m:sSupPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:sSupPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsia=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>x</m:t> </m:r> </m:e> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsia=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>k</m:t> </m:r> </m:sup> </m:sSup> <m:sSup> <m:sSupPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:sSupPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsia=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>a</m:t> </m:r> </m:e> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:eastAsia=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>n-k</m:t> </m:r> </m:sup> </m:sSup> </m:e> </m:nary> </m:oMath> </m:oMathPara>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.StartConversion(pElement);
	std::wstring wsOdf = L"<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\"block\"><semantics><mrow><msup><mrow><mo fence=\"true\" form=\"prefix\" stretchy=\"true\" />(<mrow><mrow><mi mathvariant=\"italic\">x</mi><mo stretchy=\"false\">+</mo><mi mathvariant=\"italic\">a</mi></mrow></mrow><mo fence=\"true\" form=\"postfix\" stretchy=\"true\" />)</mrow><mi mathvariant=\"italic\">n</mi></msup><mrow><mo stretchy=\"false\">=</mo></mrow><munderover><mo stretchy=\"false\">\u2211</mo><mrow><mi mathvariant=\"italic\">k</mi><mo stretchy=\"false\">=</mo><mn>0</mn></mrow><mi mathvariant=\"italic\">n</mi></munderover><mrow><mo fence=\"true\" form=\"prefix\" stretchy=\"true\" />(<mrow><mtable><mtr><mtd><mi mathvariant=\"italic\">n</mi></mtd></mtr><mtr><mtd><mi mathvariant=\"italic\">k</mi></mtd></mtr></mtable></mrow><mo fence=\"true\" form=\"postfix\" stretchy=\"true\" />)</mrow><msup><mi mathvariant=\"italic\">x</mi><mi mathvariant=\"italic\">k</mi></msup><msup><mi mathvariant=\"italic\">a</mi><mrow><mi mathvariant=\"italic\">n</mi><mo stretchy=\"false\">-</mo><mi mathvariant=\"italic\">k</mi></mrow></msup></mrow><annotation encoding=\"StarMath 5.0\">left ( x + a right ) ^ {n } =  sum from {k =  0 } to {n } left ( binom { n } { k } right ) x ^ {k } a ^ {n - k } </annotation></semantics></math>";
	EXPECT_EQ(oTest.GetOdf(),wsOdf);
}
TEST(OOXml2OdfTest,Example3)
{
	XmlUtils::CXmlLiteReader oReader;
	OOX::WritingElement* pElement = new OOX::Logic::COMathPara();
	if(oReader.FromString(L"<m:oMathPara> <m:oMath> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>A=\u03C0</m:t> </m:r> <m:sSup> <m:sSupPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> </m:ctrlPr> </m:sSupPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>r</m:t> </m:r> </m:e> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:lang w:val=\"en-US\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:sup> </m:sSup> </m:oMath> </m:oMathPara>") && oReader.ReadNextNode())
		pElement->fromXML(oReader);
	StarMath::COOXml2Odf oTest;
	oTest.StartConversion(pElement);
	std::wstring wsOdf = L"<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\"block\"><semantics><mrow><mrow><mi mathvariant=\"italic\">A</mi><mo stretchy=\"false\">=</mo><mi mathvariant=\"normal\">\u03C0</mi></mrow><msup><mi mathvariant=\"italic\">r</mi><mn>2</mn></msup></mrow><annotation encoding=\"StarMath 5.0\">A =  %pi r ^ {2 } </annotation></semantics></math>";
	EXPECT_EQ(oTest.GetOdf(),wsOdf);
}
//TEST(OOXml2OdfTest,MathTemp)
//{
//	XmlUtils::CXmlLiteReader oReader;
//	OOX::WritingElement* pElement = new OOX::Logic::COMathPara();
//	if(oReader.FromString(L"<m:oMathPara> <m:oMath> <m:sSup> <m:sSupPr> <m:ctrlPr> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> <w:i/> </w:rPr> </m:ctrlPr> </m:sSupPr> <m:e> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> </w:rPr> <m:t>1</m:t> </m:r> </m:e> <m:sup> <m:r> <w:rPr> <w:rFonts w:ascii=\"Cambria Math\" w:hAnsi=\"Cambria Math\"/> </w:rPr> <m:t>2</m:t> </m:r> </m:sup> </m:sSup> </m:oMath> </m:oMathPara>") && oReader.ReadNextNode())
//		pElement->fromXML(oReader);
//	StarMath::COOXml2Odf oTest;
//	oTest.StartConversion(pElement);
//	NSFile::CFileBinary oOriginal;
//	oOriginal.CreateFile(L"X:/Rabotka/Original.txt");
//	oOriginal.WriteStringUTF8(oTest.GetOdf());
//	oOriginal.WriteStringUTF8(oTest.GetAnnotation());
//	StMath StCorrect,StResult;
//	StResult.m_wsSemantic = oTest.GetOdf();
//	StResult.m_wsAnnotation = oTest.GetAnnotation();
//	StCorrect.m_wsSemantic = L"<mrow><mn>1</mn><mo stretchy=\"false\">/</mo><mn>2</mn></mrow>";
//	StCorrect.m_wsAnnotation = L"{1} / {2} ";
//	EXPECT_EQ(StResult,StCorrect);
//}
//TEST(OOXml2OdfTest,Math)
//{
//	XmlUtils::CXmlLiteReader oReader;
//	OOX::WritingElement* pElement = new OOX::Logic::COMath();
//	if(oReader.FromString(L"") && oReader.ReadNextNode())
//		pElement->fromXML(oReader);
//	StarMath::COOXml2Odf oTest;
//	oTest.NodeDefinition(pElement);
//	StMath StCorrect,StResult;
//	StResult.m_wsSemantic = oTest.GetOdf();
//	StResult.m_wsAnnotation = oTest.GetAnnotation();
//	StCorrect.m_wsSemantic = L"<mrow><mn>1</mn><mo stretchy=\"false\">/</mo><mn>2</mn></mrow>";
//	StCorrect.m_wsAnnotation = L"{1} / {2} ";
//	EXPECT_EQ(StResult,StCorrect);
//}
