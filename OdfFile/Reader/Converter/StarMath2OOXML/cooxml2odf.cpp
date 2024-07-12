#include "cooxml2odf.h"
#include <iterator>

namespace StarMath
{
//class OOXml2Odf
	COOXml2Odf::COOXml2Odf()
	{}
	COOXml2Odf::~COOXml2Odf()
	{}
	void COOXml2Odf::StartConversion(OOX::Logic::COMathPara *pMathPara)
	{
		m_pXmlWrite->WriteNodeBegin(L"math",true);
		m_pXmlWrite->WriteAttribute(L"xmln",L"http:\/\/www.w3.org/1998/Math/MathML");
		m_pXmlWrite->WriteAttribute(L"display",L"block");
		m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		m_pXmlWrite->WriteNodeBegin(L"semantics",false);
		if(pMathPara == nullptr)
		{
			m_pXmlWrite->WriteNodeBegin(L"annotation",false);
			EndOdf();
			return;
		}
		for(int i = 0; i< pMathPara->m_arrItems.size();i++)
		{
			NodeDefinition(pMathPara->m_arrItems[i]);
		}
		m_pXmlWrite->WriteNodeBegin(L"annotation",true);
		m_pXmlWrite->WriteAttribute(L"encoding",L"StarMath 5.0");
		m_pXmlWrite->WriteString(m_wsAnnotationStarMath);
		EndOdf();
	}
	void COOXml2Odf::NodeDefinition(OOX::WritingElement *pNode)
	{
		if(pNode == nullptr)
			return;
		switch(pNode->getType())
		{
		case OOX::EElementType::et_m_oMath:
		{
			ConversionMath(dynamic_cast<OOX::Logic::COMath*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_t:
		{
			ConversionMT(dynamic_cast<OOX::Logic::CMText*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_f:
		{
			ConversionMF(dynamic_cast<OOX::Logic::CFraction*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_r:
		{
			ConversionRun(dynamic_cast<OOX::Logic::CMRun*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_d:
		{
			ConversionMd(dynamic_cast<OOX::Logic::CDelimiter*>(pNode));
			break;
		}
		default:
		break;
		}
	}
	void COOXml2Odf::ConversionMath(OOX::Logic::COMath *pMath)
	{
		if(pMath == nullptr)
			return;
		for(int i = 0;i<pMath->m_arrItems.size();i++)
		{
			NodeDefinition(pMath->m_arrItems[i]);
		}
	}
	void COOXml2Odf::ConversionMT(OOX::Logic::CMText *pMt)
	{
		if(pMt == nullptr)
			return;
		std::wstring wsText = pMt->m_sText,wsTempElement;
		m_wsAnnotationStarMath +=L"{" + wsText + L"}";
		std::wstring::iterator itStart = wsText.begin(),itEnd = wsText.end();
		int iCountMrow{0};
		bool bMrowOpen(true),bMrowClose(false);
		while(itStart != itEnd)
		{
			wsTempElement = ParsingText(itStart,itEnd,bMrowClose,bMrowOpen);
			if(bMrowOpen)
			{
				bMrowOpen = false;
				iCountMrow++;
				m_pXmlWrite->WriteNodeBegin(L"mrow",false);
			}
			if(IsDigit(wsTempElement))
			{
				m_pXmlWrite->WriteNodeBegin(L"mn",false);
				m_pXmlWrite->WriteString(wsTempElement);
				m_pXmlWrite->WriteNodeEnd(L"mn",false,false);
			}
			else if(IsAlpha(wsTempElement))
			{
				m_pXmlWrite->WriteNodeBegin(L"mi",true);
				m_pXmlWrite->WriteAttribute(L"mathvariant",L"italic");
				m_pXmlWrite->WriteNodeEnd(L"w",true,true);
				m_pXmlWrite->WriteString(wsTempElement);
				m_pXmlWrite->WriteNodeEnd(L"mi",false,false);
			}
			else if(SpeshialSymbol(wsTempElement))
			{
				m_pXmlWrite->WriteNodeBegin(L"mo",true);
				m_pXmlWrite->WriteAttribute(L"stretchy",L"false");
				m_pXmlWrite->WriteNodeEnd(L"w",true,true);
				m_pXmlWrite->WriteString(wsTempElement);
				m_pXmlWrite->WriteNodeEnd(L"mo",false,false);
			}
			
//			if(bMrowClose)
//			{
//				for(int i = 0;i<iCountMrow;i++)
//					m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
				
//			}
//			else
//				iCountMrow++;
		}
	}
	std::wstring COOXml2Odf::ParsingText(std::wstring::iterator &itStart, std::wstring::iterator &itEnd, bool& bMrowClose, bool &bMrowOpen)
	{
		std::wstring wsElement{};
		for(;itStart != itEnd;itStart++)
		{
			if(iswspace(*itStart))
					if(wsElement.empty()) continue;
					else
					{
						itStart++;
						if(L'+' == *itStart || L'-' == *itStart)
						{
							bMrowOpen = true;
							bMrowClose = false;
						}
						return wsElement;
					}
			else if(iswdigit(*itStart) && !wsElement.empty())
				if(iswdigit(wsElement.back()))
					wsElement.push_back(*itStart);
				else 
				{
					bMrowClose = true;
					return wsElement;
				}
			else if(iswalpha(*itStart) && !wsElement.empty())
				if(iswalpha(wsElement.back()))
					wsElement.push_back(*itStart);
				else
				{
					bMrowClose = true;
					return wsElement;
				}
			else if(wsElement.empty() && (L'+' == *itStart || L'-' == *itStart))
			{
				wsElement.push_back(*itStart);
				bMrowClose = false;
				return wsElement;
			}
			else if(!wsElement.empty() && (L'+' == *itStart || L'-' == *itStart))
			{
				bMrowClose = false;
				return wsElement;
			}
			else
				wsElement.push_back(*itStart);
		}
	}
	void COOXml2Odf::ConversionMF(OOX::Logic::CFraction *pMf)
	{
		if(pMf == nullptr)
			return;
		StValuePr stPr = ConversionFpr(dynamic_cast<OOX::Logic::CFPr*>(pMf->m_oFPr.GetPointer()));
		m_pXmlWrite->WriteNodeBegin(L"mfrac",true);
		m_pXmlWrite->WriteAttribute(L"bevelled",L"true");
		m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		if(stPr.wsTypeName == L"skw")
		{
			NodeDefinition(pMf->m_oNum.GetPointer());
			m_wsAnnotationStarMath += L" wideslash ";
			NodeDefinition(pMf->m_oDen.GetPointer());
		}
		else
		{
			NodeDefinition(pMf->m_oNum.GetPointer());
			m_wsAnnotationStarMath += L" over ";
			NodeDefinition(pMf->m_oDen.GetPointer());
		}
		m_pXmlWrite->WriteNodeEnd(L"mfrac",false,false);
	}
	StValuePr COOXml2Odf::ConversionFpr(OOX::Logic::CFPr *pFpr)
	{
		StValuePr stValue;
		if(pFpr == nullptr)
			return stValue;
		stValue.wsTypeName = ConversionType(dynamic_cast<OOX::Logic::CType*>(pFpr->m_oType.GetPointer()));
		return stValue;
	}
	std::wstring COOXml2Odf::ConversionType(OOX::Logic::CType *pType)
	{
		if(pType == nullptr)
			return L"";
		std::wstring wsType = pType->m_val->ToString();
		return wsType;
	}
	void COOXml2Odf::ConversionMd(OOX::Logic::CDelimiter *pDel)
	{
		if(pDel == nullptr)
			return;
		StValuePr stDelPr;
		if(pDel->m_arrItems[0] != nullptr && pDel->m_arrItems[0]->getType() == OOX::EElementType::et_m_dPr)
			stDelPr = ConversionMdPr(dynamic_cast<OOX::Logic::CDelimiterPr*>(pDel->m_arrItems[0]));
		m_pXmlWrite->WriteNodeBegin(L"mrow",false);
		m_pXmlWrite->WriteNodeBegin(L"mo",true);
		m_pXmlWrite->WriteAttribute(L"fence",L"true");
		m_pXmlWrite->WriteAttribute(L"form",L"prefix");
		m_pXmlWrite->WriteAttribute(L"stretchy",L"false");
		m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		m_pXmlWrite->WriteString(stDelPr.wsBegBracket);
		m_wsAnnotationStarMath += stDelPr.wsBegBracket;
		for (int i = 1;i<pDel->m_arrItems.size();i++)
			NodeDefinition(pDel->m_arrItems[i]);
		m_pXmlWrite->WriteNodeBegin(L"mo",true);
		m_pXmlWrite->WriteAttribute(L"fence",L"true");
		m_pXmlWrite->WriteAttribute(L"form",L"prefix");
		m_pXmlWrite->WriteAttribute(L"stretchy",L"false");
		m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		m_pXmlWrite->WriteString(stDelPr.wsEndBracket);
		m_wsAnnotationStarMath += stDelPr.wsEndBracket;
		m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
	}
	StValuePr COOXml2Odf::ConversionMdPr(OOX::Logic::CDelimiterPr *pDelPr)
	{
		StValuePr pMdPr;
		if(pDelPr == nullptr)
			return pMdPr;
		pMdPr.wsBegBracket = ConversionBegBracket(dynamic_cast<OOX::Logic::CBegChr*>(pDelPr->m_oBegChr.GetPointer()));
		pMdPr.wsEndBracket = ConversionEndBracket(dynamic_cast<OOX::Logic::CEndChr*>(pDelPr->m_oEndChr.GetPointer()));
		return pMdPr;
	}
	std::wstring COOXml2Odf::ConversionBegBracket(OOX::Logic::CBegChr *pBegChr)
	{
		if(pBegChr == nullptr)
			return L"(";
		if(pBegChr->m_val.GetPointer() == nullptr) 
			return L"(";
		return pBegChr->m_val->GetValue();
	}
	std::wstring COOXml2Odf::ConversionEndBracket(OOX::Logic::CEndChr *pEndChr)
	{
		if(pEndChr == nullptr)
			return L")";
		if(pEndChr->m_val.GetPointer() == nullptr) 
			return L")";
		return pEndChr->m_val->GetValue();
	}
	void COOXml2Odf::EndOdf()
	{
		m_pXmlWrite->WriteNodeEnd(L"annotation",false,false);
		m_pXmlWrite->WriteNodeEnd(L"semantics",false,false);
		m_pXmlWrite->WriteNodeEnd(L"math",false,false);
	}
}
