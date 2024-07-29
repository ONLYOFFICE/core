#include "cooxml2odf.h"
#include <iterator>

namespace StarMath
{
//class OOXml2Odf
	COOXml2Odf::COOXml2Odf()
	{
		m_pXmlWrite = new XmlUtils::CXmlWriter;
	}
	COOXml2Odf::~COOXml2Odf()
	{
		delete m_pXmlWrite;
	}
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
		m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		m_pXmlWrite->WriteString(m_wsAnnotationStarMath);
		EndOdf();
		NSFile::CFileBinary oOriginal;
		oOriginal.CreateFile(L"X:/Rabotka/Original.txt");
		oOriginal.WriteStringUTF8(m_pXmlWrite->GetXmlString());
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
			ConversionMRun(dynamic_cast<OOX::Logic::CMRun*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_d:
		{
			ConversionMd(dynamic_cast<OOX::Logic::CDelimiter*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_nary:
		{
			ConversionNary(dynamic_cast<OOX::Logic::CNary*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_oMathPara:
		{
			StartConversion(dynamic_cast<OOX::Logic::COMathPara*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_acc:
		{
			ConversionAcc(dynamic_cast<OOX::Logic::CAcc*>(pNode));
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
		std::wstring wsText = pMt->m_sText.,wsTempElement;
		m_wsAnnotationStarMath +=L"{" + wsText + L"} ";
		std::wstring::iterator itStart = wsText.begin(),itEnd = wsText.end();
		m_pXmlWrite->WriteNodeBegin(L"mrow",false);
		while(itStart != itEnd)
		{
			wsTempElement = ParsingText(itStart,itEnd);
			
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
			else if(IsSpecialSymbol(wsTempElement))
			{
				m_pXmlWrite->WriteNodeBegin(L"mo",true);
				m_pXmlWrite->WriteAttribute(L"stretchy",L"false");
				m_pXmlWrite->WriteNodeEnd(L"w",true,true);
				m_pXmlWrite->WriteString(wsTempElement);
				m_pXmlWrite->WriteNodeEnd(L"mo",false,false);
			}
		}
		m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
	}
	std::wstring COOXml2Odf::ParsingText(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
		std::wstring wsElement{};
		for(;itStart != itEnd;itStart++)
		{
			if(iswspace(*itStart))
					if(wsElement.empty()) continue;
					else
					{
						itStart++;
						return wsElement;
					}
			else if(iswdigit(*itStart) && !wsElement.empty())
				if(iswdigit(wsElement.back()))
					wsElement.push_back(*itStart);
				else 
				{
					return wsElement;
				}
			else if(iswalpha(*itStart) && !wsElement.empty())
				if(iswalpha(wsElement.back()))
					wsElement.push_back(*itStart);
				else
				{
					return wsElement;
				}
			else if(wsElement.empty() && (L'+' == *itStart || L'-' == *itStart))
			{
				wsElement.push_back(*itStart);
				itStart++;
				return wsElement;
			}
			else if(!wsElement.empty() && (L'+' == *itStart || L'-' == *itStart))
			{
				return wsElement;
			}
			else
				wsElement.push_back(*itStart);
		}
		return wsElement;
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
	void COOXml2Odf::ConversionNary(OOX::Logic::CNary *pNary)
	{
		if(pNary == nullptr) return;
		std::wstring wsTypeNary;
		StValuePr stNaryPr = ConversionNaryPr(pNary->m_oNaryPr.GetPointer());
		m_pXmlWrite->WriteNodeBegin(L"mrow",false);
		if(stNaryPr.bSubHide && stNaryPr.bSupHide)
			wsTypeNary = L"munderover";
		else if(!stNaryPr.bSubHide && stNaryPr.bSupHide)
			wsTypeNary = L"mover";
		else if(!stNaryPr.bSupHide && stNaryPr.bSubHide)
			wsTypeNary = L"munder";
		else
			wsTypeNary = L"m";
		m_pXmlWrite->WriteNodeBegin(wsTypeNary,false);
		m_pXmlWrite->WriteNodeBegin(L"mo",true);
		m_pXmlWrite->WriteAttribute(L"stretchy",L"false");
		m_pXmlWrite->WriteNodeEnd(L"m",true,true);
		m_pXmlWrite->WriteString(stNaryPr.wsChr);
		m_pXmlWrite->WriteNodeEnd(L"mo",false,false);
		m_wsAnnotationStarMath += ToStringChr(stNaryPr.wsChr) + L" ";
//		if(stNaryPr.bSubHide)
//		{
//			m_wsAnnotationStarMath += L"from ";
//			ConversionSsub(pNary->m_);
//		}
//		if(stNaryPr.bSupHide)
//		{
//			m_wsAnnotationStarMath += L"to ";
//			ConversionSsup(pNary->m_oSup.GetPointer());
//		}
		m_pXmlWrite->WriteNodeEnd(wsTypeNary,false,false);
		ConversionElement(pNary->m_oElement.GetPointer());
		m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
	}
	void COOXml2Odf::ConversionSsub(OOX::Logic::CSSub *pSsub)
	{
		if(pSsub == nullptr) return;
		ConversionSub(pSsub->m_oSub.GetPointer(),pSsub->m_oElement.GetPointer());
	}
	void COOXml2Odf::ConversionSub(OOX::Logic::CSub *pSub, OOX::Logic::CElement * pElement)
	{
		if(pSub == nullptr) return;
		if(pElement == nullptr) return;
		m_wsAnnotationStarMath += L"{";
		m_pXmlWrite->WriteNodeBegin(L"msub",false);
		ConversionElement(pElement);
		m_wsAnnotationStarMath += L"}_{";
		for(int i = 0;i < pSub->m_arrItems.size();i++)
			NodeDefinition(pSub->m_arrItems[i]);
		m_pXmlWrite->WriteNodeEnd(L"msub",false,false);
		m_wsAnnotationStarMath += L"}";
	}
	void COOXml2Odf::ConversionSub(OOX::Logic::CSub *pSub)
	{
		if(pSub == nullptr)
			return;
		for(int i = 0; i < pSub->m_arrItems.size();i++)
			NodeDefinition(pSub->m_arrItems[i]);
	}
	void COOXml2Odf::ConversionSsup(OOX::Logic::CSSup *pSsup)
	{
		if(pSsup == nullptr) return;
		ConversionSup(pSsup->m_oSup.GetPointer(),pSsup->m_oElement.GetPointer());
	}
	void COOXml2Odf::ConversionSup(OOX::Logic::CSup *pSup, OOX::Logic::CElement *pElement)
	{
		if(pSup == nullptr) return;
		if(pElement == nullptr) return;
		m_wsAnnotationStarMath += L"{";
		m_pXmlWrite->WriteNodeBegin(L"msup",false);
		ConversionElement(pElement);
		m_wsAnnotationStarMath += L"}_{";
		for(int i = 0; i < pSup->m_arrItems.size();i++)
			NodeDefinition(pSup->m_arrItems[i]);
		m_pXmlWrite->WriteNodeEnd(L"msup",false,false);
		m_wsAnnotationStarMath += L"}";
	}
	void COOXml2Odf::ConversionSup(OOX::Logic::CSup *pSup)
	{
		if(pSup == nullptr)
			return;
		for(int i = 0; i < pSup->m_arrItems.size();i++)
			NodeDefinition(pSup->m_arrItems[i]);
	}
	void COOXml2Odf::ConversionElement(OOX::Logic::CElement *pElement)
	{
		if(pElement == nullptr) return;
		for(int i = 0; i < pElement->m_arrItems.size();i++)
			NodeDefinition(pElement->m_arrItems[i]);
	}
	void COOXml2Odf::ConversionMRun(OOX::Logic::CMRun *pMRun)
	{
		if(pMRun == nullptr) return;
		ConversionMT(pMRun->m_oMText.GetPointer());
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
	StValuePr COOXml2Odf::ConversionNaryPr(OOX::Logic::CNaryPr *pNaryPr)
	{
		StValuePr stNaryPr;
		if(pNaryPr == nullptr) return stNaryPr;
		stNaryPr.wsChr = ConversionChr(pNaryPr->m_oChr.GetPointer());
		if(pNaryPr->m_oSubHide.GetPointer() == nullptr)
			stNaryPr.bSubHide = true;
		if(pNaryPr->m_oSupHide.GetPointer() == nullptr)
			stNaryPr.bSupHide = true;
		return stNaryPr;
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
	//annotation?
	std::wstring COOXml2Odf::ConversionChr(OOX::Logic::CChr *pChr)
	{
		if(pChr == nullptr) return L"";
		return pChr->m_val->GetValue();
	}
	std::wstring COOXml2Odf::ToStringChr(const std::wstring &wsChr)
	{
		if(wsChr == L"∑") return L"sum";
		else if(wsChr == L"∏") return L"prod";
		else return L"";
	}
	void COOXml2Odf::EndOdf()
	{
		m_pXmlWrite->WriteNodeEnd(L"annotation",false,false);
		m_pXmlWrite->WriteNodeEnd(L"semantics",false,false);
		m_pXmlWrite->WriteNodeEnd(L"math",false,false);
	}
	bool COOXml2Odf::IsDigit(const std::wstring &wsDigit)
	{
		for(wchar_t cOneElement:wsDigit)
			if(!iswdigit(cOneElement)) return false;
		return true;
	}
	bool COOXml2Odf::IsAlpha(const std::wstring &wsAlpha)
	{
		for(wchar_t cOneElement:wsAlpha)
			if(!iswalpha(cOneElement)) return false;
		return true;
	}
	bool COOXml2Odf::IsSpecialSymbol(const std::wstring &wsSpecial)
	{
		for(wchar_t cOneElement:wsSpecial)
			if(L'+' != cOneElement || L'-' != cOneElement) return false;
		return true;
	}
	void COOXml2Odf::ConversionAcc(OOX::Logic::CAcc *pAcc)
	{
		std::wstring wsSymbol = pAcc->m_oAccPr->m_oChr.IsInit() ? pAcc->m_oAccPr->m_oChr.get().m_val->GetValue() : L"";
//		TranslationDiacritSign(wsSymbol);
		//конвертация свойств
		m_pXmlWrite->WriteNodeBegin(L"mover",true);
		m_pXmlWrite->WriteAttribute(L"accent",L"true");
		m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		ConversionElement(pAcc->m_oElement.GetPointer());
		m_pXmlWrite->WriteNodeBegin(L"mo",true);
		m_pXmlWrite->WriteAttribute(L"stretchy",L"false");
		m_pXmlWrite->WriteNodeEnd(L"w",false,false);
		m_pXmlWrite->WriteString(wsSymbol);
		m_pXmlWrite->WriteNodeEnd(L"mo",false,false);
		m_pXmlWrite->WriteNodeEnd(L"mover",false,false);
	}
	void COOXml2Odf::ConversionFunc(OOX::Logic::CFunc *pFunc)
	{
		if(pFunc == nullptr)
			return;
		if(pFunc->m_oFName.GetPointer() == nullptr)
		{
			for(int i = 0; i < pFunc->m_oFName.GetPointer()->m_arrItems.size();i++)
				NodeDefinition(pFunc->m_oFName.GetPointer()->m_arrItems[i]);
		}
		
		ConversionElement(pFunc->m_oElement.GetPointer());
	}
}
