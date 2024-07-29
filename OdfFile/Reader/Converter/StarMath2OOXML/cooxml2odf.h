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
		StValuePr():wsTypeName(L""),wsBegBracket(L""),wsEndBracket(L""),wsChr(L""),bSupHide(false),bSubHide(false)
		{}
		std::wstring wsTypeName,wsChr;
		std::wstring wsBegBracket,wsEndBracket;
		bool bSupHide,bSubHide;
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
		void ConversionNary(OOX::Logic::CNary* pNary);
		void ConversionSsub(OOX::Logic::CSSub* pSsub);
		void ConversionSub(OOX::Logic::CSub* pSub, OOX::Logic::CElement *pElement);
		void ConversionSub(OOX::Logic::CSub* pSub);
		void ConversionSsup(OOX::Logic::CSSup* pSsup);
		void ConversionSup(OOX::Logic::CSup* pSup,OOX::Logic::CElement* pElement);
		void ConversionSup(OOX::Logic::CSup* pSup);
		void ConversionElement(OOX::Logic::CElement* pElement);
		void ConversionMRun(OOX::Logic::CMRun* pMRun);
		void ConversionAcc(OOX::Logic::CAcc* pAcc);
		void ConversionFunc(OOX::Logic::CFunc* pFunc);
		StValuePr ConversionMdPr(OOX::Logic::CDelimiterPr* pDelPr);
		StValuePr ConversionNaryPr(OOX::Logic::CNaryPr* pNaryPr);
		std::wstring ConversionBegBracket(OOX::Logic::CBegChr* pBegChr);
		std::wstring ConversionEndBracket(OOX::Logic::CEndChr* pEndChr);
		std::wstring ParsingText(std::wstring::iterator &itStart, std::wstring::iterator &itEnd);
		std::wstring ConversionChr(OOX::Logic::CChr* pChr);
		std::wstring ToStringChr(const std::wstring& wsChr);
		bool IsDigit(const std::wstring& wsDigit);
		bool IsAlpha(const std::wstring& wsAlpha);
		bool IsSpecialSymbol(const std::wstring& wsSpecial);
		void EndOdf();
	private:
		XmlUtils::CXmlWriter* m_pXmlWrite;
		std::wstring m_wsAnnotationStarMath;
	};
}
#endif // COOXML2ODF_H
