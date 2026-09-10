/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "cconversionsmtoooxml.h"
#include "cstarmathpars.h"
#include "../../../../DesktopEditor/common/File.h"

namespace StarMath {

	CConversionSMtoOOXML::CConversionSMtoOOXML(): m_pXmlWriter(nullptr)
	{
	}
	CConversionSMtoOOXML::~CConversionSMtoOOXML()
	{
		if (m_pXmlWriter)
			delete m_pXmlWriter;
	}
	//check XMLWrite(if not nullptr == delete)
	void CConversionSMtoOOXML::StartConversion(const std::vector<StarMath::CElement*> arPars, const unsigned int& iAlignment)
	{
		m_pXmlWriter = new XmlUtils::CXmlWriter;
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
			m_pXmlWriter->WriteNodeBegin(wsNodeMathPara,false);
			if(iAlignment>= 0 && iAlignment <= 2)
			{
				m_pXmlWriter->WriteNodeBegin(L"m:oMathParaPr",false);
				m_pXmlWriter->WriteNodeBegin(L"m:jc",true);
				m_pXmlWriter->WriteAttribute(L"m:val",wsAlignment);
				m_pXmlWriter->WriteNodeEnd(L"",true,true);
				m_pXmlWriter->WriteNodeEnd(L"m:oMathParaPr",false,false);
			}
			m_pXmlWriter->WriteNodeBegin(wsNodeMath,false);
			for(CElement* oTempElement:arPars)
			{
				if(oTempElement != nullptr)
				{
					if(CParserStarMathString::CheckNewline(oTempElement))
					{
						m_pXmlWriter->WriteNodeBegin(L"m:r",false);
						m_pXmlWriter->WriteNodeBegin(L"w:br",true);
						m_pXmlWriter->WriteNodeEnd(L"",true,true);
						m_pXmlWriter->WriteNodeEnd(L"m:r",false,false);
						m_pXmlWriter->WriteNodeEnd(L"m:oMath",false,false);
						m_pXmlWriter->WriteNodeBegin(L"m:oMath",false);
					}
					else
						oTempElement->ConversionToOOXML(m_pXmlWriter);
				}
			}
		}
		else
		{
			m_pXmlWriter->WriteNodeBegin(L"m:oMathPara",false);
			m_pXmlWriter->WriteNodeBegin(L"m:oMath",false);
		}
		m_pXmlWriter->WriteNodeEnd(L"m:oMath",false,false);
		m_pXmlWriter->WriteNodeEnd(L"m:oMathPara",false,false);
	}
	void CConversionSMtoOOXML::StandartProperties(XmlUtils::CXmlWriter* pXmlWriter, CAttribute* pAttribute, const TypeConversion &enTypeConversion, const TextDirection &enTypeLang)
	{
		if(TypeConversion::docx == enTypeConversion || TypeConversion::undefine == enTypeConversion)
		{
			if(pAttribute == nullptr)
			{
				pXmlWriter->WriteNodeBegin(L"w:rPr",false);
				pXmlWriter->WriteNodeBegin(L"w:rFonts",true);
				pXmlWriter->WriteAttribute(L"w:hAnsi",L"Cambria Math");
				pXmlWriter->WriteAttribute(L"w:ascii",L"Cambria Math");
				pXmlWriter->WriteNodeEnd(L"w",true,true);
				pXmlWriter->WriteNodeBegin(L"w:sz",true);
				pXmlWriter->WriteAttribute(L"w:val",L"30");
				pXmlWriter->WriteNodeEnd(L"w",true,true);
				pXmlWriter->WriteNodeBegin(L"w:szCs",true);
				pXmlWriter->WriteAttribute(L"w:val",L"30");
				pXmlWriter->WriteNodeEnd(L"w",true,true);
				pXmlWriter->WriteNodeEnd(L"w:rPr",false,false);
			}
			else
			{
				std::wstring wsNameFont = pAttribute->GetFontName();
				pXmlWriter->WriteNodeBegin(L"w:rPr",false);
				pXmlWriter->WriteNodeBegin(L"w:rFonts",true);
				if(!wsNameFont.empty())
				{
					pXmlWriter->WriteAttribute(L"w:hAnsi",wsNameFont);
					pXmlWriter->WriteAttribute(L"w:ascii",wsNameFont);
				}
				else
				{
					pXmlWriter->WriteAttribute(L"w:hAnsi",L"Cambria Math");
					pXmlWriter->WriteAttribute(L"w:ascii",L"Cambria Math");
				}
				pXmlWriter->WriteNodeEnd(L"w",true,true);
				if(pAttribute->GetSize() == 0)
				{
					pXmlWriter->WriteNodeBegin(L"w:sz",true);
					pXmlWriter->WriteAttribute(L"w:val",L"30");
					pXmlWriter->WriteNodeEnd(L"w",true,true);
					pXmlWriter->WriteNodeBegin(L"w:szCs",true);
					pXmlWriter->WriteAttribute(L"w:val",L"30");
					pXmlWriter->WriteNodeEnd(L"w",true,true);
				}
				else if(pAttribute->GetSize() != 0)
				{
					pXmlWriter->WriteNodeBegin(L"w:sz",true);
					pXmlWriter->WriteAttribute(L"w:val",std::to_wstring(pAttribute->GetSize()));
					pXmlWriter->WriteNodeEnd(L"w",true,true);
					pXmlWriter->WriteNodeBegin(L"w:szCs",true);
					pXmlWriter->WriteAttribute(L"w:val",std::to_wstring(pAttribute->GetSize()));
					pXmlWriter->WriteNodeEnd(L"w",true,true);
				}
				if(!pAttribute->EmptyColor())
				{
					pXmlWriter->WriteNodeBegin(L"w:color",true);
					pXmlWriter->WriteAttribute(L"w:val",pAttribute->GetColor());
					pXmlWriter->WriteNodeEnd(L"w",true,true);
				}
				if(pAttribute->GetBold() && pAttribute->GetItal())
				{
					WriteStyNode(pXmlWriter,L"bi");
				}
				else if(pAttribute->GetBold())
				{
					pXmlWriter->WriteNodeBegin(L"m:sty", true);
					pXmlWriter->WriteAttribute(L"m:val",L"b");
					pXmlWriter->WriteNodeEnd(L"w",true,true);
					pXmlWriter->WriteNodeBegin(L"w:b",true);
					pXmlWriter->WriteNodeEnd(L"w",true,true);
					pXmlWriter->WriteNodeBegin(L"w:bCs",true);
					pXmlWriter->WriteNodeEnd(L"w",true,true);
				}
				else if(pAttribute->GetItal())
				{
					pXmlWriter->WriteNodeBegin(L"w:i",true);
					pXmlWriter->WriteNodeEnd(L"w",true,true);
				}
				if(pAttribute->GetStrike())
				{
					pXmlWriter->WriteNodeBegin(L"w:strike",true);
					pXmlWriter->WriteNodeEnd(L"w",true,true);
				}
				if(enTypeLang == TextDirection::RightToLeft)
				{
					switch (enTypeLang) {
					case StarMath::TextDirection::RightToLeft:
					{
						pXmlWriter->WriteNodeBegin(L"w:rtl",true);
						pXmlWriter->WriteNodeEnd(L"w",true,true);
						pXmlWriter->WriteNodeBegin(L"w:lang",true);
						pXmlWriter->WriteAttribute(L"w:bidi",L"ar-SA");
						pXmlWriter->WriteNodeEnd(L"w",true,true);
						break;
					}
					default:
						break;
					}
				}
				pXmlWriter->WriteNodeEnd(L"w:rPr",false,false);
			}
		}
		else if(TypeConversion::pptx == enTypeConversion || TypeConversion::xlsx == enTypeConversion)
		{
			if(pAttribute !=nullptr)
			{
				pXmlWriter->WriteNodeBegin(L"a:rPr",true);
				if(pAttribute->GetSize()!=0)
				{
					int iSize = pAttribute->GetSize();
					iSize = iSize*50;
					pXmlWriter->WriteAttribute(L"sz",iSize);
				}
				else
					pXmlWriter->WriteAttribute(L"sz",L"1500");
				if(pAttribute->GetBold())
					pXmlWriter->WriteAttribute(L"b",L"1");
				if(pAttribute->GetItal())
					pXmlWriter->WriteAttribute(L"i",L"1");
				if(pAttribute->GetStrike())
					pXmlWriter->WriteAttribute(L"strike",L"sngStrike");
				pXmlWriter->WriteAttribute(L"spc", -150);
				pXmlWriter->WriteNodeEnd(L"w",true,false);
				if(!pAttribute->GetColor().empty())
				{
				  pXmlWriter->WriteNodeBegin(L"a:solidFill",false);
				  pXmlWriter->WriteNodeBegin(L"a:srgbClr",true);
				  pXmlWriter->WriteAttribute(L"val",pAttribute->GetColor());
				  pXmlWriter->WriteNodeEnd(L"w",true,true);
				  pXmlWriter->WriteNodeEnd(L"a:solidFill",false,false);
				}
				pXmlWriter->WriteNodeBegin(L"a:latin",true);
				if(!pAttribute->GetFontName().empty())
					pXmlWriter->WriteAttribute(L"typeface",pAttribute->GetFontName());
				else
					pXmlWriter->WriteAttribute(L"typeface",L"Cambria Math");
				pXmlWriter->WriteNodeEnd(L"w",true,true);
				pXmlWriter->WriteNodeEnd(L"a:rPr",false);
			}
			else
			{
				pXmlWriter->WriteNodeBegin(L"a:rPr",true);
				pXmlWriter->WriteAttribute(L"sz",L"1500");
				pXmlWriter->WriteNodeEnd(L"w",true,false);
				pXmlWriter->WriteNodeBegin(L"a:latin",true);
				pXmlWriter->WriteAttribute(L"typeface",L"Cambria Math");
				pXmlWriter->WriteNodeEnd(L"w",true,true);
				pXmlWriter->WriteNodeEnd(L"a:rPr",false);
			}
		}
	}
	void CConversionSMtoOOXML::PropertiesMFPR(const std::wstring &wsType, XmlUtils::CXmlWriter* pXmlWriter, CAttribute* pAttribute, const TypeConversion &enTypeConversion)
	{
		pXmlWriter->WriteNodeBegin(L"m:fPr",false);
		if(!wsType.empty())
		{
			pXmlWriter->WriteNodeBegin(L"m:type",true);
			pXmlWriter->WriteAttribute(L"m:val",wsType);
			pXmlWriter->WriteNodeEnd(L"w",true,true);
		}
		WriteCtrlPrNode(pXmlWriter,pAttribute,enTypeConversion);
		pXmlWriter->WriteNodeEnd(L"m:fPr",false,false);
	}
	void CConversionSMtoOOXML::PropertiesNaryPr(const TypeElement& enTypeOp,bool bEmptySub,bool bEmptySup, XmlUtils::CXmlWriter* pXmlWriter,CAttribute* pAttribute,const TypeConversion &enTypeConversion,const bool& bEQN)
	{
		pXmlWriter->WriteNodeBegin(L"m:naryPr",false);
		switch(enTypeOp)
		{
		case TypeElement::sum:
		{
		WriteChrNode(L"\u2211",pXmlWriter);
		break;
		}
		case TypeElement::prod:
		WriteChrNode(L"\u220F",pXmlWriter);
		break;
		case TypeElement::coprod:
		WriteChrNode(L"\u2210",pXmlWriter);
		break;
		case TypeElement::iint:
		WriteChrNode(L"\u222C",pXmlWriter);
		break;
		case TypeElement::iiint:
		WriteChrNode(L"\u222D",pXmlWriter);
		break;
		case TypeElement::lint:
		WriteChrNode(L"\u222E",pXmlWriter);
		break;
		case TypeElement::llint:
		WriteChrNode(L"\u222F",pXmlWriter);
		break;
		case TypeElement::lllint:
		WriteChrNode(L"\u2230",pXmlWriter);
		break;
		case TypeElement::inter:
		WriteChrNode(L"\u22C2",pXmlWriter);
		break;
		case TypeElement::UnionOp:
		WriteChrNode(L"\u22C3",pXmlWriter);
		break;
		case TypeElement::bigvee:
		WriteChrNode(L"\u22C1",pXmlWriter);
		break;
		case TypeElement::bigwedge:
		WriteChrNode(L"\u22C0",pXmlWriter);
		break;
		default:
		break;
		}
		 if(bEQN && (enTypeOp == TypeElement::Int || enTypeOp == TypeElement::iint || enTypeOp == TypeElement::iiint || enTypeOp == TypeElement::lint || enTypeOp == TypeElement::llint || enTypeOp == TypeElement::lllint))
			WriteLimLocNode(L"subSup",pXmlWriter);
		else
			WriteLimLocNode(L"undOvr",pXmlWriter);
		if(bEmptySub)
		{
			pXmlWriter->WriteNodeBegin(L"m:subHide",true);
			pXmlWriter->WriteAttribute(L"m:val",L"1");
			pXmlWriter->WriteNodeEnd(L"w",true,true);
		}
		if(bEmptySup)
		{
			pXmlWriter->WriteNodeBegin(L"m:supHide",true);
			pXmlWriter->WriteAttribute(L"m:val",L"1");
			pXmlWriter->WriteNodeEnd(L"w",true,true);
		}
		WriteCtrlPrNode(pXmlWriter,pAttribute,enTypeConversion);
		pXmlWriter->WriteNodeEnd(L"m:naryPr",false,false);
	}
	void CConversionSMtoOOXML::WriteNodeConversion(const std::wstring &wsNameBlock, CElement *pValueBlock,XmlUtils::CXmlWriter* pXmlWriter)
	{
		if(pValueBlock != nullptr)
		{
			pXmlWriter->WriteNodeBegin(wsNameBlock,false);
			pValueBlock->ConversionToOOXML(pXmlWriter);
			pXmlWriter->WriteNodeEnd(wsNameBlock,false,false);
		}
		else
		{
			pXmlWriter->WriteNodeBegin(wsNameBlock,true);
			pXmlWriter->WriteNodeEnd(L"",true,true);
		}
	}
	std::wstring CConversionSMtoOOXML::GetOOXML()
	{
		return m_pXmlWriter->GetXmlString();
	}
	void CConversionSMtoOOXML::EndConversion()
	{
		m_pXmlWriter->WriteNodeEnd(L"m:oMath",false,false);
		m_pXmlWriter->WriteNodeEnd(L"m:oMathPara",false,false);
	}
	void CConversionSMtoOOXML::PropertiesFuncPr(XmlUtils::CXmlWriter* pXmlWriter,CAttribute* pAttribute,const TypeConversion &enTypeConversion)
	{
		pXmlWriter->WriteNodeBegin(L"m:funcPr", false);
		pXmlWriter->WriteNodeBegin(L"m:ctrlPr", false);
		StandartProperties(pXmlWriter,pAttribute,enTypeConversion);
		pXmlWriter->WriteNodeEnd(L"m:ctrlPr", false,false);
		pXmlWriter->WriteNodeEnd(L"m:funcPr",false,false);
	}

	void CConversionSMtoOOXML::PropertiesDPr(XmlUtils::CXmlWriter *pXmlWriter, const std::wstring &wsOpenBracket,const std::wstring &wsCloseBracket, CAttribute* pAttribute, const TypeConversion &enTypeConversion, const TypeElement &enTypeBracket)
	{
		pXmlWriter->WriteNodeBegin(L"m:dPr",false);
		BracketTypeNotation(wsOpenBracket,wsCloseBracket,pXmlWriter);
		pXmlWriter->WriteNodeBegin(L"m:ctrlPr");
		StandartProperties(pXmlWriter,pAttribute,enTypeConversion);
		pXmlWriter->WriteNodeEnd(L"m:ctrlPr",false,false);
		pXmlWriter->WriteNodeEnd(L"m:dPr",false,false);
	}
	void CConversionSMtoOOXML::BracketTypeNotation(const std::wstring &wsOpenBracket, const std::wstring &wsCloseBracket, XmlUtils::CXmlWriter *pXmlWriter)
	{
		if(!wsOpenBracket.empty())
		{
			pXmlWriter->WriteNodeBegin(L"m:begChr", true);
			if(wsOpenBracket == L"none")
				pXmlWriter->WriteAttribute(L"m:val",L"");
			else
				pXmlWriter->WriteAttribute(L"m:val",wsOpenBracket);
			pXmlWriter->WriteNodeEnd(L"w",true,true);
		}
		if(!wsCloseBracket.empty())
		{
			pXmlWriter->WriteNodeBegin(L"m:endChr", true);
			if(wsCloseBracket == L"none")
				pXmlWriter->WriteAttribute(L"m:val",L"");
			else
				pXmlWriter->WriteAttribute(L"m:val", wsCloseBracket);
			pXmlWriter->WriteNodeEnd(L"w",true,true);
		}
	}
	void CConversionSMtoOOXML::PropertiesMPr(XmlUtils::CXmlWriter *pXmlWriter, const TypeElement &enTypeMatrix, CAttribute* pAttribute, const TypeConversion &enTypeConversion, const unsigned int &iDimension)
	{
		pXmlWriter->WriteNodeBegin(L"m:mPr",false);
		pXmlWriter->WriteNodeBegin(L"m:mcs",false);
		pXmlWriter->WriteNodeBegin(L"m:mc",false);
		pXmlWriter->WriteNodeBegin(L"m:mcPr",false);
		pXmlWriter->WriteNodeBegin(L"m:count",true);
		const std::wstring wsNumber = std::to_wstring(iDimension);
		pXmlWriter->WriteAttribute(L"m:val",wsNumber);
		pXmlWriter->WriteNodeEnd(L"w",true,true);
		pXmlWriter->WriteNodeBegin(L"m:mcJc",true);
		pXmlWriter->WriteAttribute(L"m:val",L"center");
		pXmlWriter->WriteNodeEnd(L"w",true,true);
		pXmlWriter->WriteNodeEnd(L"m:mcPr",false,false);
		pXmlWriter->WriteNodeEnd(L"m:mc",false,false);
		pXmlWriter->WriteNodeEnd(L"m:mcs",false,false);
		pXmlWriter->WriteNodeBegin(L"m:ctrlPr",false);
		StandartProperties(pXmlWriter,pAttribute,enTypeConversion);
		pXmlWriter->WriteNodeEnd(L"m:ctrlPr",false,false);
		pXmlWriter->WriteNodeEnd(L"m:mPr",false,false);
	}
	void CConversionSMtoOOXML::NodeGrade(XmlUtils::CXmlWriter *pXmlWriter,CElement* pValueGrade,CAttribute* pAttribute)
	{
		pXmlWriter->WriteNodeBegin(L"m:d",false);
		pXmlWriter->WriteNodeBegin(L"m:dPr",false);
		pXmlWriter->WriteNodeBegin(L"m:begChr",true);
		pXmlWriter->WriteAttribute(L"m:val",L"");
		pXmlWriter->WriteNodeEnd(L"w",true,true);
		pXmlWriter->WriteNodeBegin(L"m:endChr",true);
		pXmlWriter->WriteAttribute(L"m:val",L"\u23AA");
		pXmlWriter->WriteNodeEnd(L"w",true,true);
		pXmlWriter->WriteNodeBegin(L"m:ctrlPr",false);
		StandartProperties(pXmlWriter,pAttribute,pValueGrade->GetTypeConversion());
		pXmlWriter->WriteNodeEnd(L"m:ctrlPr",false);
		pXmlWriter->WriteNodeEnd(L"m:dPr",false,false);
		CConversionSMtoOOXML::WriteNodeConversion(L"m:e",pValueGrade,pXmlWriter);
		pXmlWriter->WriteNodeEnd(L"m:d",false,false);
	}
	void CConversionSMtoOOXML::WriteCtrlPrNode(XmlUtils::CXmlWriter *pXmlWriter, CAttribute *pAttribute,const TypeConversion &enTypeConversion)
	{
		pXmlWriter->WriteNodeBegin(L"m:ctrlPr",false);
		StandartProperties(pXmlWriter,pAttribute,enTypeConversion);
		pXmlWriter->WriteNodeEnd(L"m:ctrlPr",false,false);
	}
	void CConversionSMtoOOXML::WriteChrNode(const std::wstring &wsTypeOp,XmlUtils::CXmlWriter* pXmlWriter)
	{
		pXmlWriter->WriteNodeBegin(L"m:chr",true);
		pXmlWriter->WriteAttribute(L"m:val",wsTypeOp);
		pXmlWriter->WriteNodeEnd(L"w",true,true);
	}
	void CConversionSMtoOOXML::WriteLimLocNode(const std::wstring &wsTypeLimLock,XmlUtils::CXmlWriter* pXmlWriter)
	{
		pXmlWriter->WriteNodeBegin(L"m:limLoc",true);
		pXmlWriter->WriteAttribute(L"m:val",wsTypeLimLock);
		pXmlWriter->WriteNodeEnd(L"w",true,true);
	}
	void CConversionSMtoOOXML::WriteRPrFName(const TypeElement &enTypeOp, XmlUtils::CXmlWriter *pXmlWriter,CAttribute* pAttribute,const std::wstring& wsNameOp,const TypeConversion &enTypeConversion)
	{
		pXmlWriter->WriteNodeBegin(L"m:r",false);
		pXmlWriter->WriteNodeBegin(L"m:rPr",false);
		pXmlWriter->WriteNodeBegin(L"m:sty",true);
		pXmlWriter->WriteAttribute(L"m:val",L"p");
		pXmlWriter->WriteNodeEnd(L"w",true,true);
		pXmlWriter->WriteNodeEnd(L"m:rPr",false,false);
		CConversionSMtoOOXML::StandartProperties(pXmlWriter,pAttribute,enTypeConversion);
		pXmlWriter->WriteNodeBegin(L"m:t",false);
		switch(enTypeOp)
		{
		case TypeElement::lim:
		pXmlWriter->WriteString(L"lim");
		break;
		case TypeElement::liminf:
		pXmlWriter->WriteString(L"lim inf");
		break;
		case TypeElement::limsup:
		pXmlWriter->WriteString(L"lim sup");
		break;
		case TypeElement::oper:
		pXmlWriter->WriteString(wsNameOp);
		break;
		default:
		break;
		}
		pXmlWriter->WriteNodeEnd(L"m:t",false,false);
		pXmlWriter->WriteNodeEnd(L"m:r",false,false);
	}
	void CConversionSMtoOOXML::WriteStyNode(XmlUtils::CXmlWriter *pXmlWriter, const std::wstring &wsAttributeNode)
	{
		pXmlWriter->WriteNodeBegin(L"m:sty", true);
		pXmlWriter->WriteAttribute(L"m:val",wsAttributeNode);
		pXmlWriter->WriteNodeEnd(L"w",true,true);
	}
	void CConversionSMtoOOXML::WritePreserveBlock(XmlUtils::CXmlWriter *pXmlWriter,CAttribute* pAttribute,const TypeConversion &enTypeConversion)
	{
		pXmlWriter->WriteNodeBegin(L"m:r",false);
		StandartProperties(pXmlWriter,pAttribute,enTypeConversion);
		pXmlWriter->WriteNodeBegin(L"m:t",true);
		pXmlWriter->WriteAttribute(L"xml:space",L"preserve");
		pXmlWriter->WriteNodeEnd(L"w",true,false);
		pXmlWriter->WriteNodeEnd(L"m:t",false,false);
		pXmlWriter->WriteNodeEnd(L"m:r",false,false);
	}
	void CConversionSMtoOOXML::WriteLimUpOrLowNode(XmlUtils::CXmlWriter *pXmlWriter, const std::wstring& wsNameNode, CElement* pValue, const TypeElement& enType, CAttribute* pAttribute, const TypeConversion &enTypeConvers, const std::wstring& wsName, CElement* pIndex)
	{
		pXmlWriter->WriteNodeBegin(wsNameNode,false);
		pXmlWriter->WriteNodeBegin(wsNameNode+L"Pr",false);
		CConversionSMtoOOXML::WriteCtrlPrNode(pXmlWriter,nullptr,enTypeConvers);
		pXmlWriter->WriteNodeEnd(wsNameNode+L"Pr",false,false);
		pXmlWriter->WriteNodeBegin(L"m:e",false);
		CConversionSMtoOOXML::WriteRPrFName(enType,pXmlWriter,pAttribute,wsName,enTypeConvers);
		pXmlWriter->WriteNodeEnd(L"m:e",false,false);
		if(pValue!= nullptr && pIndex != nullptr)
		{
			pXmlWriter->WriteNodeBegin(L"m:lim",false);
			pIndex->ConversionToOOXML(pXmlWriter);
			pValue->ConversionToOOXML(pXmlWriter);
			pXmlWriter->WriteNodeEnd(L"m:lim",false,false);
		}
		else if(pValue!=nullptr && pIndex == nullptr)
			WriteNodeConversion(L"m:lim",pValue,pXmlWriter);
		else if(pValue == nullptr && pIndex !=nullptr)
			WriteNodeConversion(L"m:lim",pIndex,pXmlWriter);
		pXmlWriter->WriteNodeEnd(wsNameNode,false,false);
	}
	void CConversionSMtoOOXML::ElementConversion(XmlUtils::CXmlWriter *pXmlWriter, CElement *pElement)
	{
		if(pElement!= nullptr)
			pElement->ConversionToOOXML(pXmlWriter);
	}
}

