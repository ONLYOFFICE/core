#include "cooxml2odf.h"

namespace StarMath
{
//class OOXml2Odf
	COOXml2Odf::COOXml2Odf():m_wsBaseColor(L""),m_uiBaseSize(0),m_bStretchyAcc(false),m_bHeight(false)
	{
		m_pXmlWrite = new XmlUtils::CXmlWriter;
	}
	COOXml2Odf::~COOXml2Odf()
	{
		delete m_pXmlWrite;
	}
	void COOXml2Odf::StartConversion(OOX::WritingElement *pNode)
	{
		m_pXmlWrite->WriteNodeBegin(L"math",true);
		m_pXmlWrite->WriteAttribute(L"xmlns",L"http:\/\/www.w3.org/1998/Math/MathML");
		m_pXmlWrite->WriteAttribute(L"display",L"block");
		m_pXmlWrite->WriteNodeEnd(L"w",true,false);
		m_pXmlWrite->WriteNodeBegin(L"semantics",false);
		if(pNode == nullptr)
		{
			m_pXmlWrite->WriteNodeBegin(L"annotation",false);
			EndOdf();
			return;
		}
		else
			NodeDefinition(pNode);
		m_wsSemantic = m_pXmlWrite->GetXmlString();
		m_pXmlWrite->WriteNodeBegin(L"annotation",true);
		m_pXmlWrite->WriteAttribute(L"encoding",L"StarMath 5.0");
		m_pXmlWrite->WriteNodeEnd(L"w",true,false);
		m_pXmlWrite->WriteString(m_wsAnnotationStarMath);
		EndOdf();
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
			ConversionMathPara(dynamic_cast<OOX::Logic::COMathPara*>(pNode));
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
		case OOX::EElementType::et_m_borderBox:
		{
			ConversionBorderBox(dynamic_cast<OOX::Logic::CBorderBox*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_m:
		{
			ConversionMatrix(dynamic_cast<OOX::Logic::CMatrix*>(pNode));
			break;
		}
		case OOX::EElementType::et_m_e:
		{
			ConversionElement(dynamic_cast<OOX::Logic::CElement*>(pNode));
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
	void COOXml2Odf::ConversionMathPara(OOX::Logic::COMathPara *pMathPara)
	{
		if(pMathPara == nullptr)
			return;
		bool bNewLine(false);
		if(pMathPara->m_arrItems.size() > 1)
		{
			bNewLine = true;
			m_pXmlWrite->WriteNodeBegin(L"mtable",false);
			m_pXmlWrite->WriteNodeBegin(L"mtr",false);
			m_pXmlWrite->WriteNodeBegin(L"mtd",false);
		}
		for(unsigned int i = 0; i < pMathPara->m_arrItems.size();i++)
		{
			switch (i)
			{
			case 0:
			break;
			case 1:
			{
				if(pMathPara->m_arrItems[0]->getType() != OOX::EElementType::et_m_oMathParaPr)
				{
					m_wsAnnotationStarMath += L"newline ";
					m_pXmlWrite->WriteNodeEnd(L"mtd",false,false);
					m_pXmlWrite->WriteNodeEnd(L"mtr",false,false);
					m_pXmlWrite->WriteNodeBegin(L"mtr",false);
					m_pXmlWrite->WriteNodeBegin(L"mtd",false);
				}
				break;
			}
			default:
			{
				m_wsAnnotationStarMath += L"newline ";
				m_pXmlWrite->WriteNodeEnd(L"mtd",false,false);
				m_pXmlWrite->WriteNodeEnd(L"mtr",false,false);
				m_pXmlWrite->WriteNodeBegin(L"mtr",false);
				m_pXmlWrite->WriteNodeBegin(L"mtd",false);
				break;
			}
			}
			NodeDefinition(pMathPara->m_arrItems[i]);
		}
		if(bNewLine)
		{
			m_pXmlWrite->WriteNodeEnd(L"mtd",false,false);
			m_pXmlWrite->WriteNodeEnd(L"mtr",false,false);
			m_pXmlWrite->WriteNodeEnd(L"mtable",false,false);
		}
	}
	std::vector<COneElement*> COOXml2Odf::ConversionMT(OOX::Logic::CMText *pMt, const StValuePr* pValue, const bool& bMRpr)
	{
		std::vector<COneElement*> arLine;
		if(pMt == nullptr)
			return arLine;
		std::wstring wsText = pMt->m_sText,wsElement,wsTextUTF16;
		wsTextUTF16 = COOXml2Odf::TransformationUTF32(pMt->m_sText);
		std::wstring::iterator itStart = wsTextUTF16.begin(),itEnd = wsTextUTF16.end();
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
		TFormulaSize stStart,stFrac;
		stStart = m_stSize;
		m_stSize.Zeroing();
		if(stPr.m_wsTypeName == L"noBar")
		{
			m_wsAnnotationStarMath += L"binom ";
			m_pXmlWrite->WriteNodeBegin(L"mtable",false);
			m_pXmlWrite->WriteNodeBegin(L"mtr",false);
			m_pXmlWrite->WriteNodeBegin(L"mtd",false);
			CheckVectorElementsForMf(pMf->m_oNum.GetPointer()->m_arrItems);
			COOXml2Odf::ComparisonSizeByWidth(stFrac,m_stSize);
			m_stSize.Zeroing();
			m_pXmlWrite->WriteNodeEnd(L"mtd",false,false);
			m_pXmlWrite->WriteNodeEnd(L"mtr",false,false);
			m_pXmlWrite->WriteNodeBegin(L"mtr",false);
			m_pXmlWrite->WriteNodeBegin(L"mtd",false);
			CheckVectorElementsForMf(pMf->m_oDen.GetPointer()->m_arrItems);
			COOXml2Odf::ComparisonSizeByWidth(stFrac,m_stSize);
			COOXml2Odf::ComparisonSizeByHeight(stStart,stFrac);
			m_stSize = stStart;
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
				m_pXmlWrite->WriteNodeEnd(L"w",true,false);
				m_pXmlWrite->WriteNodeBegin(L"mrow",false);
				CheckVectorElementsForMf(pMf->m_oNum.GetPointer()->m_arrItems);
				m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
				COOXml2Odf::ComparisonSizeByWidth(stFrac,m_stSize);
				m_stSize.Zeroing();
				m_wsAnnotationStarMath += L"wideslash ";
				m_pXmlWrite->WriteNodeBegin(L"mrow",false);
				CheckVectorElementsForMf(pMf->m_oDen.GetPointer()->m_arrItems);
				m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
				COOXml2Odf::ComparisonSizeByWidth(stFrac,m_stSize);
				COOXml2Odf::ComparisonSizeByHeight(stStart,stFrac);
				m_stSize = stStart;
			}
			else
			{
				m_pXmlWrite->WriteNodeEnd(L"w",true,false);
				m_pXmlWrite->WriteNodeBegin(L"mrow",false);
				CheckVectorElementsForMf(pMf->m_oNum.GetPointer()->m_arrItems);
				m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
				COOXml2Odf::ComparisonSizeByWidth(stFrac,m_stSize);
				m_stSize.Zeroing();
				m_wsAnnotationStarMath += L"over ";
				m_pXmlWrite->WriteNodeBegin(L"mrow",false);
				CheckVectorElementsForMf(pMf->m_oDen.GetPointer()->m_arrItems);
				m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
				COOXml2Odf::ComparisonSizeByWidth(stFrac,m_stSize);
				COOXml2Odf::ComparisonSizeByHeight(stStart,stFrac);
				m_stSize = stStart;
			}
			m_pXmlWrite->WriteNodeEnd(L"mfrac",false,false);
		}
		else
		{
			m_pXmlWrite->WriteNodeBegin(L"mrow",false);
			CheckVectorElementsForMf(pMf->m_oNum.GetPointer()->m_arrItems);
			RecordingMoNode(L"/",m_pXmlWrite);
			m_wsAnnotationStarMath += L"/ ";
			CheckVectorElementsForMf(pMf->m_oDen.GetPointer()->m_arrItems);
			COOXml2Odf::ComparisonSizeByHeight(m_stSize,stStart);
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
		StStyleMenClose stStyle;
		m_stSize.m_iWidth += 2;
		if(pDel->m_arrItems[0] != nullptr && pDel->m_arrItems[0]->getType() == OOX::EElementType::et_m_dPr)
			stDelPr = ConversionMdPr(dynamic_cast<OOX::Logic::CDelimiterPr*>(pDel->m_arrItems[0]),stStyle);
		m_pXmlWrite->WriteNodeBegin(L"mrow",false);
		if(!stDelPr.m_wsBegBracket.empty())
		{
			m_pXmlWrite->WriteNodeBegin(L"mo",true);
			m_pXmlWrite->WriteAttribute(L"fence",L"true");
			m_pXmlWrite->WriteAttribute(L"form",L"prefix");
			m_pXmlWrite->WriteAttribute(L"stretchy",L"true");
			m_pXmlWrite->WriteNodeEnd(L"w",true,false);
			m_pXmlWrite->WriteString(stDelPr.m_wsBegBracket);
			m_pXmlWrite->WriteNodeEnd(L"mo",false,false);
			m_stSize.m_iWidth -= 1;
		}
		m_wsAnnotationStarMath += L"left " + BracketForAnnotation(stDelPr.m_wsBegBracket,true) + L" ";
		m_pXmlWrite->WriteNodeBegin(L"mrow",false);
		for(int i = 1; i<pDel->m_arrItems.size();i++)
		{
			if(i >= 2)
			{
				RecordingMoNode(L"\u007C",m_pXmlWrite);
				m_wsAnnotationStarMath += L"mline ";
				m_stSize.m_iWidth += 1;
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
			m_pXmlWrite->WriteNodeEnd(L"w",true,false);
			m_pXmlWrite->WriteString(stDelPr.m_wsEndBracket);
			m_pXmlWrite->WriteNodeEnd(L"mo",false,false);
			m_stSize.m_iWidth -= 1;
		}
		m_wsAnnotationStarMath += L"right " + BracketForAnnotation(stDelPr.m_wsEndBracket,false) + L" ";
		m_pXmlWrite->WriteNodeEnd(L"mrow",false,false);
		StyleClosing(stStyle,m_pXmlWrite);
		if((stStyle.m_iStyle != 0 || stStyle.m_bMenClose) && !m_stAttribute.empty())
		{
			m_stAttribute.top()->Release();
			m_stAttribute.pop();
		}
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
		StStyleMenClose stStyle;
		TFormulaSize stStart = m_stSize,stFrac;
		stFrac.m_iWidth += 1;
		m_stSize.Zeroing();
		StValuePr stNaryPr = ConversionNaryPr(pNary->m_oNaryPr.GetPointer(),stStyle);
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
			COOXml2Odf::ComparisonSizeByWidth(stFrac,m_stSize);
			m_stSize.Zeroing();
			m_wsAnnotationStarMath += L"} ";
		}
		if(stNaryPr.m_bSupHide)
		{
			m_wsAnnotationStarMath += L"to {";
			ConversionSup(pNary->m_oSup.GetPointer());
			COOXml2Odf::ComparisonSizeByWidth(stFrac,m_stSize);
			m_stSize.Zeroing();
			m_wsAnnotationStarMath += L"} ";
		}
		m_pXmlWrite->WriteNodeEnd(wsTypeNary,false,false);
		ConversionElement(pNary->m_oElement.GetPointer());
		COOXml2Odf::ComparisonSizeByHeight(stStart,stFrac);
		COOXml2Odf::ComparisonSizeByHeight(m_stSize,stStart);
		m_stSize.m_iHeight += 1;
		StyleClosing(stStyle,m_pXmlWrite);
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
		TFormulaSize stStart = m_stSize,stFrac;
		m_stSize.Zeroing();
		if(pElement != nullptr)
		{
			ConversionElement(pElement);
			stFrac = m_stSize;
			m_stSize.Zeroing();
		}
		else
		{
			m_pXmlWrite->WriteNodeBegin(L"mspace",true);
			m_pXmlWrite->WriteNodeEnd(L"w",true,true);
			m_wsAnnotationStarMath += L"` ";
		}
		m_wsAnnotationStarMath += L"_ {";
		ConversionVectorWritingElement(pSub->m_arrItems);
		stFrac.m_iHeight += m_stSize.m_iHeight - 0.5;
		stFrac.m_iWidth += m_stSize.m_iWidth;
		ComparisonSizeByHeight(stStart,stFrac);
		m_stSize = stStart;
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
		TFormulaSize stStart = m_stSize,stFrac;
		m_stSize.Zeroing();
		m_pXmlWrite->WriteNodeBegin(L"msup",false);
		if(pElement != nullptr)
		{
			ConversionElement(pElement);
			stFrac = m_stSize;
			m_stSize.Zeroing();
		}
		else
		{
			m_pXmlWrite->WriteNodeBegin(L"mspace",true);
			m_pXmlWrite->WriteNodeEnd(L"w",true,true);
			m_wsAnnotationStarMath += L"` ";
		}
		m_wsAnnotationStarMath+= L"^ {";
		ConversionVectorWritingElement(pSup->m_arrItems);
		stFrac.m_iHeight += m_stSize.m_iHeight - 0.5;
		stFrac.m_iWidth += m_stSize.m_iWidth;
		ComparisonSizeByHeight(stStart,stFrac);
		m_stSize = stStart;
		m_wsAnnotationStarMath += L"} ";
		m_pXmlWrite->WriteNodeEnd(L"msup",false,false);
	}
	void COOXml2Odf::ConversionSup(OOX::Logic::CSup *pSup)
	{
		if(pSup == nullptr)
			return;
		ConversionVectorWritingElement(pSup->m_arrItems);
	}
	void COOXml2Odf::ConversionElement(OOX::Logic::CElement *pElement)
	{
		if(pElement == nullptr) return;
		ConversionVectorWritingElement(pElement->m_arrItems);
	}
	std::vector<COneElement*> COOXml2Odf::ConversionMRun(OOX::Logic::CMRun *pMRun)
	{
		std::vector<COneElement*> arLine;
		if(pMRun == nullptr) return arLine;

		StValuePr* stRpr(nullptr);		
		if(pMRun->m_oRPr.GetPointer() != nullptr)
			stRpr = ConversionRunProperties(pMRun->m_oRPr.GetPointer());
		else if (pMRun->m_oARPr.GetPointer() != nullptr)
			ConversionARpr(pMRun->m_oARPr.GetPointer(),stRpr);

		if(pMRun->m_oMRPr.GetPointer() != nullptr)
			ConversionMRunProperties(pMRun->m_oMRPr.GetPointer(),stRpr);

		for (size_t i = 0; i < pMRun->m_arrItems.size(); ++i)
		{
			switch (pMRun->m_arrItems[i]->getType())
			{
			case OOX::et_m_t:
			{
				std::vector<COneElement*> arLine_run;
				arLine_run = ConversionMT(dynamic_cast<OOX::Logic::CMText*>(pMRun->m_arrItems[i]), stRpr, pMRun->m_oMRPr.GetPointer());
				arLine.insert(arLine.end(), arLine_run.begin(), arLine_run.end());
				arLine_run.clear();
			}break;
			default:
				break;
			}
		}

		CreateAttribute(stRpr);
		if(stRpr != nullptr && !arLine.empty())
		{
			if(!m_stAttribute.empty())
				AttributeCheck(m_stAttribute.top(),stRpr);
			else
				AttributeCheck(stRpr);
			for(int i = 0; i<arLine.size();i++)
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
		return arLine;
	}
	StValuePr COOXml2Odf::ConversionMdPr(OOX::Logic::CDelimiterPr *pDelPr, StStyleMenClose &stStyle)
	{
		StValuePr pMdPr;
		if(pDelPr == nullptr)
			return pMdPr;
		stStyle = ConversionCtrlPr(pDelPr->m_oCtrlPr.GetPointer(),true);
		pMdPr.m_wsBegBracket = ConversionBegBracket(dynamic_cast<OOX::Logic::CBegChr*>(pDelPr->m_oBegChr.GetPointer()));
		pMdPr.m_wsEndBracket = ConversionEndBracket(dynamic_cast<OOX::Logic::CEndChr*>(pDelPr->m_oEndChr.GetPointer()));
		return pMdPr;
	}
	StValuePr COOXml2Odf::ConversionNaryPr(OOX::Logic::CNaryPr *pNaryPr, StStyleMenClose &stStyle)
	{
		StValuePr stNaryPr;
		if(pNaryPr == nullptr) return stNaryPr;
		stStyle = ConversionCtrlPr(pNaryPr->m_oCtrlPr.GetPointer());
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
	bool COOXml2Odf::ComparingAttributes(StValuePr *pRight, StValuePr *pLeft)
	{
		if(pRight == nullptr && pLeft == nullptr)
			return true;
		if(pRight != nullptr && pLeft != nullptr)
		{
			 if(pRight->m_wsColor != pLeft->m_wsColor)
				 return false;
			 else if(pRight->m_iSize != pLeft->m_iSize)
				 return false;
			 else if(pRight->m_enStyle != pLeft->m_enStyle)
				 return false;
			 else if(pRight->m_enFont != pLeft->m_enFont)
				 return false;
			 else return true;
		}
		else 
			return false;
	}
	void COOXml2Odf::AttributeCheck(StValuePr *&pParent, StValuePr *&pChild)
	{
		if(pParent == nullptr || pChild == nullptr)
			return;
		else
		{
			if(pParent->m_wsColor == pChild->m_wsColor)
				pChild->m_wsColor.clear();
			if(pParent->m_iSize == pChild->m_iSize)
				pChild->m_iSize = 0;
			if(pParent->m_enStyle == pChild->m_enStyle)
				pChild->m_enStyle = SimpleTypes::EStyle::stylePlain;
			if(pParent->m_enFont == pChild->m_enFont)
				pChild->m_enFont = StarMath::TypeFont::empty;
		}
	}
	void COOXml2Odf::AttributeCheck(StValuePr*& pChild)
	{
		if(pChild == nullptr && !pChild->m_bBaseAttribute)
			return;
		if(m_uiBaseSize != 0 && pChild->m_iSize == 0)
			pChild->m_iSize = m_uiBaseSize;
		if(!m_wsBaseColor.empty() && pChild->m_wsColor.empty())
			pChild->m_wsColor = m_wsBaseColor;
	}
	void COOXml2Odf::CreateAttribute(StValuePr*& pAttribute)
	{
		if(pAttribute != nullptr)
			return;
		if(m_uiBaseSize != 0 || !m_wsBaseColor.empty())
		{
			pAttribute = new StValuePr;
			pAttribute->m_bBaseAttribute = true;
			if(m_uiBaseSize != 0)
				pAttribute->m_iSize = m_uiBaseSize;
			if(!m_wsBaseColor.empty())
				pAttribute->m_wsColor = m_wsBaseColor;
		}
	}
	void COOXml2Odf::ConversionAcc(OOX::Logic::CAcc *pAcc)
	{
		std::wstring wsSymbol = pAcc->m_oAccPr->m_oChr.IsInit() ? pAcc->m_oAccPr->m_oChr.get().m_val->GetValue() : L"\u005E",wsSign;
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
			std::wstring wsStretchy;
			m_wsAnnotationStarMath += wsSign + L" ";
			if(m_bStretchyAcc)
			{
				m_wsAnnotationStarMath += L"{ ";
				wsStretchy = L"true";
			}
			else
				wsStretchy = L"false";
			m_pXmlWrite->WriteNodeBegin(L"mover",true);
			m_pXmlWrite->WriteAttribute(L"accent",L"true");
			m_pXmlWrite->WriteNodeEnd(L"w",true,false);
			ConversionElement(pAcc->m_oElement.GetPointer());
			if(wsStretchy == L"true")
				m_wsAnnotationStarMath += L"} ";
			m_pXmlWrite->WriteNodeBegin(L"mo",true);
			m_pXmlWrite->WriteAttribute(L"stretchy",wsStretchy);
			m_pXmlWrite->WriteNodeEnd(L"w",true,false);
			m_pXmlWrite->WriteString(wsSymbol);
			m_pXmlWrite->WriteNodeEnd(L"mo",false,false);
		}
		m_pXmlWrite->WriteNodeEnd(L"mover",false,false);
		m_stSize.m_iHeight += 0.5;
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
	StStyleMenClose COOXml2Odf::ConversionFuncPr(OOX::Logic::CFuncPr *pFuncPr)
	{
		StStyleMenClose stStyle;
		if(pFuncPr == nullptr)
			return stStyle;
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
	void COOXml2Odf::ConversionBorderBox(OOX::Logic::CBorderBox *pBorderBox)
	{
		if(pBorderBox == nullptr)
			return;
		NodeDefinition(pBorderBox->m_oElement.GetPointer());
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
			std::wstring wsColor,wsUpperColor(L"");
			wsColor =  pRPr->m_oColor.GetPointer()->m_oVal.GetPointer()!= nullptr ? pRPr->m_oColor.GetPointer()->m_oVal.GetPointer()->ToStringNoAlpha():L"";
			if(wsColor != L"")
			{
				for(wchar_t chToken: wsColor)
					wsUpperColor += std::toupper(chToken);
				if(wsUpperColor != L"000000")
				{
					stTempPr->m_wsColor = wsUpperColor;
					bRpr = true;
				}
			}
		}
		if(pRPr->m_oSz.GetPointer() != nullptr)
		{
			stTempPr->m_iSize = pRPr->m_oSz.GetPointer()->m_oVal.GetPointer() != nullptr ? pRPr->m_oSz.GetPointer()->m_oVal.GetPointer()->GetValue():0;
			if(stTempPr->m_iSize != 0)
				bRpr = true;
		}
		if(pRPr->m_oRFonts.GetPointer()!= nullptr && pRPr->m_oRFonts.GetPointer()->m_sAscii.GetPointer()!= nullptr)
			stTempPr->m_enFont = FontCheck(pRPr->m_oRFonts.GetPointer()->m_sAscii.get(),bRpr);
		if(pRPr->m_oStrike.GetPointer()!=nullptr && pRPr->m_oStrike.GetPointer()->m_oVal.GetValue() == SimpleTypes::EOnOff::onoffTrue)
		{
			stTempPr->m_bStrike = true;
			bRpr = true;
		}
		if(pRPr->m_oBold.GetPointer() != nullptr && pRPr->m_oBold.GetPointer()->m_oVal.GetValue() == SimpleTypes::EOnOff::onoffTrue)
		{
			if(pRPr->m_oItalic.GetPointer() != nullptr && pRPr->m_oItalic.GetPointer()->m_oVal.GetValue() == SimpleTypes::EOnOff::onoffTrue)
				stTempPr->m_enStyle = SimpleTypes::EStyle::styleBoldItalic;
			else
				stTempPr->m_enStyle = SimpleTypes::EStyle::styleBold;
			bRpr = true;
		}
		else if(pRPr->m_oItalic.GetPointer() != nullptr && pRPr->m_oItalic.GetPointer()->m_oVal.GetValue() == SimpleTypes::EOnOff::onoffTrue)
		{
			stTempPr->m_enStyle = SimpleTypes::EStyle::styleItalic;
			bRpr = true;
		}
		else if(pRPr->m_oU.GetPointer() != nullptr && pRPr->m_oU->m_oVal.GetPointer() != nullptr)
		{
			stTempPr->m_enUnderLine = pRPr->m_oU->m_oVal->GetValue();
			bRpr = true;
		}
		if(bRpr == true)
			return stTempPr;
		else
		{
			stTempPr->Release();
			return nullptr;
		}
	}
	void COOXml2Odf::ConversionMRunProperties(OOX::Logic::CMRPr *pMRpr, StValuePr*&pValue)
	{
		if(pMRpr == nullptr)
			return ;
		if(pMRpr->m_oSty.GetPointer() != nullptr && pMRpr->m_oSty.GetPointer()->m_val.GetPointer()->GetValue() != SimpleTypes::EStyle::stylePlain)
		{
			if(pValue == nullptr)
				pValue = new StValuePr;
			pValue->m_enStyle = pMRpr->m_oSty.GetPointer()->m_val.GetPointer()->GetValue();
		}
	}
	void COOXml2Odf::ConversionARpr(PPTX::Logic::RunProperties *pARpr, StValuePr *&pValue)
	{
		if(pARpr == nullptr)
			return;
		if(pValue == nullptr)
			pValue = new StValuePr;
		bool bAttribute{false};
		if(pARpr->b.IsInit() && pARpr->b.get())
		{
			if(pARpr->i.IsInit() && pARpr->i.get())
				pValue->m_enStyle = SimpleTypes::EStyle::styleBoldItalic;
			else
				pValue->m_enStyle = SimpleTypes::EStyle::styleBold;
			bAttribute = true;
		}
		else if(pARpr->i.IsInit() && pARpr->i.get())
		{
			pValue->m_enStyle = SimpleTypes::EStyle::styleItalic;
			bAttribute = true;
		}
		if(pARpr->sz.IsInit())
		{
			pValue->m_iSize = pARpr->sz.get()/100;
			bAttribute = true;
		}
		if(pARpr->latin.IsInit() && !pARpr->latin->typeface.empty())
			pValue->m_enFont = FontCheck(pARpr->latin->typeface,bAttribute);
//		if(pARpr->Fill.is_init() && pARpr->Fill.Fill.m_pData->)
	}
	StarMath::TypeFont COOXml2Odf::FontCheck(const std::wstring &wsFont, bool &bAttribute)
	{
		if(!wsFont.empty())
		{
			if(L"Liberation Serif" == wsFont)
			{
				bAttribute = true;
				return StarMath::TypeFont::serif;
			}
			else if(L"Liberation Sans" == wsFont)
			{
				bAttribute = true;
				return StarMath::TypeFont::sans;
			}
			else if(L"Liberation Mono" == wsFont)
			{
				bAttribute = true;
				return StarMath::TypeFont::fixed;
			}
			else
				return StarMath::TypeFont::empty;
		}
		else 
			return StarMath::TypeFont::empty;
	}
	bool COOXml2Odf::ColorCheck(const std::wstring &wsColor, std::wstring &wsRecordColor)
	{
		if(!wsColor.empty())
		{
			if(L"0000FF" == wsColor)
				wsRecordColor = L"blue";
			else if(L"00FF00" == wsColor)
				wsRecordColor = L"lime";
			else if(L"008000" == wsColor)
				wsRecordColor = L"green";
			else if(L"FF0000" == wsColor)
				wsRecordColor = L"red";
			else if(L"ED0DD9" == wsColor)
				wsRecordColor = L"fuchsia";
			else if(L"30D5C8" == wsColor)
				wsRecordColor = L"aqua";
			else if(L"FFFF00" == wsColor)
				wsRecordColor = L"yellow";
			else if(L"808080" == wsColor)
				wsRecordColor = L"gray";
			else if(L"800000" == wsColor)
				wsRecordColor = L"maroon";
			else if(L"000080" == wsColor)
				wsRecordColor = L"navy";
			else if(L"808000" == wsColor)
				wsRecordColor = L"olive";
			else if(L"800080" == wsColor)
				wsRecordColor = L"purple";
			else if(L"C0C0C0" == wsColor)
				wsRecordColor = L"silver";
			else if(L"008080" == wsColor)
				wsRecordColor = L"teal";
			else if(L"FF7F50" == wsColor)
				wsRecordColor = L"coral";
			else if(L"191970" == wsColor)
				wsRecordColor = L"midnightblue";
			else if(L"DC143C" == wsColor)
				wsRecordColor = L"crimson";
			else if(L"EE82EE" == wsColor)
				wsRecordColor = L"violet";
			else if(L"FFA500" == wsColor)
				wsRecordColor = L"orange";
			else if(L"FF4500" == wsColor)
				wsRecordColor = L"orangered";
			else if(L"2E8B57" == wsColor)
				wsRecordColor = L"seagreen";
			else if(L"4B0082" == wsColor)
				wsRecordColor = L"indigo";
			else if(L"FF69B4" == wsColor)
				wsRecordColor = L"hotpink";
			else if(L"FFF0F5" == wsColor)
				wsRecordColor = L"lavender";
			else
				return false;
			return true;
		}
		else
			return false;
	}
	void COOXml2Odf::CheckVectorElementsForMf(std::vector<OOX::WritingElement*> arWrElement)
	{
		if(!arWrElement.empty())
		{
			m_wsAnnotationStarMath += L"{ ";
			ConversionVectorWritingElement(arWrElement);
			m_wsAnnotationStarMath += L"} ";
		}
		else
			COOXml2Odf::EmptyBlock(m_pXmlWrite,m_wsAnnotationStarMath,m_stSize);
	}
	TFormulaSize COOXml2Odf::GetFormulaSize()
	{
		return m_stSize;
	}
	void COOXml2Odf::ConversionTextVector(std::vector<COneElement *> &arLine, std::vector<COneElement *> &arNewLine)
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
					else if(pElement->GetType() ==TypeElement::String && pTempElement->GetType() == TypeElement::String && ComparingAttributes(pTempElement->GetAttribute(),pElement->GetAttribute()))
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
				if(arNewLine[i] != nullptr)
					arNewLine[i]->Conversion(m_pXmlWrite,m_wsAnnotationStarMath,m_stSize);
			}
		}
		arLine.clear();
		arNewLine.clear();
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
					ConversionTextVector(arLine,arNewLine);
				NodeDefinition(arWrElements[i]);
			}
		}
		if(!arLine.empty())
		{
			ConversionTextVector(arLine,arNewLine);
		}
	}
	std::wstring COOXml2Odf::TranslationDiacritSign(const std::wstring &wsSymbol)
	{
		if( L"\u0308" == wsSymbol) return L"ddot";
		else if(L"\u005E" == wsSymbol)
		{
			m_bStretchyAcc = true;
			return L"widehat";
		}
		else if(L"\u0307" == wsSymbol) return L"dot";
		else if(L"\u0301" == wsSymbol) return L"acute";
		else if(L"\u0300" == wsSymbol) return L"grave";
		else if(L"\u0306" == wsSymbol) return L"breve";
		else if(L"\u030A" == wsSymbol) return L"circle";
		else if(L"\u0304" == wsSymbol) return L"bar";
		else if(L"\u20DB" == wsSymbol) return L"dddot";
		else if(L"\u20D1" == wsSymbol) return L"harpoon";
		else if(L"\u20D7" == wsSymbol) return L"vec";
		else if(L"\u007E" == wsSymbol)
		{
			m_bStretchyAcc = true;
			return L"widetilde";
		}
		else if(L"\u0302" == wsSymbol) return L"hat";
		else if(L"\u030C" == wsSymbol) return L"check";
		else if(L"\u0305" == wsSymbol)
		{
			m_bStretchyAcc = true;
			return L"overline";
		}
		else if(L"\u0332" == wsSymbol)
		{
			m_bStretchyAcc = true;
			return L"underline";
		}
		else return L"";
	}
	void COOXml2Odf::ConversionMatrix(OOX::Logic::CMatrix* pMatrix)
	{
		if(pMatrix == nullptr)
			return;
		StStyleMenClose stStyle = ConversionCMPr(dynamic_cast<OOX::Logic::CMPr*>(pMatrix->m_arrItems[0]));
		TFormulaSize stStart = m_stSize,stFrac;
		m_stSize.Zeroing();
		m_wsAnnotationStarMath += L"matrix{";
		m_pXmlWrite->WriteNodeBegin(L"mtable",false);
		for(int i = 1;i<pMatrix->m_arrItems.size();i++)
		{
			if(OOX::EElementType::et_m_mr == pMatrix->m_arrItems[i]->getType())
			{
				ConversionMr(dynamic_cast<OOX::Logic::CMr*>(pMatrix->m_arrItems[i]));
				COOXml2Odf::ComparisonSizeByWidth(stFrac,m_stSize);
				m_stSize.Zeroing();
			}
				if(i+1 < pMatrix->m_arrItems.size())
				{
					m_wsAnnotationStarMath += L"## ";
					m_stSize.m_iHeight += 1;
				}
		}
		m_pXmlWrite->WriteNodeEnd(L"mtable",false,false);
		COOXml2Odf::ComparisonSizeByHeight(stStart,stFrac);
		m_stSize = stStart;
		StyleClosing(stStyle,m_pXmlWrite);
		if((stStyle.m_iStyle != 0 || stStyle.m_bMenClose) && !m_stAttribute.empty())
		{
			m_stAttribute.top()->Release();
			m_stAttribute.pop();
		}
		m_wsAnnotationStarMath += L"} ";
	}
	void COOXml2Odf::ConversionMr(OOX::Logic::CMr *pMr)
	{
		if(pMr == nullptr)
			return;
		m_pXmlWrite->WriteNodeBegin(L"mtr",false);
		TFormulaSize stStart = m_stSize,stFrac;
		m_stSize.Zeroing();
		for(unsigned int i = 0;i<pMr->m_arrItems.size();i++)
		{
			if(pMr->m_arrItems[i]->getType() == OOX::EElementType::et_m_e)
			{
				OOX::Logic::CElement* pElement = dynamic_cast<OOX::Logic::CElement*>(pMr->m_arrItems[i]);
				m_pXmlWrite->WriteNodeBegin(L"mtd",false);
				if(!pElement->m_arrItems.empty())
				{
					ConversionElement(pElement);
					ComparisonSizeByHeight(stFrac,m_stSize);
					m_stSize.Zeroing();
				}
				else
				{
					m_pXmlWrite->WriteNodeBegin(L"mspace",true);
					m_pXmlWrite->WriteAttribute(L"width",L"2em");
					m_pXmlWrite->WriteNodeEnd(L"w",true,true);
					m_wsAnnotationStarMath += L"~ ";
					stFrac.m_iWidth += 1;
				}
				if(i+1 < pMr->m_arrItems.size())
				{
					m_wsAnnotationStarMath += L"# ";
					stFrac.m_iWidth += 1;
				}
				m_pXmlWrite->WriteNodeEnd(L"mtd",false,false);
			}
		}
		ComparisonSizeByHeight(stStart,stFrac);
		m_stSize = stStart;
		m_pXmlWrite->WriteNodeEnd(L"mtr",false,false);
	}
	StStyleMenClose COOXml2Odf::ConversionCtrlPr(OOX::Logic::CCtrlPr *pCtrlPr, const bool &bDelimiter)
	{
		StStyleMenClose stStyle;
		if(pCtrlPr == nullptr)
			return stStyle;
		StValuePr* pValue(nullptr);
		if(pCtrlPr->m_oRPr.GetPointer() != nullptr)
			pValue = ConversionRunProperties(pCtrlPr->m_oRPr.GetPointer());
		else if(pCtrlPr->m_oARPr.GetPointer() != nullptr)
			ConversionARpr(pCtrlPr->m_oARPr.GetPointer(),pValue);
		CreateAttribute(pValue);
		if(pValue != nullptr)
		{
			if(!m_stAttribute.empty())
				AttributeCheck(m_stAttribute.top(),pValue);
			else
				AttributeCheck(pValue);
			COneElement::ConversionAttribute(pValue,stStyle,m_pXmlWrite,m_wsAnnotationStarMath,bDelimiter);
			if(bDelimiter)
				m_stAttribute.push(pValue);
			return stStyle;
		}
		return stStyle;
	}
	StStyleMenClose COOXml2Odf::ConversionCMPr(OOX::Logic::CMPr *pMPr)
	{
		StStyleMenClose stStyle;
		if(pMPr == nullptr)
			return stStyle;
		return ConversionCtrlPr(dynamic_cast<OOX::Logic::CCtrlPr*>(pMPr->m_oCtrlPr.GetPointer()),true);
	}
	std::wstring COOXml2Odf::GetOdf()
	{
		return m_pXmlWrite->GetXmlString();
	}
	std::wstring COOXml2Odf::GetAnnotation()
	{
		return m_wsAnnotationStarMath;
	}
	std::wstring COOXml2Odf::GetSemantic()
	{
		return m_wsSemantic;
	}
	void COOXml2Odf::SetBaseAttribute(std::wstring wsBaseColor, unsigned int uiBaseSize)
	{
		if(!wsBaseColor.empty())
			m_wsBaseColor = wsBaseColor;
		if(uiBaseSize != 0)
			m_uiBaseSize = uiBaseSize;
	}
	void COOXml2Odf::ConversionRad(OOX::Logic::CRad *pRad)
	{
		if(pRad == nullptr)
			return;
		bool bDeg{false};
		StStyleMenClose stStyle = ConversionRadPr(pRad->m_oRadPr.GetPointer(),bDeg);
		if(bDeg)
		{
			m_wsAnnotationStarMath += L"sqrt ";
			m_pXmlWrite->WriteNodeBegin(L"msqrt",false);
			m_wsAnnotationStarMath += L"{ ";
			NodeDefinition(pRad->m_oElement.GetPointer());
			m_wsAnnotationStarMath += L"} ";
			m_pXmlWrite->WriteNodeEnd(L"msqrt",false,false);
			m_stSize.m_iHeight += 1;
		}
		else if(!bDeg)
		{
			TFormulaSize stStart = m_stSize,stFrac;
			m_stSize.Zeroing();
			m_wsAnnotationStarMath += L"nroot ";
			std::wstring wsStart = m_wsAnnotationStarMath,wsElement;
			m_wsAnnotationStarMath.clear();
			m_pXmlWrite->WriteNodeBegin(L"mroot",false);
			m_wsAnnotationStarMath += L"{ ";
			NodeDefinition(pRad->m_oElement.GetPointer());
			stFrac = m_stSize;
			m_stSize.Zeroing();
			m_wsAnnotationStarMath += L"} ";
			wsElement = m_wsAnnotationStarMath;
			m_wsAnnotationStarMath.clear();
			m_wsAnnotationStarMath += L"{ ";
			ConversionDeg(pRad->m_oDeg.GetPointer());
			stFrac.m_iHeight += m_stSize.m_iHeight - 0.5;
			stFrac.m_iWidth += m_stSize.m_iWidth;
			ComparisonSizeByHeight(stStart,stFrac);
			m_stSize = stStart;
			m_wsAnnotationStarMath += L"} ";
			wsStart += m_wsAnnotationStarMath + wsElement;
			m_wsAnnotationStarMath = wsStart;
			m_pXmlWrite->WriteNodeEnd(L"mroot",false,false);
		}
		m_stSize.m_iWidth += 1;
		StyleClosing(stStyle,m_pXmlWrite);
	}
	StStyleMenClose COOXml2Odf::ConversionRadPr(OOX::Logic::CRadPr *pRadPr,bool&bDeg)
	{
		StStyleMenClose stStyle;
		if(pRadPr == nullptr)
			return stStyle;
		bDeg = pRadPr->m_oDegHide.GetPointer() != nullptr ? true:false;
		return ConversionCtrlPr(dynamic_cast<OOX::Logic::CCtrlPr*>(pRadPr->m_oCtrlPr.GetPointer()));
	}
	void COOXml2Odf::ConversionDeg(OOX::Logic::CDeg *pDeg)
	{
		if(pDeg == nullptr)
			return;
		ConversionVectorWritingElement(pDeg->m_arrItems);
	}
	void COOXml2Odf::ConversionGroupChr(OOX::Logic::CGroupChr *pGroup, const bool &bElement)
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
				wsNode = L"mover";
				break;
			}
			case SimpleTypes::ETopBot::tbTop:
			{
				wsNode = L"munder";
				break;
			}
			}
		}
		if(!wsAnnotation.empty())
		{
			if(bElement)
			{
				WritingbBracketsFromTopBottom(wsNode,stGroupPr.m_wsChr,wsAnnotation,pGroup->m_oElement.GetPointer());
			}
			else
			{
				m_pXmlWrite->WriteNodeBegin(wsNode,false);
				WritingbBracketsFromTopBottom(wsNode,stGroupPr.m_wsChr,wsAnnotation,pGroup->m_oElement.GetPointer());
				m_pXmlWrite->WriteNodeBegin(L"mspace",true);
				m_pXmlWrite->WriteNodeEnd(L"w",true,true);
				m_wsAnnotationStarMath += L"` ";
				m_pXmlWrite->WriteNodeEnd(wsNode,false,false);
			}
		}
		else
		{
			StStyleMenClose stStyle;
			m_pXmlWrite->WriteNodeBegin(wsNode,false);
			stStyle = ConversionCtrlPr(pGroup->m_oGroupChrPr.GetPointer()->m_oCtrlPr.GetPointer());
			m_pXmlWrite->WriteNodeBegin(L"mtext",false);
			m_pXmlWrite->WriteString(stGroupPr.m_wsChr);
			m_pXmlWrite->WriteNodeEnd(L"mtext",false,false);
			StyleClosing(stStyle,m_pXmlWrite);
			m_wsAnnotationStarMath += L"\u0026quot;" + stGroupPr.m_wsChr + L"\u0026quot; ";
			m_wsAnnotationStarMath += wsNode == L"mover"? L"csup ":L"csub ";
			ConversionElement(pGroup->m_oElement.GetPointer());
			m_pXmlWrite->WriteNodeEnd(wsNode,false,false);
		}
	}
	void COOXml2Odf::WritingbBracketsFromTopBottom(const std::wstring &wsNode, const std::wstring &wsChr, const std::wstring &wsAnnotation, OOX::Logic::CElement *pElement)
	{
		m_pXmlWrite->WriteNodeBegin(wsNode,false);
		ConversionElement(pElement);
		if(!wsChr.empty())
			RecordingMoNode(wsChr,m_pXmlWrite);
		else
			RecordingMoNode(L"\u23DF",m_pXmlWrite);
		m_wsAnnotationStarMath += wsAnnotation;
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
		StStyleMenClose stStyle;
		std::wstring wsNode;
		StValuePr pBarPr = ConversionBarPr(pBar->m_oBarPr.GetPointer(),stStyle);
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
		StyleClosing(stStyle,m_pXmlWrite);
		m_stSize.m_iHeight += 1;
	}
	StValuePr COOXml2Odf::ConversionBarPr(OOX::Logic::CBarPr *pBarPr, StStyleMenClose &stStyle)
	{
		StValuePr stBarPr;
		if(pBarPr == nullptr)
			return stBarPr;
		if(pBarPr->m_oPos.GetPointer() != nullptr && pBarPr->m_oPos->m_val.GetPointer() != nullptr)
			stBarPr.m_enPos = pBarPr->m_oPos->m_val->GetValue();
		stStyle = ConversionCtrlPr(pBarPr->m_oCtrlPr.GetPointer());
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
		TFormulaSize stStart = m_stSize,stFrac;
		m_stSize.Zeroing();
		if(IsEmptyNode(pSPre->m_oElement.GetPointer()))
			EmptyBlock(m_pXmlWrite,m_wsAnnotationStarMath,m_stSize);
		else
			ConversionElement(pSPre->m_oElement.GetPointer());
		stFrac = m_stSize;
		m_stSize.Zeroing();
		m_pXmlWrite->WriteNodeBegin(L"mprescripts",true);
		m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		if(IsEmptyNode(pSPre->m_oSup.GetPointer()))
		{
			m_pXmlWrite->WriteNodeBegin(L"none",true);
			m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		else
		{
			m_wsAnnotationStarMath += L"lsup { ";
			ConversionSup(pSPre->m_oSup.GetPointer());
			m_wsAnnotationStarMath += L"} ";
		}
		if(IsEmptyNode(pSPre->m_oSub.GetPointer()))
		{
			m_pXmlWrite->WriteNodeBegin(L"none",true);
			m_pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		else
		{
			m_wsAnnotationStarMath += L"lsub { ";
			ConversionSub(pSPre->m_oSub.GetPointer());
			m_wsAnnotationStarMath += L"} ";
		}
		stFrac.m_iHeight += m_stSize.m_iHeight;
		stFrac.m_iWidth += m_stSize.m_iWidth;
		ComparisonSizeByHeight(stStart,stFrac);
		m_stSize = stStart;
		m_pXmlWrite->WriteNodeEnd(L"mmultiscripts",false,false);
	}
	void COOXml2Odf::EmptyBlock(XmlUtils::CXmlWriter* pXmlWrite, std::wstring& wsAnnotation, TFormulaSize &stSize)
	{
		pXmlWrite->WriteNodeBegin(L"mi",true);
		pXmlWrite->WriteAttribute(L"mathvariant",L"normal");
		pXmlWrite->WriteNodeEnd(L"w",true,false);
		pXmlWrite->WriteString(L"\u2751");
		pXmlWrite->WriteNodeEnd(L"mi",false,false);
		wsAnnotation += L"{\u0026lt;\u003F\u0026gt;} ";
		stSize.m_iWidth += 1;
	}
	void COOXml2Odf::ConversionLimLow(OOX::Logic::CLimLow *pLimLow)
	{
		if(pLimLow == nullptr)
			return;
		TFormulaSize stStart = m_stSize,stFrac;
		m_stSize.Zeroing();
		m_pXmlWrite->WriteNodeBegin(L"munder",false);
		if(pLimLow->m_oElement.GetPointer()->m_arrItems.size() == 1 && pLimLow->m_oElement.GetPointer()->m_arrItems[0]->getType() == OOX::EElementType::et_m_groupChr)
			ConversionGroupChr(dynamic_cast<OOX::Logic::CGroupChr*>(pLimLow->m_oElement.GetPointer()->m_arrItems[0]),true);
		else
		{
			ConversionElement(pLimLow->m_oElement.GetPointer());
			m_wsAnnotationStarMath += L"csub ";
		}
		stFrac = m_stSize;
		m_stSize.Zeroing();
		ConversionLim(pLimLow->m_oLim.GetPointer());
		ComparisonSizeByWidth(stFrac,m_stSize);
		ComparisonSizeByHeight(stStart,stFrac);
		m_stSize = stStart;
		m_pXmlWrite->WriteNodeEnd(L"munder",false,false);
	}
	void COOXml2Odf::ConversionLimUpp(OOX::Logic::CLimUpp *pLimUpp)
	{
		if(pLimUpp == nullptr)
			return;
		TFormulaSize stStart = m_stSize,stFrac;
		m_stSize.Zeroing();
		m_pXmlWrite->WriteNodeBegin(L"mover",false);
		if(pLimUpp->m_oElement.GetPointer()->m_arrItems.size() == 1 && pLimUpp->m_oElement.GetPointer()->m_arrItems[0]->getType() == OOX::EElementType::et_m_groupChr)
			ConversionGroupChr(dynamic_cast<OOX::Logic::CGroupChr*>(pLimUpp->m_oElement.GetPointer()->m_arrItems[0]),true);
		else
		{
			ConversionElement(pLimUpp->m_oElement.GetPointer());
			m_wsAnnotationStarMath += L"csup ";
		}
		stFrac = m_stSize;
		m_stSize.Zeroing();
		ConversionLim(pLimUpp->m_oLim.GetPointer());
		ComparisonSizeByWidth(stFrac,m_stSize);
		ComparisonSizeByHeight(stStart,stFrac);
		m_stSize = stStart;
		m_pXmlWrite->WriteNodeEnd(L"mover",false,false);
	}
	void COOXml2Odf::ConversionLim(OOX::Logic::CLim *pLim)
	{
		if(pLim == nullptr)
			return;
		m_wsAnnotationStarMath += L"{ ";
		ConversionVectorWritingElement(pLim->m_arrItems);
		m_wsAnnotationStarMath += L"} ";
	}
	void COOXml2Odf::ConversionEqArr(OOX::Logic::CEqArr *pEqArr)
	{
		if(pEqArr == nullptr)
			return;
		m_pXmlWrite->WriteNodeBegin(L"mtable",false);
		TFormulaSize stStart = m_stSize,stFrac;
		m_stSize.Zeroing();
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
			ComparisonSizeByWidth(stFrac,m_stSize);
			m_stSize.Zeroing();
		}
		ComparisonSizeByHeight(stStart,stFrac);
		m_stSize = stStart;
		m_wsAnnotationStarMath += L"} ";
		m_pXmlWrite->WriteNodeEnd(L"mtable",false,false);
	}
	void COOXml2Odf::ConversionSubSup(OOX::Logic::CSSubSup *pSubSup)
	{
		if(pSubSup == nullptr)
			return;
		TFormulaSize stStart = m_stSize,stFrac,stTemp;
		m_stSize.Zeroing();
		m_pXmlWrite->WriteNodeBegin(L"msubsup",false);
		ConversionElement(pSubSup->m_oElement.GetPointer());
		stTemp = m_stSize;
		m_stSize.Zeroing();
		m_wsAnnotationStarMath += L"_ { ";
		ConversionSub(pSubSup->m_oSub.GetPointer());
		stFrac = m_stSize;
		m_stSize.Zeroing();
		m_wsAnnotationStarMath += L"} ^ { ";
		ConversionSup(pSubSup->m_oSup.GetPointer());
		ComparisonSizeByWidth(stFrac,m_stSize);
		ComparisonSizeByHeight(stFrac,stTemp);
		ComparisonSizeByHeight(stStart,stFrac);
		m_stSize = stStart;
		m_wsAnnotationStarMath += L"} ";
		m_pXmlWrite->WriteNodeEnd(L"msubsup",false,false);
	}
	void COOXml2Odf::StyleClosing(const StStyleMenClose &stStyle, XmlUtils::CXmlWriter *pXmlWrite)
	{
		if(stStyle.m_bUnderlineClose)
		{
			pXmlWrite->WriteNodeBegin(L"mo",false);
			pXmlWrite->WriteString(L"\u0332");
			pXmlWrite->WriteNodeEnd(L"mo",false,false);
			pXmlWrite->WriteNodeEnd(L"munder",false,false);
		}
		if(stStyle.m_bMenClose)
			pXmlWrite->WriteNodeEnd(L"menclose",false,false);
		if(stStyle.m_iStyle != 0)
		{
			unsigned int k(0);
			while(k < stStyle.m_iStyle)
			{
				pXmlWrite->WriteNodeEnd(L"mstyle",false,false);
				k++;
			}
		}
	}
	void COOXml2Odf::MTextRecording(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation, const std::wstring &wsText,TFormulaSize& stSize)
	{
		pXmlWrite->WriteNodeBegin(L"mtext",false);
		pXmlWrite->WriteString(wsText);
		pXmlWrite->WriteNodeEnd(L"mtext",false,false);
		wsAnnotation += L"\u0026quot;" + wsText + L"\u0026quot; ";
		stSize.m_iWidth += wsText.size();
	}
	void COOXml2Odf::ComparisonSizeByHeight(TFormulaSize &stLeft, const TFormulaSize &stRight)
	{
		if(stRight.m_iHeight > stLeft.m_iHeight)
			stLeft.m_iHeight = stRight.m_iHeight;
		if(stRight.m_iWidth != 0)
			stLeft.m_iWidth += stRight.m_iWidth;
	}
	void COOXml2Odf::ComparisonSizeByWidth(TFormulaSize &stLeft, const TFormulaSize &stRight)
	{
		if(stRight.m_iHeight != 0)
			stLeft.m_iHeight += stRight.m_iHeight;
		if(stRight.m_iWidth > stLeft.m_iWidth)
			stLeft.m_iWidth = stRight.m_iWidth;
	}
	std::wstring COOXml2Odf::TransformationUTF32(const std::wstring &wsText)
	{
		NSStringUtils::CStringUTF32 oString32(wsText);
		std::wstring wsText16;
		for(unsigned int i = 0;i < oString32.length();i++)
		{
			//Mathematical Bold Capital
			if(oString32[i] >= 119808 && oString32[i] <= 119833)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119743));
			//Italic Small Dotless I
			else if(oString32[i] == 120484)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120379));
			//Italic Small Dotless J
			else if(oString32[i] == 120485)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120379));
			//Bold Nabla
			else if(oString32[i] == 120513)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 111802));
			//Italic Nabla
			else if(oString32[i] == 120571)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 111860));
			//Bold Italic Nabla
			else if(oString32[i] == 120629)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 111918));
			//Sans-Serif Bold Nabla
			else if(oString32[i] == 120687)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 111976));
			//Sans-Serif Bold Italic Nabla
			else if(oString32[i] == 120745)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 112034));
			//Mathematical Bold Small
			else if(oString32[i] >= 119834 && oString32[i] <= 119859)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119737));
			//Italic Capital
			else if(oString32[i] >= 119860 && oString32[i] <= 119885)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119795));
			//Italic Small
			else if(oString32[i] >= 119886 && oString32[i] <= 119911)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119789));
			//Bold Italic Capital
			else if(oString32[i] >= 119912 && oString32[i] <= 119937)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119847));
			//Bold Italic Small
			else if(oString32[i] >= 119938 && oString32[i] <= 119963)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119841));
			//Script Capital
			else if(oString32[i] >= 119964 && oString32[i] <= 119989)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119899));
			//Script Small
			else if(oString32[i] >= 119990 && oString32[i] <= 120015)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119893));
			//Bold Script Capital
			else if(oString32[i] >= 120016 && oString32[i] <= 120041)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119951));
			//Bold Script Small
			else if(oString32[i] >= 120042 && oString32[i] <= 120067)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119945));
			//Fraktur Capital(120093?)
			else if(oString32[i] >= 120068 && oString32[i] <= 120092)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 112003));
			//Fraktur Small
			else if(oString32[i] >= 120094 && oString32[i] <= 120119)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119997));
			//Double-Struck Capital(120145?)
			else if(oString32[i] >= 120120 && oString32[i] <= 120144)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120055));
			//Double-Struck Small
			else if(oString32[i] >= 120146 && oString32[i] <= 120171)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120049));
			//Bold Fraktur Capital
			else if(oString32[i] >= 120172 && oString32[i] <= 120197)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120107));
			//Bold Fraktur Small
			else if(oString32[i] >= 120198 && oString32[i] <= 120223)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120101));
			//Sans-Serif Capital
			else if(oString32[i] >= 120224 && oString32[i] <= 120249)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120159));
			//Sans-Serif Small
			else if(oString32[i] >= 120250 && oString32[i] <= 120275)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120153));
			//Sans-Serif Bold Capital
			else if(oString32[i] >= 120276 && oString32[i] <= 120301)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120211));
			//Sans-Serif Bold Small
			else if(oString32[i] >= 120302 && oString32[i] <= 120327)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120205));
			//Sans-Serif Italic Capital
			else if(oString32[i] >= 120328 && oString32[i] <= 120353)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120263));
			//Sans-Serif Italic Small
			else if(oString32[i] >= 120354 && oString32[i] <= 120379)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120257));
			//Sans-Serif Bold Italic Capital
			else if(oString32[i] >= 120380 && oString32[i] <= 120405)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120315));
			//Sans-Serif Bold Italic Small
			else if(oString32[i] >= 120406 && oString32[i] <= 120431)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120309));
			//Monospace Capital
			else if(oString32[i] >= 120432 && oString32[i] <= 120457)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120367));
			//Monospace Small
			else if(oString32[i] >= 120458 && oString32[i] <= 120483)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120361));
			//Bold Capital Alpha 
			else if(oString32[i] >= 120488 && oString32[i] <= 120512)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119575));
			//Bold Small Alpha
			else if(oString32[i] >= 120514 && oString32[i] <= 120538)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119569));
			//Italic Capital Alpha
			else if(oString32[i] >= 120546 && oString32[i] <= 120570)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119633));
			//Italic Small Alpha
			else if(oString32[i] >= 120572 && oString32[i] <= 120596)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119627));
			//Bold Italic Capital Alpha
			else if(oString32[i] >= 120604 && oString32[i] <= 120628)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119691));
			//Bold Italic Small Alpha
			else if(oString32[i] >= 120630 && oString32[i] <= 120654)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119685));
			//Sans-Serif Bold Capital Alpha
			else if(oString32[i] >= 120662 && oString32[i] <= 120686)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119749));
			//Sans-Serif Bold Small Alpha
			else if(oString32[i] >= 120688 && oString32[i] <= 120712)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119743));
			//Sans-Serif Bold Italic Capital Alpha
			else if(oString32[i] >= 120720 && oString32[i] <= 120744)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119807));
			//Sans-Serif Bold Italic Small Alpha
			else if(oString32[i] >= 120746 && oString32[i] <= 120770)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 119801));
			//Bold Digit Zero
			else if(oString32[i] >= 120782 && oString32[i] <= 120791)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120734));
			//Double-Struck Digit Zero
			else if(oString32[i] >= 120792 && oString32[i] <= 120801)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120744));
			//Sans-Serif Digit Zero 
			else if(oString32[i] >= 120802 && oString32[i] <= 120811)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120754));
			//Monospace Digit Zero
			else if(oString32[i] >= 120822 && oString32[i] <= 120831)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120774));
			//Sans-Serif Bold Digit Zero 
			else if(oString32[i] >= 120812 && oString32[i] <= 120821)
				wsText16.push_back((wchar_t)((unsigned int) oString32[i] - 120764));
			else
				wsText16.push_back(oString32[i]);
		}
		return wsText16;
	}
//class COneElement
	COneElement::COneElement():m_stAttribute(nullptr),m_iStyle(0)
	{}
	COneElement::COneElement(std::wstring &wsOneElement):m_stAttribute(nullptr),m_iStyle(0)
	{
	}
	COneElement::~COneElement()
	{}
	COneElement* COneElement::CreateElement(std::wstring &wsOneElement)
	{
		if(wsOneElement == L"")
			return nullptr;
		std::wstring wsAnnotation = CSpecialChar::DefinitionSpecialChar(wsOneElement);
		if(COOXml2Odf::IsDigit(wsOneElement))
			return new CNumberOrLetter(wsOneElement,StarMath::TypeElement::number);
		else if(!wsAnnotation.empty())
			return new CSpecialChar(wsOneElement,wsAnnotation);
		else if(wsOneElement == L" ")
			return new CSpace();
		wsAnnotation.clear();
		wsAnnotation =  CBinOperator::BinOperatorSymbolForAnnotation(wsOneElement);
		if(wsAnnotation != L"")
			return new CBinOperator(wsOneElement,wsAnnotation);
		else
			wsAnnotation.clear();
		wsAnnotation = CRelationsAndOperationsOnSets::TransformationForAnnotation(wsOneElement);
		if(wsAnnotation != L"")
			return new CRelationsAndOperationsOnSets(wsOneElement,wsAnnotation);
		if(COOXml2Odf::IsAlpha(wsOneElement))
			return new CNumberOrLetter(wsOneElement,StarMath::TypeElement::letter);
		else
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
	void COneElement::SetBaseAttribute(StValuePr *stAttribute)
	{
		if(m_stAttribute == nullptr)
		{
			m_stAttribute = stAttribute;
			stAttribute->AddRef();
		}
	}
	StValuePr* COneElement::GetAttribute()
	{
		return m_stAttribute;
	}
	void COneElement::ConversionAttribute(StValuePr* pAttribute, StStyleMenClose &stStyle, XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation, const bool& bDelimiter)
	{
		if(pAttribute == nullptr)
			return;
		if(!pAttribute->m_wsColor.empty())
		{
			pXmlWrite->WriteNodeBegin(L"mstyle",true);
			std::wstring wsColor;
			bool bColorName = COOXml2Odf::ColorCheck(pAttribute->m_wsColor,wsColor);
			if(!bColorName)
				wsColor = L"#" + pAttribute->m_wsColor;
			pXmlWrite->WriteAttribute(L"mathcolor",wsColor);
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			if(bColorName)
				wsAnnotation += L"color " + wsColor + L" ";
			else
				wsAnnotation += L"color hex " + pAttribute->m_wsColor + L" ";
			stStyle.m_iStyle++;
		}
		if(pAttribute->m_iSize != 0)
		{
			std::wstring wsSize = std::to_wstring(pAttribute->m_iSize);
			pXmlWrite->WriteNodeBegin(L"mstyle",true);
			pXmlWrite->WriteAttribute(L"mathsize",wsSize+L"pt");
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			wsAnnotation += L"size " + wsSize + L" ";
			stStyle.m_iStyle++;
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
			stStyle.m_iStyle++;
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
			stStyle.m_iStyle++;
		}
		if(pAttribute->m_bStrike)
		{
			pXmlWrite->WriteNodeBegin(L"menclose",true);
			pXmlWrite->WriteAttribute(L"notation",L"horizontalstrike");
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			wsAnnotation += L"overstrike ";
			stStyle.m_bMenClose = true;
		}
		if(pAttribute->m_enUnderLine == SimpleTypes::EUnderline::underlineSingle)
		{
			pXmlWrite->WriteNodeBegin(L"munder",true);
			pXmlWrite->WriteAttribute(L"accentunder",L"true");
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			wsAnnotation += L"underline ";
			stStyle.m_bUnderlineClose = true;
		}
		if(!bDelimiter)
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
	void CNumberOrLetter::Conversion(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation, TFormulaSize &stSize)
	{
		StStyleMenClose stStyle;
		stSize.m_iWidth += m_wsElement.size();
		if(stSize.m_iHeight == 0)
			stSize.m_iHeight += 1;
		bool bCloseBracket(false);
		COneElement::ConversionAttribute(GetAttribute(),stStyle,pXmlWrite,wsAnnotation);
		if(GetAttribute()!= nullptr && m_wsElement.size()>1)
		{
			switch(m_enTypeElement)
			{
				case StarMath::TypeElement::number:
				{
					if(!COOXml2Odf::IsDigit(m_wsElement))
					{
						wsAnnotation += L"{ ";
						bCloseBracket = true;
					}
					break;
				}
				case StarMath::TypeElement::letter:
				{
					if(!COOXml2Odf::IsAlpha(m_wsElement))
					{
						wsAnnotation += L"{ ";
						bCloseBracket = true;
					}
					break;
				}
				default:
				break;
			}
		}
		switch(m_enTypeElement)
		{
		case StarMath::TypeElement::number:
		{
			pXmlWrite->WriteNodeBegin(L"mn",false);
            pXmlWrite->WriteString(XmlUtils::EncodeXmlString(m_wsElement));
			pXmlWrite->WriteNodeEnd(L"mn",false,false);
			wsAnnotation += m_wsElement + L" ";
			break;
		}
		case StarMath::TypeElement::letter:
		case StarMath::TypeElement::letter_u32:
		{
			COOXml2Odf::MTextRecording(pXmlWrite,wsAnnotation,m_wsElement,stSize);
			break;
		}
		default:
		break;
		}
		if(bCloseBracket)
			wsAnnotation += L"} ";
		COOXml2Odf::StyleClosing(stStyle,pXmlWrite);
	}
	void CNumberOrLetter::Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd, COneElement *&pElement)
	{}
	void CNumberOrLetter::SetAttribute(StValuePr *pAttribute)
	{
		SetBaseAttribute(pAttribute);
	}
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
	void CBinOperator::Conversion(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation, TFormulaSize &stSize)
	{
		StStyleMenClose stStyle;
		stSize.m_iWidth += 1;
		if(m_pLeftArg == nullptr && m_pRightArg == nullptr)
		{
			if(GetAttribute() != nullptr && m_enTypeBinOp != TypeElement::undefine)
			{
				COneElement::ConversionAttribute(GetAttribute(),stStyle,pXmlWrite,wsAnnotation);
				pXmlWrite->WriteNodeBegin(L"mtext",false);
				pXmlWrite->WriteString(m_wsSymbolBinOp);
				pXmlWrite->WriteNodeEnd(L"mtext",false,false);
				wsAnnotation += L"\u0026quot;" + m_wsSymbolBinOp + L"\u0026quot; ";
				COOXml2Odf::StyleClosing(stStyle,pXmlWrite);
				return;
			}
		}
		pXmlWrite->WriteNodeBegin(L"mrow",false);
		if(m_pLeftArg != nullptr)
			m_pLeftArg->Conversion(pXmlWrite,wsAnnotation,stSize);
		else if(GetAttribute() != nullptr && m_enTypeBinOp != TypeElement::undefine)
			COneElement::ConversionAttribute(GetAttribute(),stStyle,pXmlWrite,wsAnnotation);
		COOXml2Odf::RecordingMoNode(m_wsSymbolBinOp,pXmlWrite);
		wsAnnotation += m_wsAnnotation + L" ";
		if(m_pRightArg != nullptr)
			m_pRightArg->Conversion(pXmlWrite,wsAnnotation,stSize);
		COOXml2Odf::StyleClosing(stStyle,pXmlWrite);
		pXmlWrite->WriteNodeEnd(L"mrow",false,false);
	}
	void CBinOperator::SetAttribute(StValuePr *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		if(m_pLeftArg != nullptr)
			m_pLeftArg->SetAttribute(pAttribute);
		if(m_pRightArg != nullptr)
			m_pRightArg->SetAttribute(pAttribute);
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
	void CRelationsAndOperationsOnSets::Conversion(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation, TFormulaSize &stSize)
	{
		stSize.m_iWidth += 1;
		if(m_pLeftArg == nullptr && m_pRightArg == nullptr)
			COOXml2Odf::MTextRecording(pXmlWrite,wsAnnotation,m_wsSymbol,stSize);
		else
		{
			pXmlWrite->WriteNodeBegin(L"mrow",false);
			if(m_pLeftArg!= nullptr)
			{
				if(GetAttribute() != nullptr)
				{
					GetAttribute()->AddRef();
					m_pLeftArg->SetBaseAttribute(GetAttribute());
				}
				m_pLeftArg->Conversion(pXmlWrite,wsAnnotation,stSize);
			}
			if(m_wsAnnotationSymbol == L"\u0026lt;" || m_wsAnnotationSymbol == L"\u0026gt;")
				COOXml2Odf::RecordingMoNode(m_wsAnnotationSymbol,pXmlWrite);
			else
				COOXml2Odf::RecordingMoNode(m_wsSymbol,pXmlWrite);
			wsAnnotation += m_wsAnnotationSymbol + L" ";
			if(m_pRightArg != nullptr)
			{
				if(GetAttribute() != nullptr)
				{
					GetAttribute()->AddRef();
					m_pRightArg->SetBaseAttribute(GetAttribute());
				}
				m_pRightArg->Conversion(pXmlWrite,wsAnnotation,stSize);
			}
			pXmlWrite->WriteNodeEnd(L"mrow",false,false);
		}
	}
	void CRelationsAndOperationsOnSets::SetAttribute(StValuePr *pAttribute)
	{
		SetBaseAttribute(pAttribute);
		if(m_pLeftArg != nullptr)
			m_pLeftArg->SetAttribute(pAttribute);
		if(m_pRightArg != nullptr)
			m_pRightArg->SetAttribute(pAttribute);
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
	void CSpace::Conversion(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation, TFormulaSize &stSize)
	{
		stSize.m_iWidth += 1;
		pXmlWrite->WriteNodeBegin(L"mspace",true);
		pXmlWrite->WriteAttribute(L"width",L"0.5em");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		wsAnnotation += L"` ";
	}
	void CSpace::SetAttribute(StValuePr *pAttribute)
	{}
//class CSpecialChar
	CSpecialChar::~CSpecialChar()
	{}
	void CSpecialChar::Parse(std::wstring::iterator &itStart, std::wstring::iterator &itEnd, COneElement *&pElement)
	{}
	void CSpecialChar::Conversion(XmlUtils::CXmlWriter *pXmlWrite, std::wstring &wsAnnotation, TFormulaSize &stSize)
	{
		StStyleMenClose stStyle;
		if(GetAttribute() != nullptr)
			COneElement::ConversionAttribute(GetAttribute(),stStyle,pXmlWrite,wsAnnotation);
		if(!m_wsAnnotation.empty() && m_wsAnnotation == L"space")
		{
			pXmlWrite->WriteNodeBegin(L"mspace",true);
			pXmlWrite->WriteAttribute(L"width",L"2em");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
			wsAnnotation += L"~ ";
			stSize.m_iWidth += 1;
		}
		else if(!m_wsAnnotation.empty())
		{
			pXmlWrite->WriteNodeBegin(L"mi",true);
			pXmlWrite->WriteAttribute(L"mathvariant",L"normal");
			pXmlWrite->WriteNodeEnd(L"w",true,false);
			pXmlWrite->WriteString(m_wsSymbol);
			pXmlWrite->WriteNodeEnd(L"mi",false,false);
			wsAnnotation += m_wsAnnotation + L" ";
			stSize.m_iWidth += m_wsAnnotation.size();
		}
		else if(!m_wsSymbol.empty())
			COOXml2Odf::MTextRecording(pXmlWrite,wsAnnotation,m_wsSymbol,stSize);
		COOXml2Odf::StyleClosing(stStyle,pXmlWrite);
	}
	void CSpecialChar::SetAttribute(StValuePr *pAttribute)
	{
		SetBaseAttribute(pAttribute);
	}
	std::wstring CSpecialChar::DefinitionSpecialChar(const std::wstring &wsSymbol)
	{
		if(L"\u2205" == wsSymbol) return L"emptyset";
		else if(L"\u0026" == wsSymbol) return L"space";
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
		else if(L"\u0127" == wsSymbol) return L"hbar";
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
		else if(L"\u0391" == wsSymbol) return L"%ALPHA";
		else if(L"\u03B1" == wsSymbol) return L"%alpha";
		else if(L"\u0396" == wsSymbol) return L"%ZETA";
		else if(L"\u03B6" == wsSymbol) return L"%zeta";
		else if(L"\u039B" == wsSymbol) return L"%LAMBDA";
		else if(L"\u03BB" == wsSymbol) return L"%lambda";
		else if(L"\u03A0" == wsSymbol) return L"%PI";
		else if(L"\u03C0" == wsSymbol) return L"%pi";
		else if(L"\u03A6" == wsSymbol) return L"%PHI";
		else if(L"\u03C6" == wsSymbol) return L"%phi";
		else if(L"\u03B5" == wsSymbol) return L"%varepsilon";
		else if(L"\u03B9" == wsSymbol) return L"%iota";
		else if(L"\u0399" == wsSymbol) return L"%IOTA";
		else if(L"\u03BE" == wsSymbol) return L"%xi";
		else if(L"\u039E" == wsSymbol) return L"%XI";
		else if(L"\u03F1" == wsSymbol) return L"%varrho";
		else if(L"\u0392" == wsSymbol) return L"%BETA";
		else if(L"\u03B2" == wsSymbol) return L"%beta";
		else if(L"\u0397" == wsSymbol) return L"%ETA";
		else if(L"\u03B7" == wsSymbol) return L"%eta";
		else if(L"\u039C" == wsSymbol) return L"%MU";
		else if(L"\u03BC" == wsSymbol) return L"%mu";
		else if(L"\u03A1" == wsSymbol) return L"%RHO";
		else if(L"\u03C1" == wsSymbol) return L"%rho";
		else if(L"\u03A7" == wsSymbol) return L"%CHI";
		else if( L"\u03C7" == wsSymbol) return L"%chi";
		else if(L"\u03BA" == wsSymbol) return L"%kappa";
		else if(L"\u039A" == wsSymbol) return L"%KAPPA";
		else if(L"\u039F" == wsSymbol) return L"%OMICRON";
		else if(L"\u03BF" == wsSymbol) return L"%omicron";
		else if(L"\u03A3" == wsSymbol) return L"%SIGMA";
		else if(L"\u03C3" == wsSymbol) return L"%sigma";
		else if(L"\u03C6" == wsSymbol) return L"%varphi";
		else if(L"\u0393" == wsSymbol) return L"%GAMMA";
		else if(L"\u03B3" == wsSymbol) return L"%gamma";
		else if(L"\u0398" == wsSymbol) return L"%THETA";
		else if(L"\u03B8" == wsSymbol) return L"%theta";
		else if(L"\u039D" == wsSymbol) return L"%NU";
		else if(L"\u03BD" == wsSymbol) return L"%nu";
		else if(L"\u03A8" == wsSymbol) return L"%PSI";
		else if(L"\u03C8" == wsSymbol) return L"%psi";
		else if(L"\u03DB" == wsSymbol) return L"%varsigma";
		else if(L"\u0394" == wsSymbol) return L"%DELTA";
		else if(L"\u03B4" == wsSymbol) return L"%delta";
		else if(L"\u03A4" == wsSymbol) return L"%TAU";
		else if(L"\u03C4" == wsSymbol) return L"%tau";
		else if(L"\u03A9" == wsSymbol) return L"%OMEGA";
		else if(L"\u03C9" == wsSymbol) return L"%omega";
		else if(L"\u03D6" == wsSymbol) return L"%varphi";
		else if(L"\u0395" == wsSymbol) return L"%EPSILON";
		else if(L"\u03B5" == wsSymbol) return L"%epsilon";
		else if(L"\u03A5" == wsSymbol) return L"%UPSILON";
		else if(L"\u03C5" == wsSymbol) return L"%upsilon";
		else if(L"\u03D1" == wsSymbol) return L"%vartheta";
		else return L"";
	}
}
