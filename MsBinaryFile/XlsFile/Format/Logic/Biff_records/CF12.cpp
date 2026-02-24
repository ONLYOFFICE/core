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

#include "CF12.h"
#include "CFEx.h"

#include "../Biff_structures/CFMultistate.h"
#include "../Biff_structures/CFDatabar.h"
#include "../Biff_structures/CFGradient.h"
#include "../Biff_structures/CFFilter.h"

#include "../../../../../OOXML/XlsxFormat/ComplexTypes_Spreadsheet.h"
#include "../../../../../OOXML/XlsxFormat/Styles/dxf.h"
#include "../../../../../OOXML/XlsxFormat/Styles/Xfs.h"
#include "../../../../../OOXML/XlsxFormat/Styles/NumFmts.h"
#include "../../../../../OOXML/XlsxFormat/Styles/Fonts.h"
#include "../../../../../OOXML/XlsxFormat/Styles/Fills.h"
#include "../../../../../OOXML/XlsxFormat/Styles/Borders.h"
#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

CF12::CF12(const CellRef& cell_base_ref)
:	rgce1(cell_base_ref),
	rgce2(cell_base_ref),
	fmlaActive(cell_base_ref)
{
	dxfId_ = -1;
	ipriority_ = 0;
}


CF12::~CF12()
{
}


BaseObjectPtr CF12::clone()
{
	return BaseObjectPtr(new CF12(*this));
}

void CF12::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();

	record >> frtRefHeader;
	record >> ct >> cp;
	
	unsigned short cce1;
	unsigned short cce2;
	
	record >> cce1 >> cce2;
	record >> dxf;
	
	rgce1.load(record, cce1);
	rgce2.load(record, cce2);
	fmlaActive.load(record);
	
	unsigned char flags;
	record >> flags;
	fStopIfTrue = GETBIT(flags, 1);
	
    record >> ipriority >> icfTemplate;
	
	unsigned char cbTemplateParm;
	
	record >> cbTemplateParm;
	record >> rgbTemplateParms;

	switch(ct)
	{
		case 0x03:
			rgbCT = BiffStructurePtr(new CFGradient);
			rgbCT->load(record);
			break;
		case 0x04:
			rgbCT = BiffStructurePtr(new CFDatabar);
			rgbCT->load(record);
			break;
		case 0x05:
			rgbCT = BiffStructurePtr(new CFFilter);
			rgbCT->load(record);
			break;
		case 0x06:
			rgbCT = BiffStructurePtr(new CFMultistate);
			rgbCT->load(record);
			break;
	}
	
	ipriority_	= ++record.getGlobalWorkbookInfo()->cmt_rules;
	
	std::wstringstream strm;
	dxf.serialize(strm);
	
	dxfId_ = global_info->RegistrDxfn(strm.str());
}
void ProcessBorderProp( OOX::Spreadsheet::CBorderProp* prop, unsigned char& DgPtr, unsigned char& icvPtr)
{
	if(prop->m_oStyle.IsInit())
	{
		switch(prop->m_oStyle->GetValue())
		{
			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleNone:
			{
				DgPtr = 0;
				break;
			}
			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleThin:
			{
				DgPtr = 1;
				break;
			}
			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleMedium:
			{
				DgPtr = 2;
				break;
			}

			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleDashed:
			{
				DgPtr = 3;
				break;
			}
			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleDotted:
			{
				DgPtr = 4;
				break;
			}
			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleThick:
			{
				DgPtr = 5;
				break;
			}
			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleDouble:
			{
				DgPtr = 6;
				break;
			}
			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleHair:
			{
				DgPtr = 7;
				break;
			}
			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleMediumDashed:
			{
				DgPtr = 8;
				break;
			}
			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleDashDot:
			{
				DgPtr = 9;
				break;
			}
			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleMediumDashDot:
			{
				DgPtr = 10;
				break;
			}
			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleDashDotDot:
			{
				DgPtr = 11;
				break;
			}
			case SimpleTypes::Spreadsheet::EBorderStyle::borderstyleSlantDashDot:
			{
				DgPtr = 13;
				break;
			}
			default:
				break;
		}
	}
	if(prop->m_oColor.IsInit() && prop->m_oColor->m_oIndexed.IsInit())
		icvPtr = prop->m_oColor->m_oIndexed->GetValue();
}

void CF12::writeFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	frtRefHeader.rt = 0x087A;
    record << frtRefHeader;
    record << ct << cp;

    record.reserveNunBytes(4);
    auto ccePos = record.getRdPtr();
	if(dxf.dxfn != nullptr)
	{
		//todo dxf conversion from global info
		if(dxfId_ >= 0 && global_info->arrUserDxfs.size() > dxfId_)
		{
			OOX::Spreadsheet::CDxf dxfObj;
			XmlUtils::CXmlLiteReader oReader;
			if(oReader.FromString(global_info->arrUserDxfs.at(dxfId_)))
			{
				if(oReader.ReadNextNode())
				{
					dxfObj.fromXML(oReader);
					if(dxfObj.m_oFill.IsInit() && dxfObj.m_oFill->m_oPatternFill.IsInit())
					{
						dxf.dxfn->flsNinch = false;
						dxf.dxfn->ibitAtrPat = true;
						dxf.dxfn->dxfpat.fls  = 1;
						if(dxfObj.m_oFill->m_oPatternFill->m_oFgColor.IsInit() && dxfObj.m_oFill->m_oPatternFill->m_oFgColor->m_oIndexed.IsInit())
						{
							dxf.dxfn->icvFNinch = false;
							dxf.dxfn->dxfpat.icvForeground = dxfObj.m_oFill->m_oPatternFill->m_oFgColor->m_oIndexed->GetValue();
						}
						else if(dxfObj.m_oFill->m_oPatternFill->m_oFgColor.IsInit())
						{
							dxf.dxfn->icvFNinch= false;
							ExtProp fgColor;
							if(dxf.dxfn->xfext == nullptr)
							{
								auto Ext = new XFExtNoFRT;
								dxf.dxfn->xfext = XFExtNoFRTPtr(Ext);
							}
							if(dxfObj.m_oFill->m_oPatternFill->m_oFgColor->m_oThemeColor.IsInit())
							{
								fgColor.extType = ExtProp::ForeColor;
								fgColor.extPropData.color.xclrType = 3;
								fgColor.extPropData.color.xclrValue = dxfObj.m_oFill->m_oPatternFill->m_oFgColor->m_oThemeColor->GetValue();

							}
							else if(dxfObj.m_oFill->m_oPatternFill->m_oFgColor->m_oRgb.IsInit())
							{
								fgColor.extType = ExtProp::ForeColor;
								fgColor.extPropData.color.xclrType = 2;
								fgColor.extPropData.color.xclrValue = dxfObj.m_oFill->m_oPatternFill->m_oFgColor->m_oRgb->ToInt();
							}

							dxf.dxfn->xfext->mapRgExt.emplace(ExtProp::ForeColor, fgColor);
						}
						if(dxfObj.m_oFill->m_oPatternFill->m_oBgColor.IsInit() && dxfObj.m_oFill->m_oPatternFill->m_oBgColor->m_oIndexed.IsInit())
						{
							dxf.dxfn->icvBNinch = false;
							dxf.dxfn->dxfpat.icvBackground = dxfObj.m_oFill->m_oPatternFill->m_oBgColor->m_oIndexed->GetValue();
						}
						else if(dxfObj.m_oFill->m_oPatternFill->m_oBgColor.IsInit())
						{
							ExtProp bgColor;
							dxf.dxfn->icvBNinch = false;
							if(dxf.dxfn->xfext == nullptr)
							{
								auto Ext = new XFExtNoFRT;
								dxf.dxfn->xfext = XFExtNoFRTPtr(Ext);
							}
							if(dxfObj.m_oFill->m_oPatternFill->m_oBgColor->m_oThemeColor.IsInit())
							{
								bgColor.extType = ExtProp::BackColor;
								bgColor.extPropData.color.xclrType = 3;
								bgColor.extPropData.color.xclrValue = dxfObj.m_oFill->m_oPatternFill->m_oBgColor->m_oThemeColor->GetValue();

							}
							else if(dxfObj.m_oFill->m_oPatternFill->m_oBgColor->m_oRgb.IsInit())
							{
								bgColor.extType = ExtProp::BackColor;
								bgColor.extPropData.color.xclrType = 2;
								bgColor.extPropData.color.xclrValue = dxfObj.m_oFill->m_oPatternFill->m_oBgColor->m_oRgb->ToInt();
							}

							dxf.dxfn->xfext->mapRgExt.emplace(ExtProp::BackColor, bgColor);
						}
					}
					if(dxfObj.m_oFont.IsInit())
					{
						dxf.dxfn->ibitAtrFnt = true;
						if(dxfObj.m_oFont->m_oBold.IsInit() && dxfObj.m_oFont->m_oBold->m_oVal.GetValue())
						{	dxf.dxfn->dxffntd.fBlsNinch = false;
							dxf.dxfn->dxffntd.stxp.bls = 0x02BC;
						}
						if(dxfObj.m_oFont->m_oStrike.IsInit() && dxfObj.m_oFont->m_oStrike->m_oVal.GetValue())
						{
							dxf.dxfn->dxffntd.stxp.ts.ftsStrikeout = true;
							dxf.dxfn->dxffntd.tsNinch.ftsStrikeout = false;
						}
						if(dxfObj.m_oFont->m_oItalic.IsInit() && dxfObj.m_oFont->m_oItalic->m_oVal.GetValue())
						{
							dxf.dxfn->dxffntd.stxp.ts.ftsItalic = true;
							dxf.dxfn->dxffntd.tsNinch.ftsItalic = false;
						}
						if(dxfObj.m_oFont->m_oSz.IsInit())
							dxf.dxfn->dxffntd.stxp.twpHeight = dxfObj.m_oFont->m_oSz->m_oVal->GetValue() * 20;
						if(dxfObj.m_oFont->m_oUnderline.IsInit())
						{
							dxf.dxfn->dxffntd.fUlsNinch = false;
							if(dxfObj.m_oFont->m_oUnderline->m_oUnderline->GetValue() == SimpleTypes::Spreadsheet::EUnderline::underlineSingle)
								dxf.dxfn->dxffntd.stxp.uls = 1;
							else if(dxfObj.m_oFont->m_oUnderline->m_oUnderline->GetValue() == SimpleTypes::Spreadsheet::EUnderline::underlineDouble)
								dxf.dxfn->dxffntd.stxp.uls = 2;
							else if(dxfObj.m_oFont->m_oUnderline->m_oUnderline->GetValue() == SimpleTypes::Spreadsheet::EUnderline::underlineSingleAccounting)
								dxf.dxfn->dxffntd.stxp.uls = 0x21;
							else if(dxfObj.m_oFont->m_oUnderline->m_oUnderline->GetValue() == SimpleTypes::Spreadsheet::EUnderline::underlineDoubleAccounting)
								dxf.dxfn->dxffntd.stxp.uls = 0x22;
						}
						if(dxfObj.m_oFont->m_oColor.IsInit() && dxfObj.m_oFont->m_oColor->m_oIndexed.IsInit())
							dxf.dxfn->dxffntd.icvFore = dxfObj.m_oFont->m_oColor->m_oIndexed->GetValue();
					}
					if(dxfObj.m_oBorder.IsInit())
					{
						dxf.dxfn->ibitAtrBdr = true;
						if(dxfObj.m_oBorder->m_oBottom.IsInit())
						{
							dxf.dxfn->glBottomNinch = false;
							ProcessBorderProp(dxfObj.m_oBorder->m_oBottom.GetPointer(), dxf.dxfn->dxfbdr.dgBottom, dxf.dxfn->dxfbdr.icvBottom);
						}
						if(dxfObj.m_oBorder->m_oTop.IsInit())
						{
							dxf.dxfn->glTopNinch = false;
							ProcessBorderProp(dxfObj.m_oBorder->m_oTop.GetPointer(), dxf.dxfn->dxfbdr.dgTop, dxf.dxfn->dxfbdr.icvTop);
						}
						if(dxfObj.m_oBorder->m_oStart.IsInit())
						{
							dxf.dxfn->glLeftNinch  = false;
							ProcessBorderProp(dxfObj.m_oBorder->m_oStart.GetPointer(), dxf.dxfn->dxfbdr.dgLeft, dxf.dxfn->dxfbdr.icvLeft);
						}
						if(dxfObj.m_oBorder->m_oEnd.IsInit())
						{
							dxf.dxfn->glRightNinch  = false;
							ProcessBorderProp(dxfObj.m_oBorder->m_oEnd.GetPointer(), dxf.dxfn->dxfbdr.dgRight, dxf.dxfn->dxfbdr.icvRight);
						}
						if(dxfObj.m_oBorder->m_oDiagonal.IsInit())
						{
							dxf.dxfn->glDiagDownNinch  = false;
							dxf.dxfn->glDiagUpNinch = false;
							if(dxfObj.m_oBorder->m_oDiagonalDown.IsInit() && dxfObj.m_oBorder->m_oDiagonalDown->GetValue())
								dxf.dxfn->dxfbdr.bitDiagDown = true;
							if(dxfObj.m_oBorder->m_oDiagonalUp.IsInit() && dxfObj.m_oBorder->m_oDiagonalUp->GetValue())
								dxf.dxfn->dxfbdr.bitDiagUp = true;
							ProcessBorderProp(dxfObj.m_oBorder->m_oDiagonal.GetPointer(), dxf.dxfn->dxfbdr.dgDiag, dxf.dxfn->dxfbdr.icvDiag);
						}
					}
					if(dxfObj.m_oNumFmt.IsInit())
					{
						dxf.dxfn->ibitAtrNum = true;
						if(dxfObj.m_oNumFmt->m_oNumFmtId.IsInit())
						{
							dxf.dxfn->fIfmtUser = false;
							dxf.dxfn->ifmtNinch = false;
							dxf.dxfn->dxfnum.fmt_id.ifmt = dxfObj.m_oNumFmt->m_oNumFmtId->m_eValue;
						}
						else if (dxfObj.m_oNumFmt->m_oFormatCode.IsInit())
						{
							dxf.dxfn->fIfmtUser = true;
							dxf.dxfn->dxfnum.user_defined.fmt = dxfObj.m_oNumFmt->m_oFormatCode.get();
						}
					}
					if(dxfObj.m_oProtection.IsInit())
					{
						dxf.dxfn->ibitAtrProt = true;
						dxf.dxfn->lockedNinch = false;
						dxf.dxfn->hiddenNinch = false;
						if(dxfObj.m_oProtection->m_oHidden.IsInit() && dxfObj.m_oProtection->m_oHidden->GetValue())
							dxf.dxfn->dxfprot.fHidden = true;
						if(dxfObj.m_oProtection->m_oLocked.IsInit() && dxfObj.m_oProtection->m_oLocked->GetValue())
							dxf.dxfn->dxfprot.fLocked = true;
					}
					if(dxfObj.m_oAlignment.IsInit())
					{
						dxf.dxfn->ibitAtrAlc = true;
						if(dxfObj.m_oAlignment->m_oHorizontal.IsInit())
						{
							dxf.dxfn->alchNinch = false;
							if(dxfObj.m_oAlignment->m_oHorizontal->GetValue()
								== SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentGeneral)
								dxf.dxfn->dxfalc.alc = 0;
							else if(dxfObj.m_oAlignment->m_oHorizontal->GetValue()
								== SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentLeft)
								dxf.dxfn->dxfalc.alc = 1;
							else if(dxfObj.m_oAlignment->m_oHorizontal->GetValue()
								== SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentCenter)
								dxf.dxfn->dxfalc.alc = 2;
							else if(dxfObj.m_oAlignment->m_oHorizontal->GetValue()
								== SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentRight)
								dxf.dxfn->dxfalc.alc = 3;
							else if(dxfObj.m_oAlignment->m_oHorizontal->GetValue()
								== SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentFill)
								dxf.dxfn->dxfalc.alc = 4;
							else if(dxfObj.m_oAlignment->m_oHorizontal->GetValue()
								== SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentJustify)
								dxf.dxfn->dxfalc.alc = 5;
							else if(dxfObj.m_oAlignment->m_oHorizontal->GetValue()
								== SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentCenterContinuous)
								dxf.dxfn->dxfalc.alc = 6;
							else if(dxfObj.m_oAlignment->m_oHorizontal->GetValue()
								== SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentDistributed)
								dxf.dxfn->dxfalc.alc = 7;
						}
						if(dxfObj.m_oAlignment->m_oWrapText.IsInit())
						{
							dxf.dxfn->wrapNinch = false;
							dxf.dxfn->dxfalc.fWrap = dxfObj.m_oAlignment->m_oWrapText->GetValue();
						}
						if(dxfObj.m_oAlignment->m_oVertical.IsInit())
						{
							dxf.dxfn->alcvNinch = false;
							if(dxfObj.m_oAlignment->m_oVertical->GetValue()
								== SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentTop)
							dxf.dxfn->dxfalc.alcv = 0;
							else if(dxfObj.m_oAlignment->m_oVertical->GetValue()
								== SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentCenter)
							dxf.dxfn->dxfalc.alcv = 1;
							else if(dxfObj.m_oAlignment->m_oVertical->GetValue()
								== SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentBottom)
							dxf.dxfn->dxfalc.alcv = 2;
							else if(dxfObj.m_oAlignment->m_oVertical->GetValue()
								== SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentJustify)
							dxf.dxfn->dxfalc.alcv = 3;
							else if(dxfObj.m_oAlignment->m_oVertical->GetValue()
								== SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentDistributed)
							dxf.dxfn->dxfalc.alcv = 4;
						}
						if(dxfObj.m_oAlignment->m_oJustifyLastLine.IsInit())
						{
							dxf.dxfn->kintoNinch = false;
							dxf.dxfn->dxfalc.fJustLast = dxfObj.m_oAlignment->m_oJustifyLastLine->GetValue();
						}
						if(dxfObj.m_oAlignment->m_oTextRotation.IsInit())
						{
							dxf.dxfn->trotNinch  = false;
							dxf.dxfn->dxfalc.trot = dxfObj.m_oAlignment->m_oTextRotation.get();
						}
						if(dxfObj.m_oAlignment->m_oIndent.IsInit())
						{
							dxf.dxfn->cIndentNinch  = false;
							dxf.dxfn->dxfalc.cIndent = dxfObj.m_oAlignment->m_oIndent.get();
						}
						if(dxfObj.m_oAlignment->m_oRelativeIndent.IsInit())
						{
							dxf.dxfn->cIndentNinch  = false;
							dxf.dxfn->dxfalc.iIndent = dxfObj.m_oAlignment->m_oRelativeIndent.get();
						}
						if(dxfObj.m_oAlignment->m_oShrinkToFit.IsInit())
						{
							dxf.dxfn->fShrinkNinch   = false;
							dxf.dxfn->dxfalc.fShrinkToFit = dxfObj.m_oAlignment->m_oShrinkToFit->GetValue();
						}
						if(dxfObj.m_oAlignment->m_oReadingOrder.IsInit())
						{
							dxf.dxfn->iReadingOrderNinch = false;
							dxf.dxfn->dxfalc.iReadingOrder = dxfObj.m_oAlignment->m_oReadingOrder.get();
						}
					}
				}
			}
		}
	}
    record << dxf;
    auto rgce1pos = record.getRdPtr();
    auto dxfSize = rgce1pos - ccePos;
    rgce1.save(record);
    unsigned short rgceSize = record.getRdPtr() - rgce1pos;
    record.RollRdPtrBack(rgceSize + dxfSize + 4);
    record << rgceSize;
    record.skipNunBytes(2 + rgceSize + dxfSize);

    auto rgce2pos = record.getRdPtr();
    rgce2.save(record);
    rgceSize = record.getRdPtr() - rgce2pos;
    record.RollRdPtrBack((record.getRdPtr() - ccePos) + 2);
    record << rgceSize;
    record.skipNunBytes((rgce2pos - ccePos) + rgceSize);

    fmlaActive.save(record);

    unsigned char flags = 0;

    SETBIT(flags, 1, fStopIfTrue);
    record << flags;
    BYTE cbTemplateParm = 16;
    record << ipriority << icfTemplate << cbTemplateParm;
    record << rgbTemplateParms;
    if(rgbCT != nullptr)
        rgbCT->save(record);

}
int CF12::serialize(std::wostream & stream)
{
	CFEx * cfEx = dynamic_cast<CFEx *>(m_CFEx.get());
	if (cfEx)
	{
	}
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"cfRule")
		{
			switch(ct)
			{
				case 1:	CP_XML_ATTR(L"type", L"cellIs");		break;
				case 2:	CP_XML_ATTR(L"type", L"expression");	break;
				case 3:	CP_XML_ATTR(L"type", L"colorScale");	break;
				case 4:	CP_XML_ATTR(L"type", L"dataBar");		break;
				case 6:	CP_XML_ATTR(L"type", L"iconSet");		break;
			}
			if (ct == 1)
			{
				switch(cp)
				{
					case 1:	CP_XML_ATTR(L"operator", L"between");			break;
					case 2:	CP_XML_ATTR(L"operator", L"notBetween");		break;
					case 3:	CP_XML_ATTR(L"operator", L"equal");				break;
					case 4:	CP_XML_ATTR(L"operator", L"notEqual");			break;
					case 5:	CP_XML_ATTR(L"operator", L"greaterThan");		break;
					case 6:	CP_XML_ATTR(L"operator", L"lessThan");			break;
					case 7:	CP_XML_ATTR(L"operator", L"greaterThanOrEqual");break;
					case 8:	CP_XML_ATTR(L"operator", L"lessThanOrEqual");	break;
				}
			}
			CP_XML_ATTR(L"priority", ipriority_);
			CP_XML_ATTR(L"stopIfTrue", fStopIfTrue);

			if (dxfId_ >= 0)
				CP_XML_ATTR(L"dxfId", dxfId_);

			if (rgbCT)
				rgbCT->serialize(CP_XML_STREAM());

			//что использовать зависит от типа ... todooo
			std::wstring s	= fmlaActive.getAssembledFormula();
			std::wstring s1 = rgce1.getAssembledFormula();
			std::wstring s2 = rgce2.getAssembledFormula();
			
			if (!s1.empty() || !s2.empty())
			{
				CP_XML_NODE(L"formula")
				{
					if (!s1.empty()) 
						CP_XML_STREAM() << XmlUtils::EncodeXmlString(s1);
					else if(!s2.empty()) 
						CP_XML_STREAM() << XmlUtils::EncodeXmlString(s2);
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

