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

#include "PAGESETUP.h"

#include "../Biff_records/Header.h"
#include "../Biff_records/Footer.h"
#include "../Biff_records/HCenter.h"
#include "../Biff_records/VCenter.h"
#include "../Biff_records/LeftMargin.h"
#include "../Biff_records/RightMargin.h"
#include "../Biff_records/TopMargin.h"
#include "../Biff_records/BottomMargin.h"
#include "../Biff_records/Pls.h"
#include "../Biff_records/Continue.h"
#include "../Biff_records/Setup.h"
#include "../Biff_records/HeaderFooter.h" 
#include "../Biff_records/HorizontalPageBreaks.h"
#include "../Biff_records/VerticalPageBreaks.h"

namespace XLS
{
	PAGESETUP::PAGESETUP() {}
	PAGESETUP::~PAGESETUP() {}

	class Parenthesis_PAGESETUP_1 : public ABNFParenthesis
	{
		BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_PAGESETUP_1)
	public:
		BaseObjectPtr clone()
		{
			return BaseObjectPtr(new Parenthesis_PAGESETUP_1(*this));
		}

		const bool loadContent(BinProcessor& proc)
		{
			//?????????
			//if(!proc.mandatory(Pls(proc.getParent())))
			//{
			return false;
			//}
			//proc.repeated<Continue>(0, 0);
			return true;
		};
	};



	BaseObjectPtr PAGESETUP::clone()
	{
		return BaseObjectPtr(new PAGESETUP(*this));
	}


	// PAGESETUP = Header Footer HCenter VCenter [LeftMargin] [RightMargin] [TopMargin] [BottomMargin] [Pls *Continue] Setup
	const bool PAGESETUP::loadContent(BinProcessor& proc)
	{
		while (true)
		{
			CFRecordType::TypeId type = proc.getNextRecordType();

			if (type == rt_NONE)
			{
				proc.SkipRecord();
				continue; //file(6).xls
				//break;
			}

			switch (type)
			{
			case rt_Header:
			{
				if (proc.optional<Header>())
				{
					m_Header = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_Footer:
			{
				if (proc.optional<Footer>())
				{
					m_Footer = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_HCenter:
			{
				if (proc.optional<HCenter>())
				{
					m_HCenter = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_VCenter:
			{
				if (proc.optional<VCenter>())
				{
					m_VCenter = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_BottomMargin:	proc.optional<BottomMargin>();		break;
			case rt_TopMargin:		proc.optional<TopMargin>();			break;
			case rt_LeftMargin:		proc.optional<LeftMargin>();		break;
			case rt_RightMargin:	proc.optional<RightMargin>();		break;

			case rt_HorizontalPageBreaks: proc.optional<HorizontalPageBreaks>(); break;
			case rt_VerticalPageBreaks: proc.optional<VerticalPageBreaks>(); break;
			
			case rt_Pls:
				{
					if (proc.optional<Pls>())
					{
						m_Pls = elements_.back();
						elements_.pop_back();
					}
				}break;
			case rt_Setup:
			{
				if (proc.optional<Setup>())
				{
					m_Setup = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_HeaderFooter:
			{
				if (proc.optional<HeaderFooter>())
				{
					m_HeaderFooter = elements_.back();
					elements_.pop_back();
				}
			}break;
			default:
				return true;
		}
	}
	return true;
}

int PAGESETUP::serialize(std::wostream & stream)
{
	if (elements_.empty() && !m_Setup && !m_Header && !m_Footer) return 0;
	
	Setup * setup = dynamic_cast<Setup*>(m_Setup.get());

	bool header = false;
	bool footer = false;
	bool b = false, t = false, l = false, r = false ;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"pageMargins")
		{
			if (setup)
			{
				{
					CP_XML_ATTR(L"header", std::to_wstring(setup->numHdr.data.value));
					header = true;
				}

				{
					CP_XML_ATTR(L"footer", std::to_wstring(setup->numFtr.data.value));
					footer = true;
				}
			}
			for (std::list<XLS::BaseObjectPtr>::iterator it = elements_.begin(); it != elements_.end(); ++it)
			{
				switch((*it)->get_type())
				{
				case typeTopMargin:
					if (!t)
					{
						TopMargin* top = dynamic_cast<TopMargin*>(it->get());
						CP_XML_ATTR(L"top", std::to_wstring(top->num.data.value));
						t = true;
					}break;
				case typeBottomMargin:
					if (!b)
					{
						BottomMargin* bottom = dynamic_cast<BottomMargin*>(it->get());
						CP_XML_ATTR(L"bottom", std::to_wstring(bottom->num.data.value));
						b= true;
					}break;
				case typeLeftMargin:
					{
						LeftMargin* left = dynamic_cast<LeftMargin*>(it->get());
						CP_XML_ATTR(L"left", std::to_wstring(left->num.data.value));
						l= true;
					}break;				
				case typeRightMargin:
					{
						RightMargin* right = dynamic_cast<RightMargin*>(it->get());
						CP_XML_ATTR(L"right", std::to_wstring(right->num.data.value));
						r= true;
					}break;		
				default:
					break;
				}
			}
			if (!header)	CP_XML_ATTR(L"header"	, 0.5);
			if (!footer)	CP_XML_ATTR(L"footer"	, 0.5);
			if (!t)			CP_XML_ATTR(L"top"		, 1);
			if (!b)			CP_XML_ATTR(L"bottom"	, 1);
			if (!l)			CP_XML_ATTR(L"left"		, 0.75);
			if (!r)			CP_XML_ATTR(L"right"	, 0.75);
		}

		if (setup)
		{
			if (setup->iFitHeight > 0 && setup->iFitWidth == 1 && setup->iScale == 100)
			{
				setup->iScale = 90;
			}

			if (false == setup->fNoPls)
			{	
				CP_XML_NODE(L"pageSetup")// - для chartsheet аккуратнее
				{
					CP_XML_ATTR(L"paperSize", setup->iPaperSize);
					CP_XML_ATTR(L"firstPageNumber", setup->iPageStart);
					
					CP_XML_ATTR(L"scale", setup->iScale);
					
					if (setup->iFitWidth > 0 && setup->iFitWidth < 32767)
						CP_XML_ATTR(L"fitToWidth", setup->iFitWidth);

					if (setup->iFitHeight > 0 && setup->iFitHeight < 32767)
						CP_XML_ATTR(L"fitToHeight", setup->iFitHeight);					

					CP_XML_ATTR(L"orientation", setup->fPortrait ? L"portrait" : L"landscape");

					if (setup->fUsePage)
						CP_XML_ATTR(L"useFirstPageNumber", true);
					
					CP_XML_ATTR(L"horizontalDpi", setup->iRes);
					CP_XML_ATTR(L"verticalDpi", setup->iVRes);					
				}
			}
		}
		if (m_Header || m_Footer)
		{
			CP_XML_NODE(L"headerFooter")
			{
				CP_XML_ATTR(L"differentFirst"	, false);
				CP_XML_ATTR(L"differentOddEven"	, false);

				if (m_Header)
				{
					CP_XML_NODE(L"oddHeader")
					{
						Header * h = dynamic_cast<Header*>(m_Header.get());
						CP_XML_CONTENT(STR::escape_ST_Xstring(h->ast.value()));
					}
				}
				if (m_Footer)
				{
					CP_XML_NODE(L"oddFooter")
					{
						Footer * f = dynamic_cast<Footer*>(m_Footer.get());
						CP_XML_CONTENT(STR::escape_ST_Xstring(f->ast.value()));
					}
				}
			}

		}
	}
	return 0;
}

} // namespace XLS

