#ifndef COOXML2ODF_H
#define COOXML2ODF_H
#include"../../../Writer/Converter/Converter.h"
#include"../../../../OOXML/DocxFormat/Math/oMathPara.h"
#include "../../../../DesktopEditor/xml/include/xmlwriter.h"
#include "../../../../OOXML/Base/Unit.h"
#include "typeselements.h"
#include <vector>
#include <utility>
#include <cstring>

namespace StarMath
{
	struct StValuePr
	{
		StValuePr():m_wsTypeName(L""),m_wsBegBracket(L""),m_wsEndBracket(L""),m_wsChr(L""),m_wsColor(L""),m_bSupHide(false),m_bSubHide(false)
		{}
		std::wstring m_wsTypeName,m_wsChr;
		std::wstring m_wsBegBracket,m_wsEndBracket;
		std::wstring m_wsColor;
		bool m_bSupHide,m_bSubHide;
	};
	class COneElement;
	class COOXml2Odf
	{
	public:
		COOXml2Odf();
		~COOXml2Odf();
		void StartConversion(OOX::Logic::COMathPara* pMathPara);
		void NodeDefinition(OOX::WritingElement* pNode);
		void ConversionMath(OOX::Logic::COMath* pMath);
		std::vector<COneElement *> ConversionMT(OOX::Logic::CMText* pMt);
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
		std::vector<COneElement *> ConversionMRun(OOX::Logic::CMRun* pMRun);
		void ConversionAcc(OOX::Logic::CAcc* pAcc);
		void ConversionFunc(OOX::Logic::CFunc* pFunc);
		void ConversionBox(OOX::Logic::CBox* pBox);
		void ConversionTextVector(std::vector<COneElement*>& arLine,std::vector<COneElement*>& arNewLine);
		void ConversionVectorWritingElement(std::vector<OOX::WritingElement*> arWrElements);
		StValuePr *ConversionRunProperties(OOX::Logic::CRunProperty* pRPr);
		StValuePr ConversionMdPr(OOX::Logic::CDelimiterPr* pDelPr);
		StValuePr ConversionNaryPr(OOX::Logic::CNaryPr* pNaryPr);
		std::wstring ConversionBegBracket(OOX::Logic::CBegChr* pBegChr);
		std::wstring ConversionEndBracket(OOX::Logic::CEndChr* pEndChr);
		static std::wstring ParsingText(std::wstring::iterator &itStart, std::wstring::iterator &itEnd);
		std::wstring ConversionChr(OOX::Logic::CChr* pChr);
		std::wstring ToStringChr(const std::wstring& wsChr);
		static void RecordingMoNode(const std::wstring& wsSymbol,XmlUtils::CXmlWriter* pXmlWrite);
		static bool IsDigit(const std::wstring& wsDigit);
		static bool IsAlpha(const std::wstring& wsAlpha);
		static bool IsSpecialSymbol(const std::wstring& wsSpecial);
		void EndOdf();
	private:
		XmlUtils::CXmlWriter* m_pXmlWrite;
		std::wstring m_wsAnnotationStarMath;
	};
	class COneElement
	{
	public:
		COneElement();
		COneElement(std::wstring& wsOneElement);
		virtual ~COneElement();
		virtual void Parse(std::wstring::iterator& itStart,std::wstring::iterator& itEnd) = 0;
		virtual void Conversion(XmlUtils::CXmlWriter* pXmlWrite,std::wstring& wsAnnotation) = 0;
		static COneElement* CreateElement(std::wstring& wsOneElement);
		void SetType(const TypeElement& enType);
		TypeElement GetType();
		void SetAttribute(StValuePr* stAttribute);
		StValuePr* GetAttribute();
		void ConversionAttribute(XmlUtils::CXmlWriter* pXmlWrite,std::wstring& wsAnnotation);
		bool CheckStyle();
	private:
		TypeElement m_enType;
		StValuePr* m_stAttribute;
		bool m_bStyle;
	};
	class CNumberOrLetter: public COneElement
	{
	public:
		CNumberOrLetter():m_wsElement(L""),m_enTypeElement(TypeElement::undefine)
		{
			SetType(TypeElement::String);
		}
		CNumberOrLetter(const std::wstring& wsElement,const TypeElement& enType);
		virtual ~CNumberOrLetter();
		void Conversion(XmlUtils::CXmlWriter* pXmlWrite,std::wstring& wsAnnotation) override;
		void Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd) override;
	private:
		std::wstring m_wsElement;
		TypeElement m_enTypeElement;
	};
	class CBinOperator:public COneElement
	{
	public:
		CBinOperator(const std::wstring& wsSymbol):m_pLeftArg(nullptr),m_pRightArg(nullptr),m_wsSymbolBinOp(wsSymbol),m_enTypeBinOp(TypeElement::undefine)
		{
			SetType(TypeElement::BinOperator);
		}
		virtual ~CBinOperator();
		void Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd) override;
		void Conversion(XmlUtils::CXmlWriter* pXmlWrite,std::wstring& wsAnnotation) override;
		bool CheckLeftArg();
		void SetLeftArg(COneElement* pElement);
		bool CheckRightArg();
		void SetRightArg(COneElement* pElement);
		static bool SetLeftArg(COneElement* pBinOp,COneElement* pLeftArg);
		static bool SetRightArg(COneElement* pBinOp,COneElement* pRightArg);
	private:
		COneElement* m_pLeftArg;
		COneElement* m_pRightArg;
		std::wstring m_wsSymbolBinOp;
		TypeElement m_enTypeBinOp;
	};
	class CSpace:public COneElement
	{
	public:
		CSpace()
		{
			SetType(TypeElement::Empty);
		}
		virtual ~CSpace();
		void Parse(std::wstring::iterator&itStart,std::wstring::iterator&itEnd) override;
		void Conversion(XmlUtils::CXmlWriter* pXmlWrite,std::wstring& wsAnnotation) override;
	};
}
#endif // COOXML2ODF_H
