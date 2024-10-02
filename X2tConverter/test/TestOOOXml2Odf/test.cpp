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
	std::wstring wsOdf = L"<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\"block\"><semantics><mn>1</mn><annotation encoding=\"StarMath 5.0\">{1} </annotation></semantics></math>";
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
	stCorrect.m_wsAnnotation = L"{1} ";
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
	StCorrect.m_wsAnnotation = L"{10} ";
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
	StCorrect.m_wsSemantic = L"<mn>20</mn><mrow><mn>2</mn><mo stretchy=\"false\">+</mo><mn>3</mn></mrow>";
	StCorrect.m_wsAnnotation = L"{20} {2} + {3} ";
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
	StCorrect.m_wsAnnotation = L"color hex FF0000 {20} ` color hex ED7D31 {2} color hex 70AD47 - color hex 1F4E79 {3} ";
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
	StCorrect.m_wsAnnotation = L"{1} over {2} ";
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
	StCorrect.m_wsAnnotation = L"{1} wideslash {2} ";
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
	NSFile::CFileBinary oOriginal;
	oOriginal.CreateFile(L"X:/Rabotka/Original.txt");
	oOriginal.WriteStringUTF8(oTest.GetOdf());
	oOriginal.WriteStringUTF8(oTest.GetAnnotation());
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mrow><mn>1</mn><mo stretchy=\"false\">/</mo><mn>2</mn></mrow>";
	StCorrect.m_wsAnnotation = L"{1} / {2} ";
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
	StCorrect.m_wsSemantic = L"<mrow><mo fence=\"true\" form=\"prefix\" stretchy=\"false\" />[<mrow><mn>2</mn><mo stretchy=\"false\">-</mo><mn>3</mn></mrow><mo fence=\"true\" form=\"prefix\" stretchy=\"false\" />]</mrow>";
	StCorrect.m_wsAnnotation = L"[{2} - {3} ]";
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
	StCorrect.m_wsAnnotation = L"sum from {{2} } to {{1} } {3} ";
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
	StCorrect.m_wsAnnotation = L"coprod from {{1} } {2} ";
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
	NSFile::CFileBinary oOriginal;
	oOriginal.CreateFile(L"X:/Rabotka/Original.txt");
	oOriginal.WriteStringUTF8(oTest.GetOdf());
	oOriginal.WriteStringUTF8(oTest.GetAnnotation());
	StMath StCorrect,StResult;
	StResult.m_wsSemantic = oTest.GetOdf();
	StResult.m_wsAnnotation = oTest.GetAnnotation();
	StCorrect.m_wsSemantic = L"<mrow><mo stretchy=\"false\">\u22C0</mo></mrow><mn>10</mn>";
	StCorrect.m_wsAnnotation = L"oper \u22C0 {10} ";
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
	StCorrect.m_wsAnnotation = L"ddot {1} ";
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
	StCorrect.m_wsAnnotation = L"matrix{{1} # {2} ## {3} # {4} } ";
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
	StCorrect.m_wsAnnotation = L"matrix{{1} ## {2} ## {3} } ";
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
	StCorrect.m_wsAnnotation = L"{1} over {2} ";
	EXPECT_EQ(StResult,StCorrect);
}
//TEST(OOXml2OdfTest,Math)
//{
//	XmlUtils::CXmlLiteReader oReader;
//	OOX::WritingElement* pElement = new OOX::Logic::COMath();
//	if(oReader.FromString(L"") && oReader.ReadNextNode())
//		pElement->fromXML(oReader);
//	StarMath::COOXml2Odf oTest;
//	oTest.NodeDefinition(pElement);
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
