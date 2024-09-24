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
	std::vector<COneElement*> COOXml2Odf::ConversionMT(OOX::Logic::CMText *pMt,const StValuePr* pValue)
	{
		std::vector<COneElement*> arLine;
		if(pMt == nullptr)
			return arLine;
		std::wstring wsText = pMt->m_sText,wsElement;
		std::wstring::iterator itStart = wsText.begin(),itEnd = wsText.end();
		COneElement* pRightTempElement = nullptr;
		while(itStart != itEnd)
		{
			COneElement* pTempElement;
			if(pRightTempElement == nullptr)
			{
				wsElement = ParsingText(itStart,itEnd);
				pTempElement = COneElement::CreateElement(wsElement);
			}
			else
			{
				pTempElement = pRightTempElement;
				pRightTempElement = nullptr;
			}
			if(pTempElement != nullptr)
			{
				if(pTempElement->GetType() == TypeElement::BinOperator)
				{
					CBinOperator* pBinOp = dynamic_cast<CBinOperator*>(pTempElement);
					pBinOp->Parse(itStart,itEnd,pRightTempElement);
					if(!arLine.empty() && pBinOp->GetTypeBinOp() != TypeElement::neg)
					{
						if(CBinOperator::CheckRightArg(arLine.back()) && !CBinOperator::UnaryCheck(pValue,pBinOp->GetTypeBinOp()))
						{
							pBinOp->SetLeftArg(arLine.back());
							arLine.pop_back();
						}
					}
				}
				else if(pTempElement->GetType() == TypeElement::Connection)
				{
					CRelationsAndOperationsOnSets* pRelation = dynamic_cast<CRelationsAndOperationsOnSets*>(pTempElement);
					pRelation->Parse(itStart,itEnd,pRightTempElement);
					if(!arLine.empty() && CBinOperator::CheckRightArg(arLine.back()))
					{
						pRelation->SetLeftArg(arLine.back());
						arLine.pop_back();
					}
				}
				arLine.push_back(pTempElement);
			}
		}
		return arLine;
	}
	std::wstring COOXml2Odf::ParsingText(std::wstring::iterator &itStart, std::wstring::iterator &itEnd)
	{
		std::wstring wsElement;
		for(;itStart != itEnd;itStart++)
		{
			if(iswspace(*itStart))
					if(!wsElement.empty())
						return wsElement;
					else
					{
						itStart++;
						return L" ";
					}
			else if(iswdigit(*itStart))
				if(!wsElement.empty())
				{
					if(iswdigit(wsElement.back()))
						wsElement.push_back(*itStart);
					else 
						return wsElement;
				}
				else
					wsElement.push_back(*itStart);
			else if(iswalpha(*itStart))
				if(!wsElement.empty())
				{
					if(iswalpha(wsElement.back()))
						wsElement.push_back(*itStart);
					else
						return wsElement;
				}
				else
					wsElement.push_back(*itStart);
			else if(wsElement.empty())
			{
				wsElement.push_back(*itStart);
				itStart++;
				return wsElement;
			}
			else 
				return wsElement;
		}
		if(!wsElement.empty())
			return wsElement;
		else return L"";
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
			m_wsAnnotationStarMath += L"{ ";
			ConversionVectorWritingElement(pMf->m_oNum.GetPointer()->m_arrItems);
			m_wsAnnotationStarMath += L"} ";
			m_pXmlWrite->WriteNodeEnd(L"mtd",false,false);
			m_pXmlWrite->WriteNodeEnd(L"mtr",false,false);
			m_pXmlWrite->WriteNodeBegin(L"mtr",false);
			m_pXmlWrite->WriteNodeBegin(L"mtd",false);
			m_wsAnnotationStarMath += L"{ ";
			ConversionVectorWritingElement(pMf->m_oDen.GetPointer()->m_arrItems);
			m_wsAnnotationStarMath += L"} ";
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
				m_wsAnnotationStarMath += L"{ ";
				ConversionVectorWritingElement(pMf->m_oNum.GetPointer()->m_arrItems);
				m_wsAnnotationStarMath += L"} wideslash { ";
				ConversionVectorWritingElement(pMf->m_oDen.GetPointer()->m_arrItems);
				m_wsAnnotationStarMath += L"} ";
			}
			else
			{
				m_pXmlWrite->WriteNodeEnd(L"w",true,false);
				m_wsAnnotationStarMath += L"{ ";
				ConversionVectorWritingElement(pMf->m_oNum.GetPointer()->m_arrItems);
				m_wsAnnotationStarMath += L"} over { ";
				ConversionVectorWritingElement(pMf->m_oDen.GetPointer()->m_arrItems);
				m_wsAnnotationStarMath += L"} ";
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
		unsigned int iStyle;
		if(pDel->m_arrItems[0] != nullptr && pDel->m_arrItems[0]->getType() == OOX::EElementType::et_m_dPr)
			stDelPr = ConversionMdPr(dynamic_cast<OOX::Logic::CDelimiterPr*>(pDel->m_arrItems[0]),iStyle);
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
		StyleClosing(iStyle,m_pXmlWrite);
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
		unsigned int iStyle(0);
		StValuePr stNaryPr = ConversionNaryPr(pNary->m_oNaryPr.GetPointer(),iStyle);
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
		StyleClosing(iStyle,m_pXmlWrite);
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
		m_wsAnnotationStarMath += L"_ {";
		ConversionVectorWritingElement(pSub->m_arrItems);
		m_wsAnnotationStarMath += L"} ";
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
		m_wsAnnotationStarMath+= L"^ {";
		ConversionVectorWritingElement(pSup->m_arrItems);
		m_wsAnnotationStarMath += L"} ";
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
		if(pMRun->m_oMRPr.GetPointer() != nullptr)
			ConversionMRunProperties(pMRun->m_oMRPr.GetPointer(),stRpr);
		if(pMRun->m_oMText.GetPointer() != nullptr)
		{
			arLine = ConversionMT(pMRun->m_oMText.GetPointer(),stRpr);
			for(int i = 0; i<arLine.size();i++)
			{
				if(arLine[i]->GetType()!= TypeElement::Connection && arLine[i]->GetType()!= TypeElement::Empty)
				{
					if(i != 0 )
					{
							arLine[i]->SetAttribute(stRpr);
							stRpr->AddRef();
					}
					else if(i == 0 )
						arLine[i]->SetAttribute(stRpr);
				}
			}
		}
		return arLine;
	}
	StValuePr COOXml2Odf::ConversionMdPr(OOX::Logic::CDelimiterPr *pDelPr, unsigned int &iStyle)
	{
		StValuePr pMdPr;
		if(pDelPr == nullptr)
			return pMdPr;
		iStyle = ConversionCtrlPr(pDelPr->m_oCtrlPr.GetPointer());
		pMdPr.m_wsBegBracket = ConversionBegBracket(dynamic_cast<OOX::Logic::CBegChr*>(pDelPr->m_oBegChr.GetPointer()));
		pMdPr.m_wsEndBracket = ConversionEndBracket(dynamic_cast<OOX::Logic::CEndChr*>(pDelPr->m_oEndChr.GetPointer()));
		return pMdPr;
	}
	StValuePr COOXml2Odf::ConversionNaryPr(OOX::Logic::CNaryPr *pNaryPr,unsigned int& iStyle)
	{
		StValuePr stNaryPr;
		if(pNaryPr == nullptr) return stNaryPr;
		iStyle = ConversionCtrlPr(pNaryPr->m_oCtrlPr.GetPointer());
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
	bool COOXml2Odf::СomparingAttributes(StValuePr *pRight, StValuePr *pLeft)
	{
		return !(pRight == nullptr || pLeft == nullptr || pRight->m_wsColor != pLeft->m_wsColor);
	}
	void COOXml2Odf::ConversionAcc(OOX::Logic::CAcc *pAcc)
	{
		std::wstring wsSymbol = pAcc->m_oAccPr->m_oChr.IsInit() ? pAcc->m_oAccPr->m_oChr.get().m_val->GetValue() : L"",wsSign;
		wsSign = TranslationDiacritSign(wsSymbol);
		if(wsSign.empty())
		{
			m_pXmlWrite->WriteNodeBegin(L"mover",false);
			ConversionElement(pAcc->m_oElement.GetPointer());
			m_pXmlWrite->WriteNodeBegin(L"mtext",false);
			m_pXmlWrite->WriteString(wsSymbol);
			m_pXmlWrite->WriteNodeEnd(L"mtext",false,false);
			m_wsAnnotationStarMath += L"csup ";
			m_wsAnnotationStarMath += L"\u0026quot;" + wsSymbol + L"\u0026quot; ";
		}
		else
		{
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
		}
		m_pXmlWrite->WriteNodeEnd(L"mover",false,false);
	}
	void COOXml2Odf::ConversionFunc(OOX::Logic::CFunc *pFunc)
	{
		if(pFunc == nullptr)
			return;
		m_pXmlWrite->WriteNodeBegin(L"mrow",false);
		if(pFunc->m_oFName.GetPointer() != nullptr)
			ConversionVectorWritingElement(pFunc->m_oFName.GetPointer()->m_arrItems);
		ConversionElement(pFunc->m_oElement.GetPointer());
		m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
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
			stTempPr->Release();
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
		if(pRPr->m_oSz.GetPointer() != nullptr)
		{
			bRpr = true;
			stTempPr->m_iSize = pRPr->m_oSz.GetPointer()->m_oVal.GetPointer()->GetValue();
		}
		if(pRPr->m_oRFonts.GetPointer()!= nullptr)
		{
			std::wstring wsFont = pRPr->m_oRFonts.GetPointer()->m_sAscii.get();
			if(L"Liberation Sans" == wsFont)
			{
				stTempPr->m_enFont = StarMath::TypeFont::sans;
				bRpr = true;
			}
			else if(L"Liberation Serif" == wsFont) 
			{
				stTempPr->m_enFont = StarMath::TypeFont::serif;
				bRpr = true;
			}
			else if(L"Liberation Mono" == wsFont)
			{
				stTempPr->m_enFont = StarMath::TypeFont::fixed;
				bRpr = true;
			}
			else stTempPr->m_enFont = StarMath::TypeFont::empty;
		}
		if(bRpr == true)
			return stTempPr;
		else
		{
//			delete stTempPr;
			stTempPr->Release();
			return nullptr;
		}
	}
	void COOXml2Odf::ConversionMRunProperties(OOX::Logic::CMRPr *pMRpr, StValuePr *pValue)
	{
		if(pMRpr == nullptr)
			return ;
		if(pMRpr->m_oSty.GetPointer() != nullptr)
		{
			if(pValue == nullptr)
				pValue = new StValuePr;
			pValue->m_enStyle = pMRpr->m_oSty.GetPointer()->m_val.GetPointer()->GetValue();
		}
	}
	void COOXml2Odf::ConversionARpr(PPTX::Logic::RunProperties *pARpr, StValuePr *pValue)
	{
		if(pARpr == nullptr)
			return;
		if(pValue == nullptr)
			pValue = new StValuePr;
	}
	void COOXml2Odf::ConversionTextVector(std::vector<COneElement *> &arLine, std::vector<COneElement *> &arNewLine, const bool &bMatrix)
	{
		if(!arLine.empty())
		{
			COneElement* pTempElement = nullptr;
			for(COneElement* pElement:arLine)
			{
				if(pElement->GetType() == TypeElement::String || pElement->GetType() == TypeElement::SpecialSymbol)
				{
					if(pTempElement == nullptr)
						pTempElement = pElement;
					else if(pTempElement->GetType() == TypeElement::BinOperator || pTempElement->GetType() == TypeElement::Connection)
					{
						if(!CBinOperator::SetRightArg(pTempElement,pElement))
						{
							arNewLine.push_back(pTempElement);
							pTempElement = pElement;
						}
					}
					else if(pElement->GetType() ==TypeElement::String && pTempElement->GetType() == TypeElement::String && СomparingAttributes(pTempElement->GetAttribute(),pElement->GetAttribute()))
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
				else if(pElement->GetType() == TypeElement::BinOperator || pElement->GetType() == TypeElement::Connection)
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
				if(arNewLine[i] != nullptr)
					arNewLine[i]->Conversion(m_pXmlWrite,m_wsAnnotationStarMath);
				if(bMatrix == true)
				{
					if(i+1<arNewLine.size())
						m_wsAnnotationStarMath += L"# ";
					m_pXmlWrite->WriteNodeEnd(L"mtd",false,false);
				}
			}
		}
		arLine.clear();
		arNewLine.clear();
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
		unsigned int iStyle = ConversionCMPr(dynamic_cast<OOX::Logic::CMPr*>(pMatrix->m_arrItems[0]));
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
		StyleClosing(iStyle,m_pXmlWrite);
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
	unsigned int COOXml2Odf::ConversionCtrlPr(OOX::Logic::CCtrlPr *pCtrlPr)
	{
		if(pCtrlPr == nullptr)
			return 0;
		if(pCtrlPr->m_oRPr.GetPointer() != nullptr)
		{
			unsigned int iStyle(0);
			StValuePr* pValue = ConversionRunProperties(dynamic_cast<OOX::Logic::CRunProperty*>(pCtrlPr->m_oRPr.GetPointer()));
			COneElement::ConversionAttribute(pValue,iStyle,m_pXmlWrite,m_wsAnnotationStarMath);
			return iStyle;
		}
		return 0;
	}
	unsigned int COOXml2Odf::ConversionCMPr(OOX::Logic::CMPr *pMPr)
	{
		if(pMPr == nullptr)
			return 0;
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
		unsigned int iStyle = ConversionRadPr(pRad->m_oRadPr.GetPointer(),bDeg);
		if(bDeg)
		{
			m_wsAnnotationStarMath += L"sqrt ";
			m_pXmlWrite->WriteNodeBegin(L"sqrt",false);
			m_wsAnnotationStarMath += L"{ ";
			NodeDefinition(pRad->m_oElement.GetPointer());
			m_wsAnnotationStarMath += L"} ";
			m_pXmlWrite->WriteNodeEnd(L"sqrt",false,false);
		}
		else if(!bDeg)
		{
			m_wsAnnotationStarMath += L"nroot ";
			std::wstring wsStart = m_wsAnnotationStarMath,wsElement;
			m_wsAnnotationStarMath.clear();
			m_pXmlWrite->WriteNodeBegin(L"mroot",false);
			m_wsAnnotationStarMath += L"{ ";
			NodeDefinition(pRad->m_oElement.GetPointer());
			m_wsAnnotationStarMath += L"} ";
			wsElement = m_wsAnnotationStarMath;
			m_wsAnnotationStarMath.clear();
			m_wsAnnotationStarMath += L"{ ";
			ConversionDeg(pRad->m_oDeg.GetPointer());
			m_wsAnnotationStarMath += L"} ";
			wsStart += m_wsAnnotationStarMath + wsElement;
			m_wsAnnotationStarMath = wsStart;
			m_pXmlWrite->WriteNodeEnd(L"mroot",false,false);
		}
		StyleClosing(iStyle,m_pXmlWrite);
	}
	unsigned int COOXml2Odf::ConversionRadPr(OOX::Logic::CRadPr *pRadPr,bool&bDeg)
	{
		if(pRadPr == nullptr)
			return 0;
		bDeg = pRadPr->m_oDegHide.GetPointer() != nullptr ? true:false;
		return ConversionCtrlPr(dynamic_cast<OOX::Logic::CCtrlPr*>(pRadPr->m_oCtrlPr.GetPointer()));
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
		std::wstring wsNode(L""),wsAnnotation = GroupChrForAnnotation(stGroupPr.m_wsChr);
		if(!wsAnnotation.empty())
		{
			wsNode = stGroupPr.m_wsChr == L""? L"munder":L"mover";
		}
		else
		{
			switch(stGroupPr.m_enPos)
			{
			case SimpleTypes::ETopBot::tbBot:
			{
				wsNode = L"munder";
				break;
			}
			case SimpleTypes::ETopBot::tbTop:
			{
				wsNode = L"mover";
				break;
			}
			}
		}
		m_pXmlWrite->WriteNodeBegin(wsNode,false);
		if(!wsAnnotation.empty())
		{
			m_pXmlWrite->WriteNodeBegin(wsNode,false);
			ConversionElement(pGroup->m_oElement.GetPointer());
			RecordingMoNode(stGroupPr.m_wsChr,m_pXmlWrite);
			m_wsAnnotationStarMath += wsAnnotation;
			m_pXmlWrite->WriteNodeEnd(wsNode,false,false);
			m_pXmlWrite->WriteNodeBegin(L"mspace",true);
			m_pXmlWrite->WriteNodeEnd(L"w",true,true);
			m_wsAnnotationStarMath += L"` ";
		}
		else
		{
			ConversionElement(pGroup->m_oElement.GetPointer());
			m_pXmlWrite->WriteNodeBegin(L"mtext",false);
			m_pXmlWrite->WriteString(stGroupPr.m_wsChr);
			m_pXmlWrite->WriteNodeEnd(L"mtext",false,false);
			m_wsAnnotationStarMath += wsNode == L"mover"? L"csup ":L"csub ";
			m_wsAnnotationStarMath += L"\u0026quot;" + stGroupPr.m_wsChr + L"\u0026quot; ";
		}
		m_pXmlWrite->WriteNodeEnd(wsNode,false,false);
	}
	StValuePr COOXml2Odf::ConversionGroupChrPr(OOX::Logic::CGroupChrPr *pGroupPr)
	{
		StValuePr stGroupPr;
		if(pGroupPr == nullptr)
			return stGroupPr;
		if(pGroupPr->m_oChr.GetPointer()!= nullptr)
			stGroupPr.m_wsChr = pGroupPr->m_oChr.GetPointer()->m_val.GetPointer()->GetValue();
		if(pGroupPr->m_oPos.GetPointer() != nullptr)
			stGroupPr.m_enPos = pGroupPr->m_oPos.GetPointer()->m_val.GetPointer()->GetValue();
		if(pGroupPr->m_oVertJc.GetPointer() != nullptr)
			stGroupPr.m_enVert = pGroupPr->m_oVertJc.GetPointer()->m_val.GetPointer()->GetValue();
		return stGroupPr;
	}
	std::wstring COOXml2Odf::GroupChrForAnnotation(const std::wstring &wsChr)
	{
		if(wsChr == L"\u23DE") return L"overbrace ";
		else if(L"" == wsChr) return L"underbrace ";
		else return L"";
	}
	void COOXml2Odf::ConversionBar(OOX::Logic::CBar *pBar)
	{
		if(pBar == nullptr)
			return;
		unsigned int iStyle(0);
		std::wstring wsNode;
		StValuePr pBarPr = ConversionBarPr(pBar->m_oBarPr.GetPointer(),iStyle);
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
		StyleClosing(iStyle,m_pXmlWrite);
	}
	StValuePr COOXml2Odf::ConversionBarPr(OOX::Logic::CBarPr *pBarPr, unsigned int &iStyle)
	{
		StValuePr stBarPr;
		if(pBarPr == nullptr)
			return stBarPr;
		if(pBarPr->m_oPos.GetPointer() != nullptr)
			stBarPr.m_enPos = pBarPr->m_oPos.GetPointer()->m_val.GetPointer()->GetValue();
		iStyle = ConversionCtrlPr(pBarPr->m_oCtrlPr.GetPointer());
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
			EmptyBlock(m_pXmlWrite,m_wsAnnotationStarMath);
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
	void COOXml2Odf::EmptyBlock(XmlUtils::CXmlWriter* pXmlWrite, std::wstring& wsAnnotation)
	{
		pXmlWrite->WriteNodeBegin(L"mi",true);
		pXmlWrite->WriteAttribute(L"mathvariant",L"normal");
		pXmlWrite->WriteNodeEnd(L"w",true,false);
		pXmlWrite->WriteString(L"\u2751");
		pXmlWrite->WriteNodeEnd(L"mi",false,false);
		wsAnnotation += L"{\u0026lt;\u003F\u0026gt;} "; 
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
	void COOXml2Odf::StyleClosing(const unsigned int& iStyle, XmlUtils::CXmlWriter *pXmlWrite)
	{
		if(iStyle != 0)
		{
			unsigned int k(0);
			while(k < iStyle)
			{
				pXmlWrite->WriteNodeEnd(L"mstyle",false,false);
				k++;
			}
		}
	}
//class COneElement
	COneElement::COneElement():m_stAttribute(nullptr),m_iStyle(0)
	{}
	COneElement::COneElement(std::wstring &wsOneElement):m_stAttribute(nullptr),m_iStyle(0)
	{
//		this = CreateElement(wsOneElement);
	}
	COneElement::~COneElement()
	{}
	COneElement* COneElement::CreateElement(std::wstring &wsOneElement)
	{
		if(wsOneElement == L"")
			return nullptr;
		if(COOXml2Odf::IsDigit(wsOneElement))
			return new CNumberOrLetter(wsOneElement,StarMath::TypeElement::number);
		else if(COOXml2Odf::IsAlpha(wsOneElement))
			return new CNumberOrLetter(wsOneElement,StarMath::TypeElement::letter);
		else if(wsOneElement == L" ")
			return new CSpace();
		std::wstring wsAnnotation =  CBinOperator::BinOperatorSymbolForAnnotation(wsOneElement);
		if(wsAnnotation != L"")
			return new CBinOperator(wsOneElement,wsAnnotation);
		else
			wsAnnotation.clear();
		wsAnnotation = CSpecialChar::DefinitionSpecialChar(wsOneElement);
		if(wsAnnotation != L"")
			return new CSpecialChar(wsOneElement,wsAnnotation);
		else
			wsAnnotation.clear();
		wsAnnotation = CRelationsAndOperationsOnSets::TransformationForAnnotation(wsOneElement);
		if(wsAnnotation != L"")
			return new CRelationsAndOperationsOnSets(wsOneElement,wsAnnotation);
		return new CSpecialChar(wsOneElement,L"");
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
	void COneElement::ConversionAttribute(StValuePr* pAttribute, unsigned int &iStyle, XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation)
	{
		if(pAttribute == nullptr)
			return;
		if(!pAttribute->m_wsColor.empty())
		{
			pXmlWrite->WriteNodeBegin(L"mstyle",true);
			std::wstring wsColor = L"#" + pAttribute->m_wsColor;
			pXmlWrite->WriteAttribute(L"mathcolor",wsColor);
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			wsAnnotation += L"color hex " + pAttribute->m_wsColor + L" ";
			iStyle++;
		}
		if(pAttribute->m_iSize != 0)
		{
			std::wstring wsSize = std::to_wstring(pAttribute->m_iSize);
			pXmlWrite->WriteNodeBegin(L"mstyle",true);
			pXmlWrite->WriteAttribute(L"mathsize",wsSize);
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			wsAnnotation += L"size " + wsSize + L" ";
			iStyle++;
		}
		if(pAttribute->m_enStyle != SimpleTypes::EStyle::stylePlain)
		{
			std::wstring wsStyle;
			switch(pAttribute->m_enStyle)
			{
			case SimpleTypes::EStyle::styleBold:
			{
				wsStyle = L"bold";
				wsAnnotation += L"bold ";
				break;
			}
			case SimpleTypes::EStyle::styleItalic:
			{
				wsStyle = L"italic";
				wsAnnotation += L"ital ";
				break;
			}
			case SimpleTypes::EStyle::styleBoldItalic:
			{
				wsStyle = L"bold-italic";
				wsAnnotation += L"bold ital ";
				break;
			}
			}
			pXmlWrite->WriteNodeBegin(L"mstyle",true);
			pXmlWrite->WriteAttribute(L"mathvariant",wsStyle);
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			iStyle++;
		}
		if(pAttribute->m_enFont != StarMath::TypeFont::empty)
		{
			std::wstring wsFont;
			switch(pAttribute->m_enFont)
			{
			case StarMath::TypeFont::fixed:
			{
				wsFont = L"monospace";
				break;
			}
			case StarMath::TypeFont::sans:
			{
				wsFont = L"sans-serif";
				break;
			}
			case StarMath::TypeFont::serif:
			{
				wsFont = L"normal";
				break;
			}
			}
			pXmlWrite->WriteNodeBegin(L"mstyle",true);
			pXmlWrite->WriteAttribute(L"mathvariant",wsFont);
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			iStyle++;
		}
//		delete pAttribute;
		pAttribute->Release();
	}
	bool COneElement::CheckStyle()
	{
		return (m_iStyle != 0);
	}
	unsigned int COneElement::GetStyle()
	{
		return m_iStyle;
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
		unsigned int iStyle(0);
		bool bCloseBracket(false);
		COneElement::ConversionAttribute(GetAttribute(),iStyle,pXmlWrite,wsAnnotation);
		if(GetAttribute()!= nullptr && m_wsElement.size()>1)
		{
			wsAnnotation += L"{ ";
			bCloseBracket = true;
		}
		switch(m_enTypeElement)
		{
		case StarMath::TypeElement::number:
		{
			pXmlWrite->WriteNodeBegin(L"mn",false);
			pXmlWrite->WriteString(m_wsElement);
			pXmlWrite->WriteNodeEnd(L"mn",false,false);
			wsAnnotation += m_wsElement + L" ";
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
		if(bCloseBracket)
			wsAnnotation += L"} ";
		COOXml2Odf::StyleClosing(iStyle,pXmlWrite);
	}
	void CNumberOrLetter::Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd, COneElement *&pElement)
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
	void CBinOperator::Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd, COneElement*& pElement)
	{
		std::wstring wsTempElement = COOXml2Odf::ParsingText(itStart,itEnd);
		COneElement* pointElement;
		pointElement = COneElement::CreateElement(wsTempElement);
		if(pointElement != nullptr && pointElement->GetType() == TypeElement::Connection && pointElement->GetType() == TypeElement::BinOperator)
			pElement = pointElement;
		else
			m_pRightArg = pointElement;
	}
	void CBinOperator::Conversion(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation)
	{
		unsigned int iStyle(0);
		pXmlWrite->WriteNodeBegin(L"mrow",false);
		if(GetAttribute() != nullptr && m_enTypeBinOp != TypeElement::undefine)
			COneElement::ConversionAttribute(GetAttribute(),iStyle,pXmlWrite,wsAnnotation);
		else
		{
			if(m_pLeftArg != nullptr)
				m_pLeftArg->Conversion(pXmlWrite,wsAnnotation);
		}
		COOXml2Odf::RecordingMoNode(m_wsSymbolBinOp,pXmlWrite);
		wsAnnotation += m_wsAnnotation + L" ";
		if(m_pRightArg != nullptr)
			m_pRightArg->Conversion(pXmlWrite,wsAnnotation);
		COOXml2Odf::StyleClosing(iStyle,pXmlWrite);
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
	bool CBinOperator::CheckRightArg(COneElement* pElement)
	{
		if(pElement == nullptr)
			return false;
		switch(pElement->GetType())
		{
		case TypeElement::BinOperator:
		{
			CBinOperator* pTemp = dynamic_cast<CBinOperator*>(pElement);
			return pTemp->CheckRightArg();
		}
		case TypeElement::Connection:
		{
			CRelationsAndOperationsOnSets* pTemp = dynamic_cast<CRelationsAndOperationsOnSets*>(pElement);
			return pTemp->CheckRightArg();
		}
		default:
		return true;
		}
	}
	bool CBinOperator::CheckLeftArg()
	{
		return (m_pLeftArg !=nullptr);
	}
	template<typename T>
	bool SetRightArg(T* pElement,COneElement* pRightArg)
	{
		if(!pElement->CheckRightArg())
		{
			pElement->SetRightArg(pRightArg);
			return true;
		}
		return false;
	}
	bool CBinOperator::SetRightArg(COneElement *pBinOp, COneElement *pRightArg)
	{
		switch(pBinOp->GetType())
		{
		case TypeElement::BinOperator:
		{
			return StarMath::SetRightArg(dynamic_cast<CBinOperator*>(pBinOp),pRightArg);
		}
		case TypeElement::Connection:
		{
			return StarMath::SetRightArg(dynamic_cast<CRelationsAndOperationsOnSets*>(pBinOp),pRightArg);
		}
		default:
		return false;
		}
	}
	bool CBinOperator::SetLeftArg(COneElement *pBinOp, COneElement *pLeftArg)
	{
		switch(pBinOp->GetType())
		{
		case TypeElement::BinOperator:
		{
			CBinOperator* pTemp = dynamic_cast<CBinOperator*>(pBinOp);
			switch(pTemp->GetTypeBinOp())
			{
			case TypeElement::neg:
				return false;
			case TypeElement::plus:
			case TypeElement::minus:
			case TypeElement::plus_minus:
			case TypeElement::minus_plus:
			{
				if(pTemp->GetAttribute() == nullptr)
					if(!pTemp->CheckLeftArg() && CBinOperator::CheckRightArg(pLeftArg))
					{
						pTemp->SetLeftArg(pLeftArg);
						return true;
					}
				return false;
			}
			default:
			if(!pTemp->CheckLeftArg() && CBinOperator::CheckRightArg(pLeftArg))
			{
				pTemp->SetLeftArg(pLeftArg);
				return true;
			}
			else
				return false;
			}
		}
		case TypeElement::Connection:
		{
			CRelationsAndOperationsOnSets* pTemp = dynamic_cast<CRelationsAndOperationsOnSets*>(pBinOp);
			if(!pTemp->CheckLeftArg() && CBinOperator::CheckRightArg(pLeftArg))
			{
				pTemp->SetLeftArg(pLeftArg);
				return true;
			}
			return false;
		}
		}
	}
	TypeElement CBinOperator::GetTypeBinOp()
	{
		return m_enTypeBinOp;
	}
	std::wstring CBinOperator::BinOperatorSymbolForAnnotation(const std::wstring &wsSymbol)
	{
		if(wsSymbol == L"+") return wsSymbol;
		else if(wsSymbol == L"-") return wsSymbol;
		else if(L"\u00B1" == wsSymbol) return L"+-";
		else if(L"\u2213" == wsSymbol) return L"-+";
		else if(L"*" == wsSymbol) return wsSymbol;
		else if(L"\u00B7" == wsSymbol) return L"cdot";
		else if(L"\u00D7" == wsSymbol) return L"times";
		else if(L"\u00F7" == wsSymbol) return L"div";
		else if(L"\u2295" == wsSymbol) return L"oplus";
		else if(L"\u2296" == wsSymbol) return L"ominus";
		else if(L"\u2299" == wsSymbol) return L"odot";
		else if(L"\u2297" == wsSymbol) return L"otimes";
		else if(L"\u2298" == wsSymbol) return L"odivide";
		else if(L"\u2218" == wsSymbol) return L"circ";
		else if(L"\u2227" == wsSymbol) return L"and";
		else if(L"\u2228" == wsSymbol) return L"or";
		else if(L"\u00AC" == wsSymbol) return L"neg";
		else return L"";
	}
	bool CBinOperator::UnaryCheck(const StValuePr *pValue, const TypeElement &enType)
	{
		if(pValue != nullptr)
		{
			switch(enType)
			{
			case TypeElement::plus:
			case TypeElement::plus_minus:
			case TypeElement::minus:
			case TypeElement::minus_plus:
			return true;
			default:
			return false;
			}
		}
		else
			return false;
	}
//class RelationAndOperation
	CRelationsAndOperationsOnSets::~CRelationsAndOperationsOnSets()
	{
		delete m_pLeftArg;
		delete m_pRightArg;
	}
	void CRelationsAndOperationsOnSets::Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd, COneElement *&pElement)
	{
		std::wstring wsTempElement = COOXml2Odf::ParsingText(itStart,itEnd);
		COneElement* pTempElement;
		pTempElement = COneElement::CreateElement(wsTempElement);
		if(pTempElement != nullptr && pTempElement->GetType() == TypeElement::Connection && pTempElement->GetType() == TypeElement::BinOperator)
			pElement = pTempElement;
		else
			m_pRightArg = pTempElement;
	}
	void CRelationsAndOperationsOnSets::Conversion(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation)
	{
		unsigned int iStyle = 0;
		pXmlWrite->WriteNodeBegin(L"mrow",false);
		if(m_pLeftArg!= nullptr)
			m_pLeftArg->Conversion(pXmlWrite,wsAnnotation);
		if(m_wsAnnotationSymbol == L"\u0026lt;" || m_wsAnnotationSymbol == L"\u0026gt;")
			COOXml2Odf::RecordingMoNode(m_wsAnnotationSymbol,pXmlWrite);
		else
			COOXml2Odf::RecordingMoNode(m_wsSymbol,pXmlWrite);
		wsAnnotation += m_wsAnnotationSymbol + L" ";
		if(m_pRightArg != nullptr)
			m_pRightArg->Conversion(pXmlWrite,wsAnnotation);
		if(iStyle != 0)
		{
			unsigned int k = 0;
			while(k < iStyle)
			{
				pXmlWrite->WriteNodeEnd(L"mstyle",false,false);
				k++;
			}
		}
		pXmlWrite->WriteNodeEnd(L"mrow",false,false);
	}
	void CRelationsAndOperationsOnSets::SetLeftArg(COneElement *pElement)
	{
		m_pLeftArg = pElement;
	}
	void CRelationsAndOperationsOnSets::SetRightArg(COneElement *pElement)
	{
		m_pRightArg = pElement;
	}
	bool CRelationsAndOperationsOnSets::CheckLeftArg()
	{
		return (m_pLeftArg != nullptr);
	}
	bool CRelationsAndOperationsOnSets::CheckRightArg()
	{
		return (m_pRightArg != nullptr);
	}
	std::wstring CRelationsAndOperationsOnSets::TransformationForAnnotation(std::wstring &wsSymbol)
	{
		if(wsSymbol == L"=")
			return L"= ";
		else if(wsSymbol == L"\u2260")
			return L"\u0026lt;\u0026gt; ";
		else if(wsSymbol == L"\u003C")
			return L"\u0026lt;";
		else if(wsSymbol == L"\u2264")
			return L"\u0026lt;= ";
		else if(wsSymbol == L"\u2A7D")
			return L"leslant ";
		else if(wsSymbol == L"\u003E")
			return L"\u0026gt;";
		else if(wsSymbol == L"\u2265")
			return L"\u0026gt;= ";
		else if(wsSymbol == L"\u2A7E")
			return L"geslant ";
		else if(wsSymbol == L"\u226A")
			return L"\u0026lt;\u0026lt; ";
		else if(wsSymbol == L"\u226B")
			return L"\u0026gt;\u0026gt; ";
		else if(wsSymbol == L"\u2248")
			return L"approx ";
		else if(wsSymbol == L"\u223C")
			return L"sim ";
		else if(wsSymbol == L"\u2243")
			return L"simeq ";
		else if(wsSymbol == L"\u2261")
			return L"equiv ";
		else if(wsSymbol == L"\u221D")
			return L"prop ";
		else if(wsSymbol == L"\u2225")
			return L"parallel ";
		else if(wsSymbol == L"\u22A5")
			return L"ortho ";
		else if(wsSymbol == L"\u2223")
			return L"divides ";
		else if(wsSymbol == L"\u2224")
			return L"ndivides ";
		else if(wsSymbol == L"\u2192")
			return L"toward ";
		else if(wsSymbol == L"\u22B7")
			return L"transl ";
		else if(wsSymbol == L"\u22B6")
			return L"transr ";
		else if(wsSymbol == L"\u225D")
			return L"def ";
		else if(wsSymbol == L"\u21D0")
			return L"dlarrow ";
		else if(wsSymbol == L"\u21D4")
			return L"dlrarrow ";
		else if(wsSymbol == L"\u21D2")
			return L"drarrow ";
		else if(wsSymbol == L"\u227A")
			return L"prec ";
		else if(wsSymbol == L"\u227B")
			return L"succ ";
		else if(wsSymbol == L"\u227C")
			return L"preccurlyeq ";
		else if(wsSymbol == L"\u227D")
			return L"succcurlyeq ";
		else if(wsSymbol == L"\u227E")
			return L"precsim ";
		else if(wsSymbol == L"\u227F")
			return L"succsim ";
		else if(wsSymbol == L"\u2280")
			return L"nprec ";
		else if(wsSymbol == L"\u2281")
			return L"nsucc ";
		else if(wsSymbol == L"\u2208")
			return L"in ";
		else if(wsSymbol == L"\u2209")
			return L"notin ";
		else if(wsSymbol == L"\u220B")
			return L"owns ";
		else if(wsSymbol == L"\u22C2")
			return L"intersection ";
		else if(wsSymbol == L"\u22C3")
			return L"union ";
		else if(wsSymbol == L"\u2216")
			return L"setminus ";
		else if(wsSymbol == L"\u2215")
			return L"setquotient ";
		else if(wsSymbol == L"\u2282")
			return L"subset ";
		else if(wsSymbol == L"\u2286")
			return L"subseteq ";
		else if(wsSymbol == L"\u2283")
			return L"supset ";
		else if(wsSymbol == L"\u2287")
			return L"supseteq ";
		else if(wsSymbol == L"\u2284")
			return L"nsubset ";
		else if(wsSymbol == L"\u2288")
			return L"nsubseteq ";
		else if(wsSymbol == L"\u2285")
			return L"nsupset ";
		else if(wsSymbol == L"\u2289")
			return L"nsupseteq ";
		else
			return L"";
	}
//class CSpace
	CSpace::~CSpace()
	{}
	void CSpace::Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd, COneElement *&pElement)
	{}
	void CSpace::Conversion(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation)
	{
		pXmlWrite->WriteNodeBegin(L"mspace",true);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		wsAnnotation += L"` ";
	}
//class CSpecialChar
	CSpecialChar::~CSpecialChar()
	{}
	void CSpecialChar::Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd, COneElement *&pElement)
	{}
	void CSpecialChar::Conversion(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation)
	{
		unsigned int iStyle(0);
		if(GetAttribute() != nullptr)
			COneElement::ConversionAttribute(GetAttribute(),iStyle,pXmlWrite,wsAnnotation);
		if(!m_wsAnnotation.empty())
		{
			pXmlWrite->WriteNodeBegin(L"mi",true);
			pXmlWrite->WriteAttribute(L"mathvariant",L"normal");
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			pXmlWrite->WriteString(m_wsSymbol);
			pXmlWrite->WriteNodeEnd(L"mi",false,false);
			wsAnnotation += m_wsAnnotation + L" ";
		}
		else if(!m_wsSymbol.empty())
		{
			pXmlWrite->WriteNodeBegin(L"mtext",false);
			pXmlWrite->WriteString(m_wsSymbol);
			pXmlWrite->WriteNodeEnd(L"mtext",false,false);
			wsAnnotation += L"\u0026quot;" + m_wsSymbol + L"\u0026quot; ";
		}
		COOXml2Odf::StyleClosing(iStyle,pXmlWrite);
	}
	std::wstring CSpecialChar::DefinitionSpecialChar(const std::wstring &wsSymbol)
	{
		if(L"\u2205" == wsSymbol) return L"emptyset";
		else if(L"\u2135" == wsSymbol) return L"aleph";
		else if(L"\u2115" == wsSymbol) return L"setN";
		else if(L"\u2124" == wsSymbol) return L"setZ";
		else if(L"\u211A" == wsSymbol) return L"setQ";
		else if(L"\u211D" == wsSymbol) return L"setR";
		else if(L"\u2102" == wsSymbol) return L"setC";
		else if(L"\u221E" == wsSymbol) return L"infinity";
		else if(L"\u2202" == wsSymbol) return L"partial";
		else if(L"\u2207" == wsSymbol) return L"nabla";
		else if(L"\u2203" == wsSymbol) return L"exists";
		else if(L"\u2204" == wsSymbol) return L"notexists";
		else if(L"\u2200" == wsSymbol) return L"forall";
		else if(L"\u012" == wsSymbol) return L"hbar";
		else if(L"\u019B" == wsSymbol) return L"lambdabar";
		else if(L"\u211C" == wsSymbol) return L"Re";
		else if(L"\u2111" == wsSymbol) return L"Im";
		else if(L"\u2118" == wsSymbol) return L"wp";
		else if(L"\u2112" == wsSymbol) return L"laplace";
		else if(L"\u2131" == wsSymbol) return L"fourier";
		else if(L"\u03F6" == wsSymbol) return L"backepsilon";
		else if(L"\u2190" == wsSymbol) return L"leftarrow";
		else if(L"\u2794" == wsSymbol) return L"rightarrow";
		else if(L"\u2191" == wsSymbol) return L"uparrow";
		else if(L"\u2193" == wsSymbol) return L"downarrow";
		else if(L"\u2026" == wsSymbol) return L"dotslow";
		else if(L"\u22EF" == wsSymbol) return L"dotsaxis";
		else if(L"\u22EE" == wsSymbol) return L"dotsvert";
		else if(L"\u22F0" == wsSymbol) return L"dotsup";
		else if(L"\u22F1" == wsSymbol) return L"dotsdown";
		else return L"";
	}
}
