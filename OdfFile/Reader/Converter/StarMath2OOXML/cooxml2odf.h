#ifndef COOXML2ODF_H
#define COOXML2ODF_H
#include"../../../Writer/Converter/Converter.h"
#include"../../../../OOXML/DocxFormat/Math/oMathPara.h"
#include "../../../../DesktopEditor/xml/include/xmlwriter.h"
#include "../../../../OOXML/Base/Unit.h"

namespace StarMath
{
	struct StValuePr
	{
		StValuePr():wsTypeName(L""),wsBegBracket(L""),wsEndBracket(L"")
		{}
		std::wstring wsTypeName;
		std::wstring wsBegBracket,wsEndBracket;
	};
	class COOXml2Odf
	{
	public:
		COOXml2Odf();
		~COOXml2Odf();
		void StartConversion(OOX::Logic::COMathPara* pMathPara);
		void NodeDefinition(OOX::WritingElement* pNode);
		void ConversionMath(OOX::Logic::COMath* pMath);
		void ConversionMT(OOX::Logic::CMText* pMt);
		void ConversionMF(OOX::Logic::CFraction* pMf);
		StValuePr ConversionCtrlPr(OOX::Logic::CCtrlPr* pCtrlPr);
		StValuePr ConversionFpr(OOX::Logic::CFPr* pFpr);
		std::wstring ConversionType(OOX::Logic::CType* pType);
		void ConversionMd(OOX::Logic::CDelimiter* pDel);
		StValuePr ConversionMdPr(OOX::Logic::CDelimiterPr* pDelPr);
		std::wstring ConversionBegBracket(OOX::Logic::CBegChr* pBegChr);
		std::wstring ConversionEndBracket(OOX::Logic::CEndChr* pEndChr);
		std::wstring ParsingText(std::wstring::iterator &itStart, std::wstring::iterator &itEnd,bool& bMrowClose,bool& bMrowOpen);
		void EndOdf();
	private:
		XmlUtils::CXmlWriter* m_pXmlWrite;
		std::wstring m_wsAnnotationStarMath;
	};
}
#endif // COOXML2ODF_H
