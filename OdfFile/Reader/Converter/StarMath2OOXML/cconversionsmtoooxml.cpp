/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "cconversionsmtoooxml.h"
#include "../../../../DesktopEditor/common/File.h"
#include <fstream>
namespace StarMath {
	CConversionSMtoOOXML::CConversionSMtoOOXML(): m_pXmlWrite(nullptr)
	{
	}
	CConversionSMtoOOXML::~CConversionSMtoOOXML()
	{
		if (m_pXmlWrite)
			delete m_pXmlWrite;
	}
	//check XMLWrite(if not nullptr == delete)
	void CConversionSMtoOOXML::StartConversion(std::vector<StarMath::CElement*> arPars, const unsigned int& iAlignment)
	{
		m_pXmlWrite = new XmlUtils::CXmlWriter;
		if(!arPars.empty())
		{
			std::wstring wsNodeMath(L"m:oMath"),wsNodeMathPara(L"m:oMathPara"),wsAlignment;
			switch(iAlignment)
			{
			case 0:
			wsAlignment = L"left";
			break;
			case 1:
			wsAlignment = L"center";
			break;
			case 2:
			wsAlignment = L"right";
			break;
			default:
			wsAlignment = L"center";
			break;
			}
			if(arPars[0]->GetTypeConversion() == TypeConversion::pptx || arPars[0]->GetTypeConversion() == TypeConversion::xlsx)
			{
				wsNodeMath += L" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\"";
				wsNodeMathPara += L" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\"";
				wsAlignment += L"Group";
			}
			m_pXmlWrite->WriteNodeBegin(wsNodeMathPara,false);
			if(iAlignment>= 0 && iAlignment <= 2)
			{
				m_pXmlWrite->WriteNodeBegin(L"m:oMathParaPr",false);
				m_pXmlWrite->WriteNodeBegin(L"m:jc",true);
				m_pXmlWrite->WriteAttribute(L"m:val",wsAlignment);
				m_pXmlWrite->WriteNodeEnd(L"",true,true);
				m_pXmlWrite->WriteNodeEnd(L"m:oMathParaPr",false,false);
			}
			m_pXmlWrite->WriteNodeBegin(wsNodeMath,false);
			for(CElement* oTempElement:arPars)
			{
				if(oTempElement != nullptr)
				{
					if(CParserStarMathString::CheckNewline(oTempElement))
					{
						m_pXmlWrite->WriteNodeBegin(L"m:r",false);
						m_pXmlWrite->WriteNodeBegin(L"w:br",true);
						m_pXmlWrite->WriteNodeEnd(L"",true,true);
						m_pXmlWrite->WriteNodeEnd(L"m:r",false,false);
						m_pXmlWrite->WriteNodeEnd(L"m:oMath",false,false);
						m_pXmlWrite->WriteNodeBegin(L"m:oMath",false);
					}
					else
						oTempElement->ConversionToOOXML(m_pXmlWrite);
				}
			}
		}
		else
		{
			m_pXmlWrite->WriteNodeBegin(L"m:oMathPara",false);
			m_pXmlWrite->WriteNodeBegin(L"m:oMath",false);
		}
		m_pXmlWrite->WriteNodeEnd(L"m:oMath",false,false);
		m_pXmlWrite->WriteNodeEnd(L"m:oMathPara",false,false);
	}
	void CConversionSMtoOOXML::StandartProperties(XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute,const TypeConversion &enTypeConversion)
	{
		if(TypeConversion::docx == enTypeConversion || TypeConversion::undefine == enTypeConversion)
		{
			if(pAttribute == nullptr)
			{
				pXmlWrite->WriteNodeBegin(L"w:rPr",false);
				pXmlWrite->WriteNodeBegin(L"w:rFonts",true);
				pXmlWrite->WriteAttribute(L"w:hAnsi",L"Cambria Math");
				pXmlWrite->WriteAttribute(L"w:ascii",L"Cambria Math");
				pXmlWrite->WriteNodeEnd(L"w",true,true);
				pXmlWrite->WriteNodeBegin(L"w:sz",true);
				pXmlWrite->WriteAttribute(L"w:val",L"30");
				pXmlWrite->WriteNodeEnd(L"w",true,true);
				pXmlWrite->WriteNodeBegin(L"w:szCs",true);
				pXmlWrite->WriteAttribute(L"w:val",L"30");
				pXmlWrite->WriteNodeEnd(L"w",true,true);
				pXmlWrite->WriteNodeEnd(L"w:rPr",false,false);
			}
			else
			{
				std::wstring wsNameFont = pAttribute->GetFontName();
				pXmlWrite->WriteNodeBegin(L"w:rPr",false);
				pXmlWrite->WriteNodeBegin(L"w:rFonts",true);
				if(!wsNameFont.empty())
				{
					pXmlWrite->WriteAttribute(L"w:hAnsi",wsNameFont);
					pXmlWrite->WriteAttribute(L"w:ascii",wsNameFont);
				}
				else
				{
					pXmlWrite->WriteAttribute(L"w:hAnsi",L"Cambria Math");
					pXmlWrite->WriteAttribute(L"w:ascii",L"Cambria Math");
				}
				pXmlWrite->WriteNodeEnd(L"w",true,true);
				if(pAttribute->GetSize() == 0)
				{
					pXmlWrite->WriteNodeBegin(L"w:sz",true);
					pXmlWrite->WriteAttribute(L"w:val",L"30");
					pXmlWrite->WriteNodeEnd(L"w",true,true);
					pXmlWrite->WriteNodeBegin(L"w:szCs",true);
					pXmlWrite->WriteAttribute(L"w:val",L"30");
					pXmlWrite->WriteNodeEnd(L"w",true,true);
				}
				else if(pAttribute->GetSize() != 0)
				{
					pXmlWrite->WriteNodeBegin(L"w:sz",true);
					pXmlWrite->WriteAttribute(L"w:val",std::to_wstring(pAttribute->GetSize()));
					pXmlWrite->WriteNodeEnd(L"w",true,true);
					pXmlWrite->WriteNodeBegin(L"w:szCs",true);
					pXmlWrite->WriteAttribute(L"w:val",std::to_wstring(pAttribute->GetSize()));
					pXmlWrite->WriteNodeEnd(L"w",true,true);
				}
				if(!pAttribute->EmptyColor())
				{
					pXmlWrite->WriteNodeBegin(L"w:color",true);
					pXmlWrite->WriteAttribute(L"w:val",pAttribute->GetColor());
					pXmlWrite->WriteNodeEnd(L"w",true,true);
				}
				if(pAttribute->GetBold() && pAttribute->GetItal())
				{
					WriteStyNode(pXmlWrite,L"bi");
				}
				else if(pAttribute->GetBold())
				{
					pXmlWrite->WriteNodeBegin(L"m:sty", true);
					pXmlWrite->WriteAttribute(L"m:val",L"b");
					pXmlWrite->WriteNodeEnd(L"w",true,true);
					pXmlWrite->WriteNodeBegin(L"w:b",true);
					pXmlWrite->WriteNodeEnd(L"w",true,true);
					pXmlWrite->WriteNodeBegin(L"w:bCs",true);
					pXmlWrite->WriteNodeEnd(L"w",true,true);
				}
				else if(pAttribute->GetItal())
				{
					pXmlWrite->WriteNodeBegin(L"w:i",true);
					pXmlWrite->WriteNodeEnd(L"w",true,true);
				}
				if(pAttribute->GetStrike())
				{
					pXmlWrite->WriteNodeBegin(L"w:strike",true);
					pXmlWrite->WriteNodeEnd(L"w",true,true);
				}
				pXmlWrite->WriteNodeEnd(L"w:rPr",false,false);
			}
		}
		else if(TypeConversion::pptx == enTypeConversion || TypeConversion::xlsx == enTypeConversion)
		{
			if(pAttribute !=nullptr)
			{
				pXmlWrite->WriteNodeBegin(L"a:rPr",true);
				if(pAttribute->GetSize()!=0)
				{
					int iSize = pAttribute->GetSize();
					iSize = iSize*50;
					pXmlWrite->WriteAttribute(L"sz",iSize);
				}
				else
					pXmlWrite->WriteAttribute(L"sz",L"1500");
				if(pAttribute->GetBold())
					pXmlWrite->WriteAttribute(L"b",L"1");
				if(pAttribute->GetItal())
					pXmlWrite->WriteAttribute(L"i",L"1");
				if(pAttribute->GetStrike())
					pXmlWrite->WriteAttribute(L"strike",L"sngStrike");
				pXmlWrite->WriteAttribute(L"spc", -150);
				pXmlWrite->WriteNodeEnd(L"w",true,false);
				if(!pAttribute->GetColor().empty())
				{
				  pXmlWrite->WriteNodeBegin(L"a:solidFill",false);
				  pXmlWrite->WriteNodeBegin(L"a:srgbClr",true);
				  pXmlWrite->WriteAttribute(L"val",pAttribute->GetColor());
				  pXmlWrite->WriteNodeEnd(L"w",true,true);
				  pXmlWrite->WriteNodeEnd(L"a:solidFill",false,false);
				}
				pXmlWrite->WriteNodeBegin(L"a:latin",true);
				if(!pAttribute->GetFontName().empty())
					pXmlWrite->WriteAttribute(L"typeface",pAttribute->GetFontName());
				else
					pXmlWrite->WriteAttribute(L"typeface",L"Cambria Math");
				pXmlWrite->WriteNodeEnd(L"w",true,true);
				pXmlWrite->WriteNodeEnd(L"a:rPr",false);
			}
			else
			{
				pXmlWrite->WriteNodeBegin(L"a:rPr",true);
				pXmlWrite->WriteAttribute(L"sz",L"1500");
				pXmlWrite->WriteNodeEnd(L"w",true,false);
				pXmlWrite->WriteNodeBegin(L"a:latin",true);
				pXmlWrite->WriteAttribute(L"typeface",L"Cambria Math");
				pXmlWrite->WriteNodeEnd(L"w",true,true);
				pXmlWrite->WriteNodeEnd(L"a:rPr",false);
			}
		}
	}
	void CConversionSMtoOOXML::PropertiesMFPR(const std::wstring &wsType, XmlUtils::CXmlWriter* pXmlWrite, CAttribute* pAttribute, const TypeConversion &enTypeConversion)
	{
		pXmlWrite->WriteNodeBegin(L"m:fPr",false);
		if(!wsType.empty())
		{
			pXmlWrite->WriteNodeBegin(L"m:type",true);
			pXmlWrite->WriteAttribute(L"m:val",wsType);
			pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		WriteCtrlPrNode(pXmlWrite,pAttribute,enTypeConversion);
		pXmlWrite->WriteNodeEnd(L"m:fPr",false,false);
	}
	void CConversionSMtoOOXML::PropertiesNaryPr(const TypeElement& enTypeOp,bool bEmptySub,bool bEmptySup,XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute,const TypeConversion &enTypeConversion)
	{
		pXmlWrite->WriteNodeBegin(L"m:naryPr",false);
		switch(enTypeOp)
		{
		case TypeElement::sum:
		{
		WriteChrNode(L"\u2211",pXmlWrite);
		break;
		}
		case TypeElement::prod:
		WriteChrNode(L"\u220F",pXmlWrite);
		break;
		case TypeElement::coprod:
		WriteChrNode(L"\u2210",pXmlWrite);
		break;
		case TypeElement::iint:
		WriteChrNode(L"\u222C",pXmlWrite);
		break;
		case TypeElement::iiint:
		WriteChrNode(L"\u222D",pXmlWrite);
		break;
		case TypeElement::lint:
		WriteChrNode(L"\u222E",pXmlWrite);
		break;
		case TypeElement::llint:
		WriteChrNode(L"\u222F",pXmlWrite);
		break;
		case TypeElement::lllint:
		WriteChrNode(L"\u2230",pXmlWrite);
		break;
		default:
		break;
		}
		WriteLimLocNode(L"undOvr",pXmlWrite);
		if(bEmptySub)
		{
			pXmlWrite->WriteNodeBegin(L"m:subHide",true);
			pXmlWrite->WriteAttribute(L"m:val",L"1");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		if(bEmptySup)
		{
			pXmlWrite->WriteNodeBegin(L"m:supHide",true);
			pXmlWrite->WriteAttribute(L"m:val",L"1");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		WriteCtrlPrNode(pXmlWrite,pAttribute,enTypeConversion);
		pXmlWrite->WriteNodeEnd(L"m:naryPr",false,false);
	}
	void CConversionSMtoOOXML::WriteNodeConversion(const std::wstring &wsNameBlock, CElement *pValueBlock,XmlUtils::CXmlWriter* pXmlWrite)
	{
		if(pValueBlock != nullptr)
		{
			pXmlWrite->WriteNodeBegin(wsNameBlock,false);
			pValueBlock->ConversionToOOXML(pXmlWrite);
			pXmlWrite->WriteNodeEnd(wsNameBlock,false,false);
		}
		else
		{
			pXmlWrite->WriteNodeBegin(wsNameBlock,true);
			pXmlWrite->WriteNodeEnd(L"",true,true);
		}
	}
	std::wstring CConversionSMtoOOXML::GetOOXML()
	{
		return m_pXmlWrite->GetXmlString();
	}
	void CConversionSMtoOOXML::EndConversion()
	{
		m_pXmlWrite->WriteNodeEnd(L"m:oMath",false,false);
		m_pXmlWrite->WriteNodeEnd(L"m:oMathPara",false,false);
	}
	void CConversionSMtoOOXML::PropertiesFuncPr(XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute,const TypeConversion &enTypeConversion)
	{
		pXmlWrite->WriteNodeBegin(L"m:funcPr", false);
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr", false);
		StandartProperties(pXmlWrite,pAttribute,enTypeConversion);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr", false,false);
		pXmlWrite->WriteNodeEnd(L"m:funcPr",false,false);
	}

	void CConversionSMtoOOXML::PropertiesDPr(XmlUtils::CXmlWriter *pXmlWrite, const std::wstring &wsOpenBracket,const std::wstring &wsCloseBracket, CAttribute* pAttribute, const TypeConversion &enTypeConversion, const TypeElement &enTypeBracket)
	{
		pXmlWrite->WriteNodeBegin(L"m:dPr",false);
		BracketTypeNotation(wsOpenBracket,wsCloseBracket,pXmlWrite);
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr");
		StandartProperties(pXmlWrite,pAttribute,enTypeConversion);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:dPr",false,false);
	}
	void CConversionSMtoOOXML::BracketTypeNotation(const std::wstring &wsOpenBracket, const std::wstring &wsCloseBracket, XmlUtils::CXmlWriter *pXmlWrite)
	{
		if(!wsOpenBracket.empty())
		{
			pXmlWrite->WriteNodeBegin(L"m:begChr", true);
			if(wsOpenBracket == L"none")
				pXmlWrite->WriteAttribute(L"m:val",L"");
			else
				pXmlWrite->WriteAttribute(L"m:val",wsOpenBracket);
			pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		if(!wsCloseBracket.empty())
		{
			pXmlWrite->WriteNodeBegin(L"m:endChr", true);
			if(wsCloseBracket == L"none")
				pXmlWrite->WriteAttribute(L"m:val",L"");
			else
				pXmlWrite->WriteAttribute(L"m:val", wsCloseBracket);
			pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
	}
	void CConversionSMtoOOXML::PropertiesMPr(XmlUtils::CXmlWriter *pXmlWrite, const TypeElement &enTypeMatrix,CAttribute* pAttribute,const TypeConversion &enTypeConversion)
	{
		pXmlWrite->WriteNodeBegin(L"m:mPr",false);
		pXmlWrite->WriteNodeBegin(L"m:mcs",false);
		pXmlWrite->WriteNodeBegin(L"m:mc",false);
		pXmlWrite->WriteNodeBegin(L"m:mcPr",false);
		pXmlWrite->WriteNodeBegin(L"m:count",true);
		switch(enTypeMatrix)
		{
			case TypeElement::matrix:
				pXmlWrite->WriteAttribute(L"m:val",L"2");
				break;
			default:
				pXmlWrite->WriteAttribute(L"m:val",L"1");
				break;
		}
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"m:mcJc",true);
		pXmlWrite->WriteAttribute(L"m:val",L"center");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeEnd(L"m:mcPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:mc",false,false);
		pXmlWrite->WriteNodeEnd(L"m:mcs",false,false);
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
		StandartProperties(pXmlWrite,pAttribute,enTypeConversion);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:mPr",false,false);
	}
	void CConversionSMtoOOXML::NodeGrade(XmlUtils::CXmlWriter *pXmlWrite,CElement* pValueGrade,CAttribute* pAttribute)
	{
		pXmlWrite->WriteNodeBegin(L"m:d",false);
		pXmlWrite->WriteNodeBegin(L"m:dPr",false);
		pXmlWrite->WriteNodeBegin(L"m:begChr",true);
		pXmlWrite->WriteAttribute(L"m:val",L"");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"m:endChr",true);
		pXmlWrite->WriteAttribute(L"m:val",L"\u23AA");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
		StandartProperties(pXmlWrite,pAttribute,pValueGrade->GetTypeConversion());
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false);
		pXmlWrite->WriteNodeEnd(L"m:dPr",false,false);
		CConversionSMtoOOXML::WriteNodeConversion(L"m:e",pValueGrade,pXmlWrite);
		pXmlWrite->WriteNodeEnd(L"m:d",false,false);
	}
	void CConversionSMtoOOXML::WriteCtrlPrNode(XmlUtils::CXmlWriter *pXmlWrite, CAttribute *pAttribute,const TypeConversion &enTypeConversion)
	{
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
		StandartProperties(pXmlWrite,pAttribute,enTypeConversion);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
	}
	void CConversionSMtoOOXML::WriteChrNode(const std::wstring &wsTypeOp,XmlUtils::CXmlWriter* pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:chr",true);
		pXmlWrite->WriteAttribute(L"m:val",wsTypeOp);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
	}
	void CConversionSMtoOOXML::WriteLimLocNode(const std::wstring &wsTypeLimLock,XmlUtils::CXmlWriter* pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:limLoc",true);
		pXmlWrite->WriteAttribute(L"m:val",wsTypeLimLock);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
	}
	void CConversionSMtoOOXML::WriteRPrFName(const TypeElement &enTypeOp, XmlUtils::CXmlWriter *pXmlWrite,CAttribute* pAttribute,const std::wstring& wsNameOp,const TypeConversion &enTypeConversion)
	{
		pXmlWrite->WriteNodeBegin(L"m:r",false);
		pXmlWrite->WriteNodeBegin(L"m:rPr",false);
		pXmlWrite->WriteNodeBegin(L"m:sty",true);
		pXmlWrite->WriteAttribute(L"m:val",L"p");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeEnd(L"m:rPr",false,false);
		CConversionSMtoOOXML::StandartProperties(pXmlWrite,pAttribute,enTypeConversion);
		pXmlWrite->WriteNodeBegin(L"m:t",false);
		switch(enTypeOp)
		{
		case TypeElement::lim:
		pXmlWrite->WriteString(L"lim");
		break;
		case TypeElement::liminf:
		pXmlWrite->WriteString(L"lim inf");
		break;
		case TypeElement::limsup:
		pXmlWrite->WriteString(L"lim sup");
		break;
		case TypeElement::oper:
		pXmlWrite->WriteString(wsNameOp);
		break;
		default:
		break;
		}
		pXmlWrite->WriteNodeEnd(L"m:t",false,false);
		pXmlWrite->WriteNodeEnd(L"m:r",false,false);
	}
	void CConversionSMtoOOXML::WriteStyNode(XmlUtils::CXmlWriter *pXmlWrite, const std::wstring &wsAttributeNode)
	{
		pXmlWrite->WriteNodeBegin(L"m:sty", true);
		pXmlWrite->WriteAttribute(L"m:val",wsAttributeNode);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
	}
	void CConversionSMtoOOXML::WritePreserveBlock(XmlUtils::CXmlWriter *pXmlWrite,CAttribute* pAttribute,const TypeConversion &enTypeConversion)
	{
		pXmlWrite->WriteNodeBegin(L"m:r",false);
		StandartProperties(pXmlWrite,pAttribute,enTypeConversion);
		pXmlWrite->WriteNodeBegin(L"m:t",true);
		pXmlWrite->WriteAttribute(L"xml:space",L"preserve");
		pXmlWrite->WriteNodeEnd(L"w",true,false);
		pXmlWrite->WriteNodeEnd(L"m:t",false,false);
		pXmlWrite->WriteNodeEnd(L"m:r",false,false);
	}
	void CConversionSMtoOOXML::WriteLimUpOrLowNode(XmlUtils::CXmlWriter *pXmlWrite, const std::wstring& wsNameNode, CElement* pValue, const TypeElement& enType, CAttribute* pAttribute, const TypeConversion &enTypeConvers, const std::wstring& wsName, CElement* pIndex)
	{
		pXmlWrite->WriteNodeBegin(wsNameNode,false);
		pXmlWrite->WriteNodeBegin(wsNameNode+L"Pr",false);
		CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWrite,nullptr,enTypeConvers);
		pXmlWrite->WriteNodeEnd(wsNameNode+L"Pr",false,false);
		pXmlWrite->WriteNodeBegin(L"m:e",false);
		CConversionSMtoOOXML::WriteRPrFName(enType,pXmlWrite,pAttribute,wsName,enTypeConvers);
		pXmlWrite->WriteNodeEnd(L"m:e",false,false);
		if(pValue!= nullptr && pIndex != nullptr)
		{
			pXmlWrite->WriteNodeBegin(L"m:lim",false);
			pIndex->ConversionToOOXML(pXmlWrite);
			pValue->ConversionToOOXML(pXmlWrite);
			pXmlWrite->WriteNodeEnd(L"m:lim",false,false);
		}
		else if(pValue!=nullptr && pIndex == nullptr)
			WriteNodeConversion(L"m:lim",pValue,pXmlWrite);
		else if(pValue == nullptr && pIndex !=nullptr)
			WriteNodeConversion(L"m:lim",pIndex,pXmlWrite);
		pXmlWrite->WriteNodeEnd(wsNameNode,false,false);
	}
	void CConversionSMtoOOXML::ElementConversion(XmlUtils::CXmlWriter *pXmlWrite, CElement *pElement)
	{
		if(pElement!= nullptr)
			pElement->ConversionToOOXML(pXmlWrite);
	}
}

