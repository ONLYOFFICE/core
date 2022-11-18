/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

#include "TablePropertiesMapping.h"

namespace DocFileFormat
{
	TablePropertiesMapping::TablePropertiesMapping (XMLTools::CStringXmlWriter* pWriter, StyleSheet* styles, std::vector<short>* grid, bool isTableStyleNeeded ):
			PropertiesMapping(pWriter), _tblPr(NULL), _tblGrid(NULL), _tblBorders(NULL), _grid(grid),
			brcLeft(NULL), brcTop(NULL), brcBottom(NULL), brcRight(NULL), brcHorz(NULL), brcVert(NULL), _styles(styles),
			_isTableStyleNeeded(isTableStyleNeeded)
{
    _tblPr		= new XMLTools::XMLElement( L"w:tblPr");
    _tblBorders = new XMLTools::XMLElement( L"w:tblBorders");
}
TablePropertiesMapping::~TablePropertiesMapping()
{
	RELEASEOBJECT (_tblPr);
	RELEASEOBJECT (_tblGrid);
	RELEASEOBJECT (_tblBorders);
}

void TablePropertiesMapping::Apply(IVisitable* visited)
{
	TablePropertyExceptions* tapx = static_cast<TablePropertyExceptions*>(visited);

	XMLTools::XMLElement	tblCellMar(L"w:tblCellMar");
	XMLTools::XMLElement	tblLayout(L"w:tblLayout");
	XMLTools::XMLElement	tblpPr(L"w:tblpPr");
	XMLTools::XMLAttribute	layoutType(L"w:type", L"fixed");

	_CP_OPT(short) tblIndent;
	short gabHalf = 0;
	short marginLeft = 0;
	short marginRight = 0;

	short cellSpacing = 0;

	BYTE itcFirst = 0;
	BYTE itcLim = 0;

	bool bTableW = false;

	for (std::list<SinglePropertyModifier>::iterator iter = tapx->grpprl->begin(); iter != tapx->grpprl->end(); iter++)
	{
		switch (iter->OpCode)
		{
		case sprmOldTDxaGapHalf:
		case sprmTDxaGapHalf:
		{
			gabHalf = FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize);
		}break;
		case sprmOldTDefTable:
		case sprmTDefTable:
		{
			//table definition
			SprmTDefTable tDef(iter->Arguments, iter->argumentsSize);
			//Workaround for retrieving the indent of the table:
			//In some files there is a indent but no sprmTWidthIndent is set.
			//For this cases we can calculate the indent of the table by getting the 
			//first boundary of the TDef and adding the padding of the cells

			tblIndent = gabHalf + FormatUtils::BytesToInt16(iter->Arguments, 1, iter->argumentsSize);
			//If there follows a real sprmTWidthIndent, this value will be overwritten

			//tblIndent = (std::max)((int)tblIndent,0); //cerere.doc
		}break;
		case sprmTTableWidth:
		{
			//preferred table width
			unsigned char fts = iter->Arguments[0];
			short width = FormatUtils::BytesToInt16(iter->Arguments, 1, iter->argumentsSize);

			XMLTools::XMLElement tblW(L"w:tblW");

			XMLTools::XMLAttribute w(L"w:w", FormatUtils::IntToWideString(width));
			XMLTools::XMLAttribute type(L"w:type", FormatUtils::MapValueToWideString(fts, &WidthType[0][0], 4, 5));

			tblW.AppendAttribute(type);
			tblW.AppendAttribute(w);

			_tblPr->AppendChild(tblW);

			bTableW = true;
		}break;
		case sprmTMerge:
		{
			itcFirst = iter->Arguments[0];
			itcLim = iter->Arguments[1];
		}break;
		case sprmTSplit:
		{

		}break;
		case sprmOldTJc:
		case sprmTJc:
		case sprmTJcRow:
		{
			appendValueElement(_tblPr, L"jc", FormatUtils::MapValueToWideString(iter->Arguments[0], &Global::JustificationCode[0][0], 10, 15), true);
		}break;
		case sprmTWidthIndent:
		{
			tblIndent = FtsWWidth_Indent(iter->Arguments).wWidth;
		}break;
		case sprmTIstd:
		case sprmTIstdPermute:
		{
			if (_isTableStyleNeeded)
			{
				size_t ind = FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize);

				std::wstring id = ind < _styles->Styles->size() ? StyleSheetMapping::MakeStyleId(_styles->Styles->at(ind)) : L"";

				if (id != std::wstring(L"TableNormal") && !id.empty())
				{
					appendValueElement(_tblPr, L"tblStyle", id, true);
				}
			}
		}break;
		case sprmTFBiDi:
		case sprmTFBiDi90:
		{
			appendValueElement(_tblPr, L"bidiVisual", FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize), true);
		}break;
		case sprmOldTTlp:
		case sprmTTlp:
		{
			appendValueElement(_tblPr, L"tblLook", FormatUtils::IntToFormattedWideString(FormatUtils::BytesToInt16(iter->Arguments, 2, iter->argumentsSize), L"%04x"), true);
		}break;
		case sprmTFAutofit:
		{
			if (iter->Arguments[0] == 1)
			{
				layoutType.SetValue(L"auto");
			}
		}break;
		case sprmTCellSpacing:
		case sprmTCellSpacingDefault:
		{
			unsigned char grfbrc = iter->Arguments[2];
			short wSpc = FormatUtils::BytesToInt16(iter->Arguments, 4, iter->argumentsSize);
			std::wstring strValue = FormatUtils::IntToWideString(wSpc);
			if (FormatUtils::BitmaskToBool((int)grfbrc, 0x01))
			{
				appendDxaElement(_tblPr, L"tblCellSpacing", strValue, true);
			}
			if (FormatUtils::BitmaskToBool((int)grfbrc, 0x02))
			{
				cellSpacing = wSpc;
			}
		}break;
		case sprmTCellPadding:
		case sprmTCellPaddingDefault:
		case sprmTCellPaddingOuter:
		{//default cell padding (margin)
			unsigned char grfbrc = iter->Arguments[2];
			short wMar = FormatUtils::BytesToInt16(iter->Arguments, 4, iter->argumentsSize);
			std::wstring strValue = FormatUtils::IntToWideString(wMar);

			if (FormatUtils::BitmaskToBool((int)grfbrc, 0x01))
			{
				appendDxaElement(&tblCellMar, L"top", strValue, true);
			}
			if (FormatUtils::BitmaskToBool((int)grfbrc, 0x02))
			{
				marginLeft = wMar;
			}
			if (FormatUtils::BitmaskToBool((int)grfbrc, 0x04))
			{
				appendDxaElement(&tblCellMar, L"bottom", strValue, true);
			}
			if (FormatUtils::BitmaskToBool((int)grfbrc, 0x08))
			{
				marginRight = wMar;
			}
		}break;
		case sprmTCHorzBands:
		{
			appendValueElement(_tblPr, L"tblStyleRowBandSize", iter->Arguments[0], true);
		}break;
		case sprmTCVertBands:
		{
			appendValueElement(_tblPr, L"tblStyleColBandSize", iter->Arguments[0], true);
		}break;
		case sprmTFNoAllowOverlap:
		{
			std::wstring tblOverlapVal(L"overlap");

			if (iter->Arguments[0])
			{
				tblOverlapVal = L"never";
			}

			appendValueElement(_tblPr, L"tblOverlap", tblOverlapVal, true);
		}break;
		case sprmOldTSetShd:
		case sprmTSetShdTable:
		{

			appendShading(_tblPr, ShadingDescriptor(iter->Arguments, iter->argumentsSize));
		}break;
		case sprmTTableBorders80:
		{
			const int size = 4;
			unsigned char brc80[size];

			memcpy(brc80, iter->Arguments, size);
			brcTop = std::shared_ptr<BorderCode>(new BorderCode(brc80, size));

			memcpy(brc80, (iter->Arguments + 4), size);
			brcLeft = std::shared_ptr<BorderCode>(new BorderCode(brc80, size));

			memcpy(brc80, (iter->Arguments + 8), size);
			brcBottom = std::shared_ptr<BorderCode>(new BorderCode(brc80, size));

			memcpy(brc80, (iter->Arguments + 12), size);
			brcRight = std::shared_ptr<BorderCode>(new BorderCode(brc80, size));

			memcpy(brc80, (iter->Arguments + 16), size);
			brcHorz = std::shared_ptr<BorderCode>(new BorderCode(brc80, size));

			memcpy(brc80, (iter->Arguments + 20), size);
			brcVert = std::shared_ptr<BorderCode>(new BorderCode(brc80, size));
		}break;
		case sprmOldTTableBorders:
		case sprmTTableBorders:
		{
			const int size = 8;
			unsigned char brc[size];

			memcpy(brc, iter->Arguments, size);
			brcTop = std::shared_ptr<BorderCode>(new BorderCode(brc, size));

			memcpy(brc, (iter->Arguments + 8), size);
			brcLeft = std::shared_ptr<BorderCode>(new BorderCode(brc, size));

			memcpy(brc, (iter->Arguments + 16), size);
			brcBottom = std::shared_ptr<BorderCode>(new BorderCode(brc, size));

			memcpy(brc, (iter->Arguments + 24), size);
			brcRight = std::shared_ptr<BorderCode>(new BorderCode(brc, size));

			memcpy(brc, (iter->Arguments + 32), size);
			brcHorz = std::shared_ptr<BorderCode>(new BorderCode(brc, size));

			memcpy(brc, (iter->Arguments + 40), size);
			brcVert = std::shared_ptr<BorderCode>(new BorderCode(brc, size));
		}break;
		case sprmTPc:
		{
			unsigned char flag = (iter->Arguments[0] & 0x30) >> 4;

			appendValueAttribute(&tblpPr, L"w:vertAnchor", FormatUtils::MapValueToWideString(flag, &Global::VerticalPositionCode[0][0], 4, 7));

			flag = (iter->Arguments[0] & 0xC0) >> 6;

			appendValueAttribute(&tblpPr, L"w:horzAnchor", FormatUtils::MapValueToWideString(flag, &Global::HorizontalPositionCode[0][0], 4, 7));
		}break;
		case sprmTDxaFromText:
		{
			appendValueAttribute(&tblpPr, L"w:leftFromText", FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize));
		}break;
		case sprmTDxaFromTextRight:
		{
			appendValueAttribute(&tblpPr, L"w:rightFromText", FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize));
		}break;
		case sprmTDyaFromText:
		{
			appendValueAttribute(&tblpPr, L"w:topFromText", FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize));
		}break;
		case sprmTDyaFromTextBottom:
		{
			appendValueAttribute(&tblpPr, L"w:bottomFromText", FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize));
		}break;
		case sprmTDxaAbs:
		{
			appendValueAttribute(&tblpPr, L"w:tblpX", FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize));
		}break;
		case sprmTDyaAbs:
		{
			appendValueAttribute(&tblpPr, L"w:tblpY", FormatUtils::BytesToInt16(iter->Arguments, 0, iter->argumentsSize));
		}break;
		}
	}
	if (false == bTableW)
	{
		XMLTools::XMLElement tblW(L"w:tblW");

		XMLTools::XMLAttribute w(L"w:w", L"0");
		XMLTools::XMLAttribute type(L"w:type", L"auto");

		tblW.AppendAttribute(type);
		tblW.AppendAttribute(w);

		_tblPr->AppendChild(tblW);
	}

	if (tblIndent)
	{
		XMLTools::XMLElement tblInd(L"w:tblInd");

		XMLTools::XMLAttribute tblIndW(L"w:w", FormatUtils::IntToWideString(*tblIndent));
		tblInd.AppendAttribute(tblIndW);

		XMLTools::XMLAttribute tblIndType(L"w:type", L"dxa");
		tblInd.AppendAttribute(tblIndType);

		_tblPr->AppendChild(tblInd);
	}

	if (tblpPr.GetAttributeCount() > 0)
	{
		_tblPr->AppendChild(tblpPr);
	}

	if (brcTop)
	{
		XMLTools::XMLElement topBorder(L"w:top");
		appendBorderAttributes(brcTop.get(), &topBorder);
		addOrSetBorder(_tblBorders, &topBorder);
	}

	if (brcLeft)
	{
		XMLTools::XMLElement leftBorder(L"w:left");
		appendBorderAttributes(brcLeft.get(), &leftBorder);
		addOrSetBorder(_tblBorders, &leftBorder);
	}

	if (brcBottom)
	{
		XMLTools::XMLElement bottomBorder(L"w:bottom");
		appendBorderAttributes(brcBottom.get(), &bottomBorder);
		addOrSetBorder(_tblBorders, &bottomBorder);
	}

	if (brcRight)
	{
		XMLTools::XMLElement rightBorder(L"w:right");
		appendBorderAttributes(brcRight.get(), &rightBorder);
		addOrSetBorder(_tblBorders, &rightBorder);
	}

	if (brcHorz)
	{
		XMLTools::XMLElement insideHBorder(L"w:insideH");
		appendBorderAttributes(brcHorz.get(), &insideHBorder);
		addOrSetBorder(_tblBorders, &insideHBorder);
	}

	if (brcVert)
	{
		XMLTools::XMLElement insideVBorder(L"w:insideV");
		appendBorderAttributes(brcVert.get(), &insideVBorder);
		addOrSetBorder(_tblBorders, &insideVBorder);
	}

	if (_tblBorders->GetChildCount() > 0)
	{
		_tblPr->AppendChild(*_tblBorders);
	}

	if (false == layoutType.GetValue().empty())
	{
		tblLayout.AppendAttribute(layoutType);
	}
	_tblPr->AppendChild(tblLayout);

	if ((marginLeft == 0) && (gabHalf != 0))
	{
		appendDxaElement(&tblCellMar, L"left", FormatUtils::IntToWideString(gabHalf), true);
	}
	else
	{
		appendDxaElement(&tblCellMar, L"left", FormatUtils::IntToWideString(marginLeft), true);
	}

	if ((marginRight == 0) && (gabHalf != 0))
	{
		appendDxaElement(&tblCellMar, L"right", FormatUtils::IntToWideString(gabHalf), true);
	}
	else
	{
		appendDxaElement(&tblCellMar, L"right", FormatUtils::IntToWideString(marginRight), true);
	}

	_tblPr->AppendChild(tblCellMar);

	if ((_tblPr->GetChildCount() > 0) || (_tblPr->GetAttributeCount() > 0))
	{
		m_pXmlWriter->WriteString(_tblPr->GetXMLString());
	}

	//append the grid
	_tblGrid = new XMLTools::XMLElement(L"w:tblGrid");

	for (size_t i = 0; i < _grid->size(); i++)
	{
		XMLTools::XMLElement gridCol(L"w:gridCol");
		if (_grid->at(i) > 0)
		{
			XMLTools::XMLAttribute gridColW(L"w:w", FormatUtils::IntToWideString(_grid->at(i)));
			gridCol.AppendAttribute(gridColW);
		}
		_tblGrid->AppendChild(gridCol);
	}

	m_pXmlWriter->WriteString(_tblGrid->GetXMLString());
}
}
