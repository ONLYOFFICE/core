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
		case OOX::EElementType::et_m_box:
		{
			ConversionBox(dynamic_cast<OOX::Logic::CBox*>(pNode));
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
		bool bMrow(false);
		if(pMath->m_arrItems.size() > 1)
		{
			bMrow = true;
			m_pXmlWrite->WriteNodeBegin(L"mrow",false);
		}
		ConversionVectorWritingElement(pMath->m_arrItems);
		if(bMrow == true)
			m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
	}
	std::vector<COneElement*> COOXml2Odf::ConversionMT(OOX::Logic::CMText *pMt)
	{
		std::vector<COneElement*> arLine;
		if(pMt == nullptr)
			return arLine;
		std::wstring wsText = pMt->m_sText,wsElement;
		std::wstring::iterator itStart = wsText.begin(),itEnd = wsText.end();
		while(itStart != itEnd)
		{
			COneElement* pTempElement;
			wsElement = ParsingText(itStart,itEnd);
			pTempElement = COneElement::CreateElement(wsElement);
			if(!arLine.empty() && pTempElement->GetType() == TypeElement::BinOperator)
			{
				CBinOperator* pBinOp = dynamic_cast<CBinOperator*>(pTempElement);
				pBinOp->Parse(itStart,itEnd);
				pBinOp->SetLeftArg(arLine.back());
				arLine.pop_back();
				arLine.push_back(pBinOp);
			}
			else
				arLine.push_back(pTempElement);
		}
		return arLine;
	}
	std::wstring COOXml2Odf::ParsingText(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
		std::wstring wsElement;
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
		if(!wsElement.empty())
			return wsElement;
		else return L" ";
	}
	void COOXml2Odf::ConversionMF(OOX::Logic::CFraction *pMf)
	{
		if(pMf == nullptr)
			return;
		StValuePr stPr = ConversionFpr(dynamic_cast<OOX::Logic::CFPr*>(pMf->m_oFPr.GetPointer()));
		if(stPr.m_wsTypeName != L"lin")
		{
			m_pXmlWrite->WriteNodeBegin(L"mfrac",true);
			if(stPr.m_wsTypeName == L"skw")
			{
				m_pXmlWrite->WriteAttribute(L"bevelled",L"true");
				m_pXmlWrite->WriteNodeEnd(L"w",true,true);
				NodeDefinition(pMf->m_oNum.GetPointer());
				m_wsAnnotationStarMath += L"wideslash ";
				NodeDefinition(pMf->m_oDen.GetPointer());
			}
			else
			{
				m_pXmlWrite->WriteNodeEnd(L"w",true,true);
				NodeDefinition(pMf->m_oNum.GetPointer());
				m_wsAnnotationStarMath += L"over ";
				NodeDefinition(pMf->m_oDen.GetPointer());
			}
			m_pXmlWrite->WriteNodeEnd(L"mfrac",false,false);
		}
		else
		{
			m_pXmlWrite->WriteNodeBegin(L"mrow",false);
			NodeDefinition(pMf->m_oNum.GetPointer());
			RecordingMoNode(L"/",m_pXmlWrite);
			m_wsAnnotationStarMath += L"/ ";
			NodeDefinition(pMf->m_oDen.GetPointer());
			m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
		}
	}
	StValuePr COOXml2Odf::ConversionFpr(OOX::Logic::CFPr *pFpr)
	{
		StValuePr stValue;
		if(pFpr == nullptr)
			return stValue;
		stValue.m_wsTypeName = ConversionType(dynamic_cast<OOX::Logic::CType*>(pFpr->m_oType.GetPointer()));
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
		m_pXmlWrite->WriteString(stDelPr.m_wsBegBracket);
		m_wsAnnotationStarMath += stDelPr.m_wsBegBracket;
		for (int i = 1;i<pDel->m_arrItems.size();i++)
			NodeDefinition(pDel->m_arrItems[i]);
		m_pXmlWrite->WriteNodeBegin(L"mo",true);
		m_pXmlWrite->WriteAttribute(L"fence",L"true");
		m_pXmlWrite->WriteAttribute(L"form",L"prefix");
		m_pXmlWrite->WriteAttribute(L"stretchy",L"false");
		m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		m_pXmlWrite->WriteString(stDelPr.m_wsEndBracket);
		m_wsAnnotationStarMath += stDelPr.m_wsEndBracket;
		m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
	}
	void COOXml2Odf::ConversionNary(OOX::Logic::CNary *pNary)
	{
		if(pNary == nullptr) return;
		std::wstring wsTypeNary;
		StValuePr stNaryPr = ConversionNaryPr(pNary->m_oNaryPr.GetPointer());
		if(stNaryPr.m_bSubHide && stNaryPr.m_bSupHide)
			wsTypeNary = L"munderover";
		else if(!stNaryPr.m_bSubHide && stNaryPr.m_bSupHide)
			wsTypeNary = L"mover";
		else if(!stNaryPr.m_bSupHide && stNaryPr.m_bSubHide)
			wsTypeNary = L"munder";
		else
			wsTypeNary = L"mrow";
		m_pXmlWrite->WriteNodeBegin(wsTypeNary,false);
		RecordingMoNode(stNaryPr.m_wsChr,m_pXmlWrite);
		m_wsAnnotationStarMath += ToStringChr(stNaryPr.m_wsChr) + L" ";
		if(stNaryPr.m_bSubHide)
		{
			m_wsAnnotationStarMath += L"from ";
			ConversionSub(pNary->m_oSub.GetPointer());
		}
		if(stNaryPr.m_bSupHide)
		{
			m_wsAnnotationStarMath += L"to ";
			ConversionSup(pNary->m_oSup.GetPointer());
		}
		m_pXmlWrite->WriteNodeEnd(wsTypeNary,false,false);
		ConversionElement(pNary->m_oElement.GetPointer());
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
	std::vector<COneElement*> COOXml2Odf::ConversionMRun(OOX::Logic::CMRun *pMRun)
	{
		StValuePr* stRpr;
		std::vector<COneElement*> arLine;
		if(pMRun == nullptr) return arLine;
		if(pMRun->m_oRPr.GetPointer() != nullptr)
			stRpr = ConversionRunProperties(pMRun->m_oRPr.GetPointer());
		if(pMRun->m_oMText.GetPointer() != nullptr)
		{
			arLine = ConversionMT(pMRun->m_oMText.GetPointer());
			for(COneElement* cTemp:arLine)
				cTemp->SetAttribute(stRpr);
		}
		return arLine;
	}
	StValuePr COOXml2Odf::ConversionMdPr(OOX::Logic::CDelimiterPr *pDelPr)
	{
		StValuePr pMdPr;
		if(pDelPr == nullptr)
			return pMdPr;
		pMdPr.m_wsBegBracket = ConversionBegBracket(dynamic_cast<OOX::Logic::CBegChr*>(pDelPr->m_oBegChr.GetPointer()));
		pMdPr.m_wsEndBracket = ConversionEndBracket(dynamic_cast<OOX::Logic::CEndChr*>(pDelPr->m_oEndChr.GetPointer()));
		return pMdPr;
	}
	StValuePr COOXml2Odf::ConversionNaryPr(OOX::Logic::CNaryPr *pNaryPr)
	{
		StValuePr stNaryPr;
		if(pNaryPr == nullptr) return stNaryPr;
		stNaryPr.m_wsChr = ConversionChr(pNaryPr->m_oChr.GetPointer());
		if(pNaryPr->m_oSubHide.GetPointer() == nullptr)
			stNaryPr.m_bSubHide = true;
		if(pNaryPr->m_oSupHide.GetPointer() == nullptr)
			stNaryPr.m_bSupHide = true;
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
		if(wsChr == L"\u2211") return L"sum";
		else if(wsChr == L"\u220F") return L"prod";
		else if(wsChr == L"\u2210") return L"coprod";
		else return L"oper " + wsChr;
	}
	void COOXml2Odf::EndOdf()
	{
		m_pXmlWrite->WriteNodeEnd(L"annotation",false,false);
		m_pXmlWrite->WriteNodeEnd(L"semantics",false,false);
		m_pXmlWrite->WriteNodeEnd(L"math",false,false);
	}
	bool COOXml2Odf::IsDigit(const std::wstring &wsDigit)
	{
		if(wsDigit.empty())
			return false;
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
			if(L'+' == cOneElement || L'-' == cOneElement) return true;
		return false;
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
	void COOXml2Odf::RecordingMoNode(const std::wstring &wsSymbol, XmlUtils::CXmlWriter *pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"mo",true);
		pXmlWrite->WriteAttribute(L"stretchy",L"false");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteString(wsSymbol);
		pXmlWrite->WriteNodeEnd(L"mo",false,false);
	}
	void COOXml2Odf::ConversionBox(OOX::Logic::CBox *pBox)
	{
		if(pBox == nullptr)
			return;
		NodeDefinition(pBox->m_oElement.GetPointer());
	}
	StValuePr* COOXml2Odf::ConversionRunProperties(OOX::Logic::CRunProperty *pRPr)
	{
		StValuePr* stTempPr = new StValuePr;
		if(pRPr == nullptr)
			return nullptr;
		bool bRpr(false);
		if(pRPr->m_oColor.GetPointer() != nullptr)
		{
			bRpr = true;
			std::wstring wsColor,wsUpperColor(L"");
			wsColor =  pRPr->m_oColor.GetPointer()->m_oVal.GetPointer()->ToStringNoAlpha();
			for(wchar_t chToken: wsColor)
				wsUpperColor += std::toupper(chToken);
			stTempPr->m_wsColor = wsUpperColor;
		}
		if(bRpr == true)
			return stTempPr;
		else
			return nullptr;
	}
	void COOXml2Odf::ConversionTextVector(std::vector<COneElement *> &arLine, std::vector<COneElement *> &arNewLine)
	{
		if(!arLine.empty())
		{
			COneElement* pTempElement = nullptr;
			for(COneElement* pElement:arLine)
			{
				if(pElement->GetType() == TypeElement::String)
				{
					if(pTempElement == nullptr)
						pTempElement = pElement;
					else if(pTempElement->GetType() == TypeElement::BinOperator)
					{
						if(!CBinOperator::SetRightArg(pTempElement,pElement))
						{
							arNewLine.push_back(pTempElement);
							pTempElement = pElement;
						}
					}
					else
					{
						arNewLine.push_back(pTempElement);
						pTempElement = pElement;
					}
				}
				else if(pElement->GetType() == TypeElement::BinOperator)
					{
						if(pTempElement == nullptr)
							pTempElement = pElement;
						else if(pTempElement->GetType() != TypeElement::Empty)
						{
							if(!CBinOperator::SetLeftArg(pElement,pTempElement))
							{
								arNewLine.push_back(pTempElement);
								pTempElement = pElement;
							}
							else 
								pTempElement = pElement;
						}
						else
						{
							arNewLine.push_back(pTempElement);
							pTempElement = pElement;
						}
					}
				else
				{
					arNewLine.push_back(pTempElement);
					pTempElement = pElement;
				}
			}
		arNewLine.push_back(pTempElement);
		}
		if(!arNewLine.empty())
		{
			arLine.clear();
			for(COneElement* pElement:arNewLine)
				pElement->Conversion(m_pXmlWrite,m_wsAnnotationStarMath);
		}
	}
	void COOXml2Odf::ConversionVectorWritingElement(std::vector<OOX::WritingElement *> arWrElements)
	{
		std::vector<COneElement*> arLine,arNewLine;
		for(int i = 0; i < arWrElements.size() ;i++)
		{
			if(arWrElements[i]->getType() == OOX::EElementType::et_m_r)
			{
				std::vector<COneElement*> arTemp = ConversionMRun(dynamic_cast<OOX::Logic::CMRun*>(arWrElements[i]));
				arLine.insert(arLine.end(),arTemp.begin(),arTemp.end());
			}
			else
			{
				if(!arLine.empty())
				{
					ConversionTextVector(arLine,arNewLine);
					NodeDefinition(arWrElements[i]);
				}
				else
				{
					NodeDefinition(arWrElements[i]);
				}
			}
		}
		if(!arLine.empty())
		{
			ConversionTextVector(arLine,arNewLine);
		}
	}
//class COneElement
	COneElement::COneElement():m_stAttribute(nullptr),m_bStyle(false)
	{}
	COneElement::COneElement(std::wstring &wsOneElement):m_stAttribute(nullptr),m_bStyle(false)
	{
//		this = CreateElement(wsOneElement);
	}
	COneElement::~COneElement()
	{}
	COneElement* COneElement::CreateElement(std::wstring &wsOneElement)
	{
		if(COOXml2Odf::IsDigit(wsOneElement))
			return new CNumberOrLetter(wsOneElement,StarMath::TypeElement::number);
		else if(COOXml2Odf::IsAlpha(wsOneElement))
			return new CNumberOrLetter(wsOneElement,StarMath::TypeElement::letter);
		else if(COOXml2Odf::IsSpecialSymbol(wsOneElement))
			return new CBinOperator(wsOneElement);
		else 
			return new CSpace();
	}
	void COneElement::SetType(const TypeElement &enType)
	{
		m_enType = enType;
	}
	TypeElement COneElement::GetType()
	{
		return m_enType;
	}
	void COneElement::SetAttribute(StValuePr *stAttribute)
	{
		m_stAttribute = stAttribute;
	}
	StValuePr* COneElement::GetAttribute()
	{
		return m_stAttribute;
	}
	void COneElement::ConversionAttribute(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation)
	{
		if(!m_stAttribute->m_wsColor.empty())
		{
			pXmlWrite->WriteNodeBegin(L"mstyle",true);
			std::wstring wsColor = L"#" + m_stAttribute->m_wsColor;
			pXmlWrite->WriteAttribute(L"mathcolor",wsColor);
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			m_bStyle = true;
			wsAnnotation += L"color hex " + m_stAttribute->m_wsColor + L" ";  
		}
	}
	bool COneElement::CheckStyle()
	{
		return m_bStyle;
	}
//class CNumberOrLetter
	CNumberOrLetter::CNumberOrLetter(const std::wstring &wsElement, const TypeElement &enType)
	{
		m_wsElement = wsElement;
		m_enTypeElement = enType;
		SetType(TypeElement::String);
	}
	CNumberOrLetter::~CNumberOrLetter()
	{}
	void CNumberOrLetter::Conversion(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation)
	{
		ConversionAttribute(pXmlWrite,wsAnnotation);
		switch(m_enTypeElement)
		{
		case StarMath::TypeElement::number:
		{
			pXmlWrite->WriteNodeBegin(L"mn",false);
			pXmlWrite->WriteString(m_wsElement);
			pXmlWrite->WriteNodeEnd(L"mn",false,false);
			break;
		}
		case StarMath::TypeElement::letter:
		{
			pXmlWrite->WriteNodeBegin(L"mi",true);
			pXmlWrite->WriteAttribute(L"mathvariant",L"italic");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
			pXmlWrite->WriteString(m_wsElement);
			pXmlWrite->WriteNodeEnd(L"mi",false,false);
			break;
		}
		default:
		break;
		}
		if(CheckStyle())
			pXmlWrite->WriteNodeEnd(L"mstyle",false,false);
		wsAnnotation += L'{' + m_wsElement + L"} ";
	}
	void CNumberOrLetter::Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{}
//class CBinOperator
	CBinOperator::~CBinOperator()
	{
		delete m_pRightArg;
		delete m_pLeftArg;
	}
	void CBinOperator::Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
		std::wstring wsTempElement = COOXml2Odf::ParsingText(itStart,itEnd);
		COneElement* cElement;
		cElement = COneElement::CreateElement(wsTempElement);
		cElement->Parse(itStart,itEnd);
		m_pRightArg = cElement;
	}
	void CBinOperator::Conversion(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation)
	{
		pXmlWrite->WriteNodeBegin(L"mrow",false);
		ConversionAttribute(pXmlWrite,wsAnnotation);
		if(m_pLeftArg != nullptr)
			m_pLeftArg->Conversion(pXmlWrite,wsAnnotation);
		COOXml2Odf::RecordingMoNode(m_wsSymbolBinOp,pXmlWrite);
		wsAnnotation += m_wsSymbolBinOp + L" ";
		if(m_pRightArg != nullptr)
			m_pRightArg->Conversion(pXmlWrite,wsAnnotation);
		if(CheckStyle())
			pXmlWrite->WriteNodeEnd(L"mstyle",false,false);
		pXmlWrite->WriteNodeEnd(L"mrow",false,false);
	}
	void CBinOperator::SetLeftArg(COneElement *pElement)
	{
		m_pLeftArg = pElement;
	}
	void CBinOperator::SetRightArg(COneElement *pElement)
	{
		m_pRightArg = pElement;
	}
	bool CBinOperator::CheckRightArg()
	{
		return (m_pRightArg != nullptr);
	}
	bool CBinOperator::CheckLeftArg()
	{
		return (m_pLeftArg !=nullptr);
	}
	bool CBinOperator::SetRightArg(COneElement *pBinOp, COneElement *pRightArg)
	{
		CBinOperator* pTempBinOp = dynamic_cast<CBinOperator*>(pBinOp);
		if(!pTempBinOp->CheckRightArg())
		{
			pTempBinOp->SetRightArg(pRightArg);
			return true;
		}
		else
			return false;
	}
	bool CBinOperator::SetLeftArg(COneElement *pBinOp, COneElement *pLeftArg)
	{
		CBinOperator* pTempBinOp = dynamic_cast<CBinOperator*>(pBinOp);
		if(!pTempBinOp->CheckLeftArg() && pTempBinOp->GetAttribute() == nullptr)
		{
			pTempBinOp->SetLeftArg(pLeftArg);
			return true;
		}
		else 
			return false;
	}
//class CSpace
	CSpace::~CSpace()
	{}
	void CSpace::Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{}
	void CSpace::Conversion(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation)
	{
		pXmlWrite->WriteNodeBegin(L"mspace",true);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		wsAnnotation += L"` ";
	}
}
