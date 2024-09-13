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
		m_pXmlWrite->WriteAttribute(L"xmlns",L"http:\/\/www.w3.org/1998/Math/MathML");
		m_pXmlWrite->WriteAttribute(L"display",L"block");
		m_pXmlWrite->WriteNodeEnd(L"w",true,false);
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
		m_pXmlWrite->WriteNodeEnd(L"w",true,false);
		m_pXmlWrite->WriteString(m_wsAnnotationStarMath);
		EndOdf();
		NSFile::CFileBinary oOriginal;
		oOriginal.CreateFile(L"X:/Rabotka/Original.txt");
		oOriginal.WriteStringUTF8(m_pXmlWrite->GetXmlString());
	}
	void COOXml2Odf::NodeDefinition(OOX::WritingElement *pNode,const bool& bMatrix)
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
		case OOX::EElementType::et_m_rad:
		{
			ConversionRad(dynamic_cast<OOX::Logic::CRad*>(pNode));
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
		case OOX::EElementType::et_m_m:
		{
			ConversionMatrix(dynamic_cast<OOX::Logic::CMatrix*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_e:
		{
			ConversionElement(dynamic_cast<OOX::Logic::CElement*>(pNode),bMatrix);
			break;
		}
		case OOX::EElementType::et_m_groupChr:
		{
			ConversionGroupChr(dynamic_cast<OOX::Logic::CGroupChr*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_bar:
		{
			ConversionBar(dynamic_cast<OOX::Logic::CBar*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_sPre:
		{
			ConversionSPre(dynamic_cast<OOX::Logic::CSPre*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_sSub:
		{
			ConversionSsub(dynamic_cast<OOX::Logic::CSSub*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_sSup:
		{
			ConversionSsup(dynamic_cast<OOX::Logic::CSSup*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_eqArr:
		{
			ConversionEqArr(dynamic_cast<OOX::Logic::CEqArr*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_limLow:
		{
			ConversionLimLow(dynamic_cast<OOX::Logic::CLimLow*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_limUpp:
		{
			ConversionLimUpp(dynamic_cast<OOX::Logic::CLimUpp*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_func:
		{
			ConversionFunc(dynamic_cast<OOX::Logic::CFunc*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_sSubSup:
		{
			ConversionSubSup(dynamic_cast<OOX::Logic::CSSubSup*>(pNode));
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
		if(stPr.m_wsTypeName == L"noBar")
		{
			m_wsAnnotationStarMath += L"binom ";
			m_pXmlWrite->WriteNodeBegin(L"mtable",false);
			m_pXmlWrite->WriteNodeBegin(L"mtr",false);
			m_pXmlWrite->WriteNodeBegin(L"mtd",false);
			ConversionVectorWritingElement(pMf->m_oNum.GetPointer()->m_arrItems);
			m_pXmlWrite->WriteNodeEnd(L"mtd",false,false);
			m_pXmlWrite->WriteNodeEnd(L"mtr",false,false);
			m_pXmlWrite->WriteNodeBegin(L"mtr",false);
			m_pXmlWrite->WriteNodeBegin(L"mtd",false);
			ConversionVectorWritingElement(pMf->m_oDen.GetPointer()->m_arrItems);
			m_pXmlWrite->WriteNodeEnd(L"mtd",false,false);
			m_pXmlWrite->WriteNodeEnd(L"mtr",false,false);
			m_pXmlWrite->WriteNodeEnd(L"mtable",false,false);
		}
		else if(stPr.m_wsTypeName != L"lin")
		{
			m_pXmlWrite->WriteNodeBegin(L"mfrac",true);
			if(stPr.m_wsTypeName == L"skw")
			{
				m_pXmlWrite->WriteAttribute(L"bevelled",L"true");
				m_pXmlWrite->WriteNodeEnd(L"w",true,true);
				ConversionVectorWritingElement(pMf->m_oNum.GetPointer()->m_arrItems);
				m_wsAnnotationStarMath += L"wideslash ";
				ConversionVectorWritingElement(pMf->m_oDen.GetPointer()->m_arrItems);
			}
			else
			{
				m_pXmlWrite->WriteNodeEnd(L"w",true,false);
				ConversionVectorWritingElement(pMf->m_oNum.GetPointer()->m_arrItems);
				m_wsAnnotationStarMath += L"over ";
				ConversionVectorWritingElement(pMf->m_oDen.GetPointer()->m_arrItems);
			}
			m_pXmlWrite->WriteNodeEnd(L"mfrac",false,false);
		}
		else
		{
			m_pXmlWrite->WriteNodeBegin(L"mrow",false);
			ConversionVectorWritingElement(pMf->m_oNum.GetPointer()->m_arrItems);
			RecordingMoNode(L"/",m_pXmlWrite);
			m_wsAnnotationStarMath += L"/ ";
			ConversionVectorWritingElement(pMf->m_oDen.GetPointer()->m_arrItems);
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
		bool bStyle;
		if(pDel->m_arrItems[0] != nullptr && pDel->m_arrItems[0]->getType() == OOX::EElementType::et_m_dPr)
			stDelPr = ConversionMdPr(dynamic_cast<OOX::Logic::CDelimiterPr*>(pDel->m_arrItems[0]),bStyle);
		m_pXmlWrite->WriteNodeBegin(L"mrow",false);
		if(!stDelPr.m_wsBegBracket.empty())
		{
			m_pXmlWrite->WriteNodeBegin(L"mo",true);
			m_pXmlWrite->WriteAttribute(L"fence",L"true");
			m_pXmlWrite->WriteAttribute(L"form",L"prefix");
			m_pXmlWrite->WriteAttribute(L"stretchy",L"true");
			m_pXmlWrite->WriteNodeEnd(L"w",true,true);
			m_pXmlWrite->WriteString(stDelPr.m_wsBegBracket);
		}
		m_wsAnnotationStarMath += L"left " + BracketForAnnotation(stDelPr.m_wsBegBracket,true) + L" ";
		m_pXmlWrite->WriteNodeBegin(L"mrow",false);
		for(int i = 1; i<pDel->m_arrItems.size();i++)
		{
			if(i >= 2)
			{
				RecordingMoNode(L"\u007C",m_pXmlWrite);
				m_wsAnnotationStarMath += L"mline ";
			}
			NodeDefinition(pDel->m_arrItems[i]);
		}
		m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
		if(!stDelPr.m_wsEndBracket.empty())
		{
			m_pXmlWrite->WriteNodeBegin(L"mo",true);
			m_pXmlWrite->WriteAttribute(L"fence",L"true");
			m_pXmlWrite->WriteAttribute(L"form",L"postfix");
			m_pXmlWrite->WriteAttribute(L"stretchy",L"true");
			m_pXmlWrite->WriteNodeEnd(L"w",true,true);
			m_pXmlWrite->WriteString(stDelPr.m_wsEndBracket);
		}
		m_wsAnnotationStarMath += L"right " + BracketForAnnotation(stDelPr.m_wsEndBracket,false) + L" ";
		m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
		if(bStyle)
			m_pXmlWrite->WriteNodeEnd(L"mstyle",false);
	}
	std::wstring COOXml2Odf::BracketForAnnotation(const std::wstring &wsBracket, const bool &bBeg)
	{
		if(wsBracket.empty()) return L"none";
		else if(wsBracket == L"\u007C" && bBeg) return L"lline";
		else if(wsBracket == L"\u007C" && !bBeg) return L"rline";
		else if(wsBracket == L"\u2016" && bBeg) return L"ldline";
		else if(wsBracket == L"\u2016" && !bBeg) return L"rdline";
		else if(wsBracket == L"\u27E6") return L"ldbracket";
		else if(wsBracket == L"\u27E7") return L"rdbracket";
		else if(wsBracket == L"\u007B") return L"lbrace";
		else if(wsBracket == L"\u007D") return L"rbrace";
		else if(wsBracket == L"\u2329" || L"\u27E8" == wsBracket) return L"langle";
		else if(wsBracket == L"\u232A" || L"\u27E9" == wsBracket) return L"rangle";
		else if(wsBracket == L"\u2308") return L"lceil";
		else if(wsBracket == L"\u2309") return L"rceil";
		else if(wsBracket == L"\u230A") return L"lfloor";
		else if(wsBracket == L"\u230B") return L"rfloor";
		else return wsBracket;
	}
	void COOXml2Odf::ConversionNary(OOX::Logic::CNary *pNary)
	{
		if(pNary == nullptr) return;
		std::wstring wsTypeNary;
		bool bStyle;
		StValuePr stNaryPr = ConversionNaryPr(pNary->m_oNaryPr.GetPointer(),bStyle);
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
			m_wsAnnotationStarMath += L"from {";
			ConversionSub(pNary->m_oSub.GetPointer());
			m_wsAnnotationStarMath += L"} ";
		}
		if(stNaryPr.m_bSupHide)
		{
			m_wsAnnotationStarMath += L"to {";
			ConversionSup(pNary->m_oSup.GetPointer());
			m_wsAnnotationStarMath += L"} ";
		}
		ConversionElement(pNary->m_oElement.GetPointer());
		m_pXmlWrite->WriteNodeEnd(wsTypeNary,false,false);
		if(bStyle)
			m_pXmlWrite->WriteNodeEnd(L"mstyle",false,false);
	}
	void COOXml2Odf::ConversionSsub(OOX::Logic::CSSub *pSsub)
	{
		if(pSsub == nullptr) return;
		ConversionSub(pSsub->m_oSub.GetPointer(),pSsub->m_oElement.GetPointer());
	}
	void COOXml2Odf::ConversionSub(OOX::Logic::CSub *pSub, OOX::Logic::CElement * pElement)
	{
		if(pSub == nullptr) return;
		m_pXmlWrite->WriteNodeBegin(L"msub",false);
		if(pElement != nullptr)
			ConversionElement(pElement);
		else
		{
			m_pXmlWrite->WriteNodeBegin(L"mspace",true);
			m_pXmlWrite->WriteNodeEnd(L"w",true,true);
			m_wsAnnotationStarMath += L"` ";
		}
		m_wsAnnotationStarMath += L"_";
		ConversionVectorWritingElement(pSub->m_arrItems);
		m_pXmlWrite->WriteNodeEnd(L"msub",false,false);
	}
	void COOXml2Odf::ConversionSub(OOX::Logic::CSub *pSub)
	{
		if(pSub == nullptr)
			return;
		ConversionVectorWritingElement(pSub->m_arrItems);
	}
	void COOXml2Odf::ConversionSsup(OOX::Logic::CSSup *pSsup)
	{
		if(pSsup == nullptr) return;
		ConversionSup(pSsup->m_oSup.GetPointer(),pSsup->m_oElement.GetPointer());
	}
	void COOXml2Odf::ConversionSup(OOX::Logic::CSup *pSup, OOX::Logic::CElement *pElement)
	{
		if(pSup == nullptr) return;
		m_pXmlWrite->WriteNodeBegin(L"msup",false);
		if(pElement != nullptr)
			ConversionElement(pElement);
		else
		{
			m_pXmlWrite->WriteNodeBegin(L"mspace",true);
			m_pXmlWrite->WriteNodeEnd(L"w",true,true);
			m_wsAnnotationStarMath += L"` ";
		}
		m_wsAnnotationStarMath+= L"^";
		ConversionVectorWritingElement(pSup->m_arrItems);
		m_pXmlWrite->WriteNodeEnd(L"msup",false,false);
	}
	void COOXml2Odf::ConversionSup(OOX::Logic::CSup *pSup)
	{
		if(pSup == nullptr)
			return;
		ConversionVectorWritingElement(pSup->m_arrItems);
	}
	void COOXml2Odf::ConversionElement(OOX::Logic::CElement *pElement,const bool& bMatrix)
	{
		if(pElement == nullptr) return;
		ConversionVectorWritingElement(pElement->m_arrItems,bMatrix);	
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
	StValuePr COOXml2Odf::ConversionMdPr(OOX::Logic::CDelimiterPr *pDelPr,bool& bStyle)
	{
		StValuePr pMdPr;
		if(pDelPr == nullptr)
			return pMdPr;
		bStyle = ConversionCtrlPr(pDelPr->m_oCtrlPr.GetPointer());
		pMdPr.m_wsBegBracket = ConversionBegBracket(dynamic_cast<OOX::Logic::CBegChr*>(pDelPr->m_oBegChr.GetPointer()));
		pMdPr.m_wsEndBracket = ConversionEndBracket(dynamic_cast<OOX::Logic::CEndChr*>(pDelPr->m_oEndChr.GetPointer()));
		return pMdPr;
	}
	StValuePr COOXml2Odf::ConversionNaryPr(OOX::Logic::CNaryPr *pNaryPr,bool& bStyle)
	{
		StValuePr stNaryPr;
		if(pNaryPr == nullptr) return stNaryPr;
		bStyle = ConversionCtrlPr(pNaryPr->m_oCtrlPr.GetPointer());
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
		else if(wsChr.empty()) return L"int";
		else if(wsChr == L"\u222C") return L"iint";
		else if(wsChr == L"\u222D") return L"iiint";
		else if(wsChr == L"\u222E") return L"lint";
		else if(wsChr == L"\u222F") return L"llint";
		else if(wsChr == L"\u2230") return L"lllint";
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
	bool COOXml2Odf::СomparingAttributes(StValuePr *pRight, StValuePr *pLeft)
	{
		if(pRight->m_wsColor != pLeft->m_wsColor)
			return false;
		return true;
	}
	void COOXml2Odf::ConversionAcc(OOX::Logic::CAcc *pAcc)
	{
		std::wstring wsSymbol = pAcc->m_oAccPr->m_oChr.IsInit() ? pAcc->m_oAccPr->m_oChr.get().m_val->GetValue() : L"",wsSign;
		wsSign = TranslationDiacritSign(wsSymbol);
		m_wsAnnotationStarMath += wsSign + L" ";
		m_pXmlWrite->WriteNodeBegin(L"mover",true);
		m_pXmlWrite->WriteAttribute(L"accent",L"true");
		m_pXmlWrite->WriteNodeEnd(L"w",true,false);
		ConversionElement(pAcc->m_oElement.GetPointer());
		m_pXmlWrite->WriteNodeBegin(L"mo",true);
		m_pXmlWrite->WriteAttribute(L"stretchy",L"false");
		m_pXmlWrite->WriteNodeEnd(L"w",true,false);
		m_pXmlWrite->WriteString(wsSymbol);
		m_pXmlWrite->WriteNodeEnd(L"mo",false,false);
		m_pXmlWrite->WriteNodeEnd(L"mover",false,false);
	}
	void COOXml2Odf::ConversionFunc(OOX::Logic::CFunc *pFunc)
	{
		if(pFunc == nullptr)
			return;
		bool bStyle = ConversionFuncPr(pFunc->m_oFuncPr.GetPointer());
		if(pFunc->m_oFName.GetPointer() != nullptr)
			ConversionVectorWritingElement(pFunc->m_oFName.GetPointer()->m_arrItems);
		ConversionElement(pFunc->m_oElement.GetPointer());
		if(bStyle)
			m_pXmlWrite->WriteNodeEnd(L"mstyle",false,false);
	}
	bool COOXml2Odf::ConversionFuncPr(OOX::Logic::CFuncPr *pFuncPr)
	{
		if(pFuncPr == nullptr)
			return false;
		return ConversionCtrlPr(pFuncPr->m_oCtrlPr.GetPointer());
	}
	void COOXml2Odf::RecordingMoNode(const std::wstring &wsSymbol, XmlUtils::CXmlWriter *pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"mo",true);
		pXmlWrite->WriteAttribute(L"stretchy",L"false");
		pXmlWrite->WriteNodeEnd(L"w",true,false);
		if(!wsSymbol.empty())
			pXmlWrite->WriteString(wsSymbol);
		else
			pXmlWrite->WriteString(L"\u222B");
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
		{
			delete stTempPr;
			return nullptr;
		}
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
		{
			delete stTempPr;
			return nullptr;
		}
	}
	void COOXml2Odf::ConversionTextVector(std::vector<COneElement *> &arLine, std::vector<COneElement *> &arNewLine, const bool &bMatrix)
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
					else if(pTempElement->GetType() == TypeElement::String && СomparingAttributes(pTempElement->GetAttribute(),pElement->GetAttribute()))
					{
						CNumberOrLetter* pFirst = dynamic_cast<CNumberOrLetter*>(pTempElement);
						CNumberOrLetter* pSecond = dynamic_cast<CNumberOrLetter*>(pElement);
						pFirst->AddingStrings(pSecond->GetString());
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
			for(int i = 0;i< arNewLine.size();i++)
			{
				if(bMatrix == true)
					m_pXmlWrite->WriteNodeBegin(L"mtd",false);
				arNewLine[i]->Conversion(m_pXmlWrite,m_wsAnnotationStarMath);
				if(bMatrix == true)
				{
					if(i+1<arNewLine.size())
						m_wsAnnotationStarMath += L"# ";
					m_pXmlWrite->WriteNodeEnd(L"mtd",false,false);
				}
			}
		}
	}
	void COOXml2Odf::ConversionVectorWritingElement(std::vector<OOX::WritingElement *> arWrElements,const bool& bMatrix)
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
					ConversionTextVector(arLine,arNewLine,bMatrix);
				if(bMatrix == true)
					m_pXmlWrite->WriteNodeBegin(L"mtd",false);
				NodeDefinition(arWrElements[i],true);
				if(bMatrix == true)
				{
					m_pXmlWrite->WriteNodeEnd(L"mtd",false,false);
					if(i+1 < arWrElements.size())
						m_wsAnnotationStarMath += L"# ";
				}
			}
		}
		if(!arLine.empty())
		{
			ConversionTextVector(arLine,arNewLine,bMatrix);
		}
	}
	std::wstring COOXml2Odf::TranslationDiacritSign(const std::wstring &wsSymbol)
	{
		if( L"\u0308" == wsSymbol) return L"ddot"; 
		else if(L"\u0307" == wsSymbol) return L"dot";
		else if(L"\u0301" == wsSymbol) return L"acute";
		else if(L"\u0300" == wsSymbol) return L"grave";
		else if(L"\u0306" == wsSymbol) return L"breve";
		else if(L"\u030A" == wsSymbol) return L"circle";
		else if(L"\u0304" == wsSymbol) return L"bar";
		else if(L"\u20DB" == wsSymbol) return L"dddot";
		else if(L"\u20D1" == wsSymbol) return L"harpoon";
		else if(L"\u20D7" == wsSymbol) return L"vec";
		else if(L"\u0342" == wsSymbol) return L"tilde";
		else if(L"\u0302" == wsSymbol) return L"hat";
		else if(L"\u030C" == wsSymbol) return L"check";
		else if(L"\u0305" == wsSymbol) return L"overline";
		else if(L"\u0332" == wsSymbol) return L"underline";
		else return L"";
	}
	void COOXml2Odf::ConversionMatrix(OOX::Logic::CMatrix* pMatrix)
	{
		if(pMatrix == nullptr)
			return;
		bool bStyle = ConversionCMPr(dynamic_cast<OOX::Logic::CMPr*>(pMatrix->m_arrItems[0]));
		m_wsAnnotationStarMath += L"matrix{";
		m_pXmlWrite->WriteNodeBegin(L"mtable",false);
		for(int i = 1;i<pMatrix->m_arrItems.size();i++)
		{
			if(OOX::EElementType::et_m_mr == pMatrix->m_arrItems[i]->getType())
				ConversionMr(dynamic_cast<OOX::Logic::CMr*>(pMatrix->m_arrItems[i]));
			if(i+1 < pMatrix->m_arrItems.size())
				m_wsAnnotationStarMath += L"## ";
		}
		m_pXmlWrite->WriteNodeEnd(L"mtable",false,false);
		if(bStyle == true)
			m_pXmlWrite->WriteNodeEnd(L"mstyle",false,false);
		m_wsAnnotationStarMath += L"} ";
	}
	void COOXml2Odf::ConversionMr(OOX::Logic::CMr *pMr)
	{
		if(pMr == nullptr)
			return;
		m_pXmlWrite->WriteNodeBegin(L"mtr",false);
		ConversionVectorWritingElement(pMr->m_arrItems,true);
		m_pXmlWrite->WriteNodeEnd(L"mtr",false,false);
	}
	bool COOXml2Odf::ConversionCtrlPr(OOX::Logic::CCtrlPr *pCtrlPr)
	{
		if(pCtrlPr == nullptr)
			return false;
		if(pCtrlPr->m_oRPr.GetPointer() != nullptr)
		{
			bool bStyle{false};
			StValuePr* pValue = ConversionRunProperties(dynamic_cast<OOX::Logic::CRunProperty*>(pCtrlPr->m_oRPr.GetPointer()));
			COneElement::ConversionAttribute(pValue,bStyle,m_pXmlWrite,m_wsAnnotationStarMath);
			return bStyle;
		}
		return false;
	}
	bool COOXml2Odf::ConversionCMPr(OOX::Logic::CMPr *pMPr)
	{
		if(pMPr == nullptr)
			return false;
		return ConversionCtrlPr(dynamic_cast<OOX::Logic::CCtrlPr*>(pMPr->m_oCtrlPr.GetPointer()));
	}
	std::wstring COOXml2Odf::GetOdf()
	{
		return m_pXmlWrite->GetXmlString();
	}
	std::wstring COOXml2Odf::GetAnnotation()
	{
		return m_wsAnnotationStarMath;
	}
	void COOXml2Odf::ConversionRad(OOX::Logic::CRad *pRad)
	{
		if(pRad == nullptr)
			return;
		bool bDeg{false};
		bool bStyle = ConversionRadPr(pRad->m_oRadPr.GetPointer(),bDeg);
		if(bDeg)
		{
			m_wsAnnotationStarMath += L"sqrt";
			m_pXmlWrite->WriteNodeBegin(L"sqrt",false);
			NodeDefinition(pRad->m_oElement.GetPointer());
			m_pXmlWrite->WriteNodeEnd(L"sqrt",false,false);
		}
		else if(!bDeg)
		{
			m_wsAnnotationStarMath += L"nroot";
			std::wstring wsStart = m_wsAnnotationStarMath,wsElement;
			m_wsAnnotationStarMath.clear();
			m_pXmlWrite->WriteNodeBegin(L"mroot",false);
			NodeDefinition(pRad->m_oElement.GetPointer());
			wsElement = m_wsAnnotationStarMath;
			m_wsAnnotationStarMath.clear();
			ConversionDeg(pRad->m_oDeg.GetPointer());
			wsStart += m_wsAnnotationStarMath + wsElement;
			m_wsAnnotationStarMath = wsStart;
			m_pXmlWrite->WriteNodeEnd(L"mroot",false,false);
		}
		if(bStyle)
			m_pXmlWrite->WriteNodeEnd(L"mstyle",false,false);
	}
	bool COOXml2Odf::ConversionRadPr(OOX::Logic::CRadPr *pRadPr,bool&bDeg)
	{
		if(pRadPr == nullptr)
			return false;
		bool bStyle = ConversionCtrlPr(dynamic_cast<OOX::Logic::CCtrlPr*>(pRadPr->m_oCtrlPr.GetPointer()));
		bDeg = pRadPr->m_oDegHide.GetPointer() != nullptr ? true:false;
		return bStyle;
	}
	void COOXml2Odf::ConversionDeg(OOX::Logic::CDeg *pDeg)
	{
		if(pDeg == nullptr)
			return;
		ConversionVectorWritingElement(pDeg->m_arrItems);
	}
	void COOXml2Odf::ConversionGroupChr(OOX::Logic::CGroupChr *pGroup)
	{
		if(pGroup == nullptr)
			return;
		StValuePr stGroupPr;
		stGroupPr = ConversionGroupChrPr(pGroup->m_oGroupChrPr.GetPointer());
		std::wstring wsNode = stGroupPr.m_wsChr == L"\u23DE" ? L"mover":L"munder";
		m_pXmlWrite->WriteNodeBegin(wsNode,false);
		m_pXmlWrite->WriteNodeBegin(wsNode,false);
		ConversionElement(pGroup->m_oElement.GetPointer());
		stGroupPr.m_wsChr == L"\u23DE" ? RecordingMoNode(L"\u23DE",m_pXmlWrite):RecordingMoNode(L"\u23DF",m_pXmlWrite);
		m_wsAnnotationStarMath += stGroupPr.m_wsChr == L"\u23DE" ? L"overbrace " : L"underbrace";
		m_pXmlWrite->WriteNodeEnd(wsNode,false,false);
		m_pXmlWrite->WriteNodeBegin(L"mspace",true);
		m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		m_wsAnnotationStarMath += L"` ";
		m_pXmlWrite->WriteNodeEnd(wsNode,false,false);
	}
	StValuePr COOXml2Odf::ConversionGroupChrPr(OOX::Logic::CGroupChrPr *pGroupPr)
	{
		StValuePr stGroupPr;
		if(pGroupPr == nullptr)
			return stGroupPr;
		stGroupPr.m_wsChr = pGroupPr->m_oChr.GetPointer()->m_val.GetPointer()->GetValue();
		stGroupPr.m_enPos = pGroupPr->m_oPos.GetPointer()->m_val.GetPointer()->GetValue();
		stGroupPr.m_enVert = pGroupPr->m_oVertJc.GetPointer()->m_val.GetPointer()->GetValue();
		return stGroupPr;
	}
	void COOXml2Odf::ConversionBar(OOX::Logic::CBar *pBar)
	{
		if(pBar == nullptr)
			return;
		bool bStyle{false};
		std::wstring wsNode;
		StValuePr pBarPr = ConversionBarPr(pBar->m_oBarPr.GetPointer(),bStyle);
		if(pBarPr.m_enPos == SimpleTypes::ETopBot::tbTop)
		{
			wsNode = L"mover";
			m_wsAnnotationStarMath += L"overline ";
		}
		else
		{
			wsNode = L"munder";
			m_wsAnnotationStarMath += L"underline ";
		}
		m_pXmlWrite->WriteNodeBegin(wsNode,true);
		pBarPr.m_enPos == SimpleTypes::ETopBot::tbTop ? m_pXmlWrite->WriteAttribute(L"accent",L"true"): m_pXmlWrite->WriteAttribute(L"accentunder",L"true");
		ConversionElement(pBar->m_oElement.GetPointer());
		m_pXmlWrite->WriteNodeEnd(L"w",true,false);
		m_pXmlWrite->WriteNodeBegin(L"mo",false);
		pBarPr.m_enPos == SimpleTypes::ETopBot::tbTop ? m_pXmlWrite->WriteString(L"\u203E"):m_pXmlWrite->WriteString(L"\u005F");
		m_pXmlWrite->WriteNodeEnd(L"mo",false,false);
		m_pXmlWrite->WriteNodeEnd(wsNode,false,false);
		if(bStyle)
			m_pXmlWrite->WriteNodeEnd(L"mstyle",false,false);
	}
	StValuePr COOXml2Odf::ConversionBarPr(OOX::Logic::CBarPr *pBarPr, bool &bStyle)
	{
		StValuePr stBarPr;
		if(pBarPr == nullptr)
			return stBarPr;
		stBarPr.m_enPos = pBarPr->m_oPos.GetPointer()->m_val.GetPointer()->GetValue();
		bStyle = ConversionCtrlPr(pBarPr->m_oCtrlPr.GetPointer());
		return stBarPr;		
	}
	template<typename T>
	bool IsEmptyNode(T pNode)
	{
		if(pNode == nullptr)
			return true;
		if(pNode->m_arrItems.size() == 0)
			return true;
		return false;
	}
	void COOXml2Odf::ConversionSPre(OOX::Logic::CSPre *pSPre)
	{
		if(pSPre == nullptr)
			return ;
		m_pXmlWrite->WriteNodeBegin(L"mmultiscripts",false);
		if(IsEmptyNode(pSPre->m_oElement.GetPointer()))
			EmptyBlock();
		else
			ConversionElement(pSPre->m_oElement.GetPointer());
		m_pXmlWrite->WriteNodeBegin(L"mprescripts",true);
		m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		if(IsEmptyNode(pSPre->m_oSup.GetPointer()))
		{
			m_pXmlWrite->WriteNodeBegin(L"none",true);
			m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		else
		{
			m_wsAnnotationStarMath += L"lsup ";
			ConversionSup(pSPre->m_oSup.GetPointer());						
		}
		if(IsEmptyNode(pSPre->m_oSub.GetPointer()))
		{
			m_pXmlWrite->WriteNodeBegin(L"none",true);
			m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		else
		{
			m_wsAnnotationStarMath += L"lsub ";
			ConversionSub(pSPre->m_oSub.GetPointer());
		}
		m_pXmlWrite->WriteNodeEnd(L"mmultiscripts",false,false);
	}
	void COOXml2Odf::EmptyBlock()
	{
//		m_pXmlWrite->WriteNodeBegin(L"mi",true);
//		m_pXmlWrite->WriteAttribute(L"mathvariant",L"normal");
//		m_pXmlWrite->WriteNodeEnd(L"w",true,false);
//		m_pXmlWrite->WriteString(L"\u2751");
//		m_pXmlWrite->WriteNodeEnd(L"mi",false,false);
//		m_wsAnnotationStarMath += L"{&lt;\u003F&gt;} "; 
	}
	void COOXml2Odf::ConversionLimLow(OOX::Logic::CLimLow *pLimLow)
	{
		if(pLimLow == nullptr)
			return;
		m_pXmlWrite->WriteNodeBegin(L"munder",false);
		ConversionElement(pLimLow->m_oElement.GetPointer());
		m_wsAnnotationStarMath += L"csub ";
		ConversionLim(pLimLow->m_oLim.GetPointer());
		m_pXmlWrite->WriteNodeEnd(L"munder",false,false);
	}
	void COOXml2Odf::ConversionLimUpp(OOX::Logic::CLimUpp *pLimUpp)
	{
		if(pLimUpp == nullptr)
			return;
		m_pXmlWrite->WriteNodeBegin(L"mover",false);
		ConversionElement(pLimUpp->m_oElement.GetPointer());
		m_wsAnnotationStarMath += L"csup ";
		ConversionLim(pLimUpp->m_oLim.GetPointer());
		m_pXmlWrite->WriteNodeEnd(L"mover",false,false);
	}
	void COOXml2Odf::ConversionLim(OOX::Logic::CLim *pLim)
	{
		if(pLim == nullptr)
			return;
		ConversionVectorWritingElement(pLim->m_arrItems);
	}
	void COOXml2Odf::ConversionEqArr(OOX::Logic::CEqArr *pEqArr)
	{
		if(pEqArr == nullptr)
			return;
		m_pXmlWrite->WriteNodeBegin(L"mtable",false);
		m_wsAnnotationStarMath += L"stack{";
		for(int i = 1; i<pEqArr->m_arrItems.size();i++)
		{
			m_pXmlWrite->WriteNodeBegin(L"mtr",false);
			m_pXmlWrite->WriteNodeBegin(L"mtd",false);
			NodeDefinition(pEqArr->m_arrItems[i]);
			m_pXmlWrite->WriteNodeEnd(L"mtd",false,false);
			m_pXmlWrite->WriteNodeEnd(L"mtr",false,false);
			if(pEqArr->m_arrItems.size()>1 && i+1 < pEqArr->m_arrItems.size())
				m_wsAnnotationStarMath += L"# ";
		}
		m_wsAnnotationStarMath += L"} ";
		m_pXmlWrite->WriteNodeEnd(L"mtable",false,false);
	}
	void COOXml2Odf::ConversionSubSup(OOX::Logic::CSSubSup *pSubSup)
	{
		if(pSubSup == nullptr)
			return;
		m_pXmlWrite->WriteNodeBegin(L"msubsup",false);
		ConversionElement(pSubSup->m_oElement.GetPointer());
		m_wsAnnotationStarMath += L"_ ";
		ConversionSub(pSubSup->m_oSub.GetPointer());
		m_wsAnnotationStarMath += L"^ ";
		ConversionSup(pSubSup->m_oSup.GetPointer());
		m_pXmlWrite->WriteNodeEnd(L"msubsup",false,false);
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
	void COneElement::ConversionAttribute(StValuePr* pAttribute,bool& bStyle,XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation)
	{
		if(pAttribute == nullptr)
			return;
		if(!pAttribute->m_wsColor.empty())
		{
			pXmlWrite->WriteNodeBegin(L"mstyle",true);
			std::wstring wsColor = L"#" + pAttribute->m_wsColor;
			pXmlWrite->WriteAttribute(L"mathcolor",wsColor);
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			bStyle = true;
			wsAnnotation += L"color hex " + pAttribute->m_wsColor + L" ";  
		}
		delete pAttribute;
	}
	bool COneElement::CheckStyle()
	{
		return m_bStyle;
	}
	bool& COneElement::GetStyle()
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
		COneElement::ConversionAttribute(GetAttribute(),GetStyle(),pXmlWrite,wsAnnotation);
		switch(m_enTypeElement)
		{
		case StarMath::TypeElement::number:
		{
			pXmlWrite->WriteNodeBegin(L"mn",false);
			pXmlWrite->WriteString(m_wsElement);
			pXmlWrite->WriteNodeEnd(L"mn",false,false);
			wsAnnotation += L'{' + m_wsElement + L"} ";
			break;
		}
		case StarMath::TypeElement::letter:
		{
			pXmlWrite->WriteNodeBegin(L"mi",true);
			pXmlWrite->WriteAttribute(L"mathvariant",L"italic");
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			pXmlWrite->WriteString(m_wsElement);
			pXmlWrite->WriteNodeEnd(L"mi",false,false);
			wsAnnotation += m_wsElement + L" ";
			break;
		}
		default:
		break;
		}
		if(CheckStyle())
			pXmlWrite->WriteNodeEnd(L"mstyle",false,false);
//		wsAnnotation += L'{' + m_wsElement + L"} ";
	}
	void CNumberOrLetter::Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{}
	const std::wstring& CNumberOrLetter::GetString()
	{
		return m_wsElement;
	}
	void CNumberOrLetter::AddingStrings(const std::wstring &wsString)
	{
		m_wsElement += wsString;
	}
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
		COneElement::ConversionAttribute(GetAttribute(),GetStyle(),pXmlWrite,wsAnnotation);
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
