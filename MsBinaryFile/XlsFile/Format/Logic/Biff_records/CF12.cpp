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
			//todooo
			//rgbCT = BiffStructurePtr(new CFFilter);
			//rgbCT->load(record);
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
void CF12::writeFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	frtRefHeader.rt = 0x087A;
    record << frtRefHeader;
    record << ct << cp;

    record.reserveNunBytes(4);
    auto ccePos = record.getRdPtr();
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
						if(dxfObj.m_oFill->m_oPatternFill->m_oBgColor.IsInit() && dxfObj.m_oFill->m_oPatternFill->m_oBgColor->m_oIndexed.IsInit())
						{
							dxf.dxfn->icvBNinch = false;
							dxf.dxfn->dxfpat.icvBackground = dxfObj.m_oFill->m_oPatternFill->m_oBgColor->m_oIndexed->GetValue();
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

