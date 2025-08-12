#ifndef COOXML2ODF_H
#define COOXML2ODF_H
#include"../../../Writer/Converter/Converter.h"
#include"../../../../OOXML/DocxFormat/Math/oMathPara.h"
#include "../../../../DesktopEditor/xml/include/xmlwriter.h"
#include "../../../../OOXML/Base/Unit.h"
#include "../../../../OOXML/Common/SimpleTypes_OMath.h"
#include "../../../../DesktopEditor/common/StringUTF32.h"
#include "../../../../../DesktopEditor/common/File.h"
#include "typeselements.h"
#include "fontType.h"
#include <vector>
#include <utility>
#include <cstring>
#include <sstream>
#include <stack>
#include "../../../../../Common/3dParty/cryptopp/sha.h"
#include "../../../../../Common/3dParty/cryptopp/hex.h"
#include "../../../../../Common/3dParty/cryptopp/base64.h"
#include "../../../../../Common/3dParty/cryptopp/filters.h"
#include "../../../../../Common/3dParty/cryptopp/files.h"

namespace StarMath
{
	struct StValuePr
	{
		StValuePr():m_wsTypeName(L""),m_wsChr(L""),m_wsBegBracket(L""),m_wsEndBracket(L""),m_wsColor(L""),m_bSupHide(false),m_bSubHide(false),m_enStyle(SimpleTypes::EStyle::stylePlain),m_iSize(0),m_enPos(SimpleTypes::ETopBot::tbBot),m_enVert(SimpleTypes::ETopBot::tbBot),m_enFont(StarMath::TypeFont::empty),m_iCount(0),m_bStrike(false),m_enUnderLine(SimpleTypes::EUnderline::underlineNone),m_bBaseAttribute(false)
		{}
		std::wstring m_wsTypeName,m_wsChr;
		std::wstring m_wsBegBracket,m_wsEndBracket;
		std::wstring m_wsColor;
		bool m_bSupHide,m_bSubHide;
		SimpleTypes::EStyle m_enStyle;
		int m_iSize;
		SimpleTypes::ETopBot m_enPos,m_enVert;
		StarMath::TypeFont m_enFont;
		int m_iCount;
		bool m_bStrike;
		SimpleTypes::EUnderline m_enUnderLine;
		bool m_bBaseAttribute;
		void AddRef()
		{
			m_iCount++;
		}
		void Release()
		{
			m_iCount--;
			if(m_iCount == 0)
				delete this;
		}
	};
	struct TFormulaSize
	{
		TFormulaSize():m_iHeight(0),m_iWidth(0) {};
		TFormulaSize(const float& iHeight,const float& iWidth):m_iHeight(iHeight),m_iWidth(iWidth) {};
		float m_iHeight;
		float m_iWidth;
		void Zeroing()
		{
			m_iHeight = 0;
			m_iWidth = 0;
		}
		TFormulaSize& operator=(const TFormulaSize& stOther)
		{
			this->m_iHeight = stOther.m_iHeight;
			this->m_iWidth = stOther.m_iWidth;
			return *this;
		}
	};
	struct StStyleMenClose
	{
		StStyleMenClose():m_iStyle(0),m_bMenClose(false),m_bUnderlineClose(false)
		{}
		unsigned int m_iStyle;
		bool m_bMenClose;
		bool m_bUnderlineClose;
	};
	class COneElement;
	class COOXml2Odf
	{
	public:
		COOXml2Odf();
		~COOXml2Odf();
		void StartConversion(OOX::WritingElement* pNode);
		void ConversionMathPara(OOX::Logic::COMathPara* pMathPara);
		void NodeDefinition(OOX::WritingElement* pNode, const bool &bMatrix = false);
		void ConversionMath(OOX::Logic::COMath* pMath);
		std::vector<COneElement *> ConversionMT(OOX::Logic::CMText* pMt, const StValuePr* pValue, const bool &bMRpr = false);
		void ConversionMF(OOX::Logic::CFraction* pMf);
		StStyleMenClose ConversionCtrlPr(OOX::Logic::CCtrlPr* pCtrlPr, const bool& bDelimiter = false);
		StStyleMenClose ConversionCMPr(OOX::Logic::CMPr* pMPr);
		StValuePr ConversionFpr(OOX::Logic::CFPr* pFpr);
		std::wstring ConversionType(OOX::Logic::CType* pType);
		void ConversionMd(OOX::Logic::CDelimiter* pDel);
		std::wstring BracketForAnnotation(const std::wstring& wsBracket,const bool& bBeg);
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
		StStyleMenClose ConversionFuncPr(OOX::Logic::CFuncPr* pFuncPr);
		void ConversionBox(OOX::Logic::CBox* pBox);
		void ConversionBorderBox(OOX::Logic::CBorderBox* pBorderBox);
		void ConversionTextVector(std::vector<COneElement*>& arLine, std::vector<COneElement*>& arNewLine);
		void ConversionVectorWritingElement(std::vector<OOX::WritingElement*> arWrElements);
		void ConversionMatrix(OOX::Logic::CMatrix *pMatrix);
		void ConversionMr(OOX::Logic::CMr* pMr);
		void ConversionRad(OOX::Logic::CRad* pRad);
		StStyleMenClose ConversionRadPr(OOX::Logic::CRadPr* pRadPr,bool& bDeg);
		void ConversionDeg(OOX::Logic::CDeg* pDeg);
		void ConversionGroupChr(OOX::Logic::CGroupChr* pGroup,const bool& bElement = false);
		StValuePr ConversionGroupChrPr(OOX::Logic::CGroupChrPr* pGroupPr);
		void WritingbBracketsFromTopBottom(const std::wstring& wsNode,const std::wstring& wsChr,const std::wstring& wsAnnotation,OOX::Logic::CElement* pElement);
		void ConversionBar(OOX::Logic::CBar* pBar);
		StValuePr ConversionBarPr(OOX::Logic::CBarPr* pBarPr, StStyleMenClose &stStyle);
		void ConversionSPre(OOX::Logic::CSPre* pSPre);
		static void EmptyBlock(XmlUtils::CXmlWriter* pXmlWrite, std::wstring& wsAnnotation,TFormulaSize& stSize);
		void ConversionLimLow(OOX::Logic::CLimLow* pLimLow);
		void ConversionLimUpp(OOX::Logic::CLimUpp* pLimUpp);
		void ConversionLim(OOX::Logic::CLim* pLim);
		void ConversionEqArr(OOX::Logic::CEqArr* pEqArr);
		void ConversionSubSup(OOX::Logic::CSSubSup* pSubSup);
		StValuePr *ConversionRunProperties(OOX::Logic::CRunProperty* pRPr);
		void ConversionMRunProperties(OOX::Logic::CMRPr* pMRpr, StValuePr*&pValue);
		void ConversionARpr(PPTX::Logic::RunProperties* pARpr, StValuePr*&pValue);
		StValuePr ConversionMdPr(OOX::Logic::CDelimiterPr* pDelPr, StStyleMenClose &stStyle);
		StValuePr ConversionNaryPr(OOX::Logic::CNaryPr* pNaryPr, StStyleMenClose &stStyle);
		std::wstring ConversionBegBracket(OOX::Logic::CBegChr* pBegChr);
		std::wstring ConversionEndBracket(OOX::Logic::CEndChr* pEndChr);
		std::wstring TranslationDiacritSign(const std::wstring &wsSymbol);
		static std::wstring ParsingText(std::wstring::iterator &itStart, std::wstring::iterator &itEnd);
		std::wstring ConversionChr(OOX::Logic::CChr* pChr);
		std::wstring ToStringChr(const std::wstring& wsChr);
		std::wstring GroupChrForAnnotation(const std::wstring& wsChr);
		static void RecordingMoNode(const std::wstring& wsSymbol,XmlUtils::CXmlWriter* pXmlWrite);
		static bool IsDigit(const std::wstring& wsDigit);
		static bool IsAlpha(const std::wstring& wsAlpha);
		static void StyleClosing(const StStyleMenClose &stStyle, XmlUtils::CXmlWriter* pXmlWrite);
		static void MTextRecording(XmlUtils::CXmlWriter* pXmlWrite, std::wstring& wsAnnotation,const std::wstring& wsText,TFormulaSize& stSize);
		static void ComparisonSizeByHeight(TFormulaSize& stLeft,const TFormulaSize& stRight);
		static void ComparisonSizeByWidth(TFormulaSize& stLeft,const TFormulaSize& stRight);
		std::wstring TransformationUTF32(const std::wstring& wsText);
		bool ComparingAttributes(StValuePr* pRight, StValuePr* pLeft);
		void AttributeCheck(StValuePr*& pParent, StValuePr*& pChild);
		void AttributeCheck(StValuePr*& pChild);
		void CreateAttribute(StValuePr*& pAttribute);
		StarMath::TypeFont FontCheck(const std::wstring& wsFont, bool& bAttribute);
		static bool ColorCheck(const std::wstring& wsColor,std::wstring& wsRecordColor);
		void CheckVectorElementsForMf(std::vector<OOX::WritingElement*> arWrElement);
		TFormulaSize GetFormulaSize();
		void EndOdf();
		std::wstring GetOdf();
		std::wstring GetAnnotation();
		std::wstring GetSemantic();
		void SetBaseAttribute(std::wstring wsBaseColor = L"", unsigned int uiBaseSize = 0);
		static std::wstring HashingAnnotation(const std::wstring & wsAnnotation);
		static bool HashComparison(const std::wstring& wsHashFirst,const std::wstring& wsHashSecond);
		std::wstring GetHashAnnotation();
	private:
		XmlUtils::CXmlWriter* m_pXmlWrite;
		std::wstring m_wsAnnotationStarMath,m_wsSemantic;
		std::stack<StValuePr*> m_stAttribute;
		std::wstring m_wsBaseColor,m_wsHashAnnotation;
		unsigned int m_uiBaseSize;
		TFormulaSize m_stSize;
		bool m_bHeight;
		bool m_bStretchyAcc;
	};
	class COneElement
	{
	public:
		COneElement();
		COneElement(std::wstring& wsOneElement);
		virtual ~COneElement();
		virtual void Parse(std::wstring::iterator& itStart,std::wstring::iterator& itEnd,COneElement*& pElement) = 0;
		virtual void Conversion(XmlUtils::CXmlWriter* pXmlWrite,std::wstring& wsAnnotation,TFormulaSize& stSize) = 0;
		virtual void SetAttribute(StValuePr* pAttribute) = 0;
		static COneElement* CreateElement(std::wstring& wsOneElement);
		void SetType(const TypeElement& enType);
		TypeElement GetType();
		void SetBaseAttribute(StValuePr* stAttribute);
		StValuePr* GetAttribute();
		static void ConversionAttribute(StValuePr* pAttribute, StStyleMenClose & stStyle , XmlUtils::CXmlWriter* pXmlWrite, std::wstring& wsAnnotation, const bool &bDelimiter = false);
		bool CheckStyle();
		unsigned int GetStyle();
	private:
		TypeElement m_enType;
		StValuePr* m_stAttribute;
		unsigned int m_iStyle;
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
		void Conversion(XmlUtils::CXmlWriter* pXmlWrite,std::wstring& wsAnnotation,TFormulaSize& stSize) override;
		void Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd,COneElement*& pElement) override;
		void SetAttribute(StValuePr* pAttribute) override;
		const std::wstring& GetString();
		void AddingStrings(const std::wstring& wsString);
	private:
		std::wstring m_wsElement;
		TypeElement m_enTypeElement;
	};
	class CBinOperator:public COneElement
	{
	public:
		CBinOperator(const std::wstring& wsSymbol,const std::wstring& wsAnnotation):m_pLeftArg(nullptr),m_pRightArg(nullptr),m_wsSymbolBinOp(wsSymbol),m_wsAnnotation(wsAnnotation),m_enTypeBinOp(TypeElement::undefine)
		{
			SetType(TypeElement::BinOperator);
			if(wsSymbol == L"+")
				m_enTypeBinOp = TypeElement::plus;
			else if(wsSymbol == L"-")
				m_enTypeBinOp = TypeElement::minus;
			else if(wsSymbol == L"\u00B1")
				m_enTypeBinOp = TypeElement::plus_minus;
			else if(wsSymbol == L"\u2213")
				m_enTypeBinOp = TypeElement::minus_plus;
			else if(L"\u00AC" == wsSymbol)
				m_enTypeBinOp = TypeElement::neg;
		}
		virtual ~CBinOperator();
		void Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd, COneElement *&pElement) override;
		void Conversion(XmlUtils::CXmlWriter* pXmlWrite,std::wstring& wsAnnotation,TFormulaSize& stSize) override;
		void SetAttribute(StValuePr* pAttribute) override;
		bool CheckLeftArg();
		void SetLeftArg(COneElement* pElement);
		bool CheckRightArg();
		void SetRightArg(COneElement* pElement);
		TypeElement GetTypeBinOp();
		static bool SetLeftArg(COneElement* pBinOp,COneElement* pLeftArg);
		static bool SetRightArg(COneElement* pBinOp,COneElement* pRightArg);
		static bool CheckRightArg(COneElement* pElement);
		static std::wstring BinOperatorSymbolForAnnotation(const std::wstring& wsSymbol);
		static bool UnaryCheck(const StValuePr* pValue,const TypeElement& enType);
	private:
		COneElement* m_pLeftArg;
		COneElement* m_pRightArg;
		std::wstring m_wsSymbolBinOp;
		std::wstring m_wsAnnotation;
		TypeElement m_enTypeBinOp;
	};
	class CRelationsAndOperationsOnSets: public COneElement
	{
	public:
		CRelationsAndOperationsOnSets(const std::wstring& wsSymbol,const std::wstring& wsAnnotation):m_pLeftArg(nullptr),m_pRightArg(nullptr),m_wsSymbol(wsSymbol),m_wsAnnotationSymbol(wsAnnotation)
		{
			SetType(TypeElement::Connection);
		}
		virtual ~CRelationsAndOperationsOnSets();
		void Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd,COneElement*& pElement) override;
		void Conversion(XmlUtils::CXmlWriter* pXmlWrite,std::wstring& wsAnnotation,TFormulaSize& stSize) override;
		void SetAttribute(StValuePr* pAttribute) override;
		void SetLeftArg(COneElement* pElement);
		void SetRightArg(COneElement* pElement);
		bool CheckRightArg();
		bool CheckLeftArg();
		static std::wstring TransformationForAnnotation(std::wstring& wsSymbol);
	private:
		COneElement* m_pLeftArg;
		COneElement* m_pRightArg;
		std::wstring m_wsSymbol;
		std::wstring m_wsAnnotationSymbol;
	};
	class CSpace:public COneElement
	{
	public:
		CSpace()
		{
			SetType(TypeElement::Empty);
		}
		virtual ~CSpace();
		void Parse(std::wstring::iterator&itStart,std::wstring::iterator&itEnd,COneElement*& pElement) override;
		void Conversion(XmlUtils::CXmlWriter* pXmlWrite,std::wstring& wsAnnotation,TFormulaSize& stSize) override;
		void SetAttribute(StValuePr* pAttribute) override;
	};
	class CSpecialChar: public COneElement
	{
	public:
		CSpecialChar(const std::wstring& wsSymbol,const std::wstring& wsAnnotation):m_wsSymbol(wsSymbol),m_wsAnnotation(wsAnnotation)
		{
			SetType(TypeElement::SpecialSymbol);
		}
		virtual ~CSpecialChar();
		void Parse(std::wstring::iterator&itStart,std::wstring::iterator&itEnd,COneElement*& pElement) override;
		void Conversion(XmlUtils::CXmlWriter* pXmlWrite,std::wstring& wsAnnotation,TFormulaSize& stSize) override;
		void SetAttribute(StValuePr* pAttribute) override;
		static std::wstring DefinitionSpecialChar(const std::wstring& wsSymbol);
	private:
		std::wstring m_wsSymbol,m_wsAnnotation;
	};
}
#endif // COOXML2ODF_H
