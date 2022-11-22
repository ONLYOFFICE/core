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

#include "FORMATTING.h"
#include "TABLESTYLES.h"
#include "XFS.h"
#include "THEME.h"
#include "STYLES.h"

#include "../Biff_records/XF.h"
#include "../Biff_records/Font.h"
#include "../Biff_records/Format.h"
#include "../Biff_records/DXF.h"
#include "../Biff_records/Palette.h"
#include "../Biff_records/ClrtClient.h"
#include "../Biff_records/TableStyles.h"
#include "../Biff_records/Compat12.h"

#include "../Biff_records/Style.h"
#include "../Biff_records/StyleExt.h"
namespace XLS
{


FORMATTING::FORMATTING()
{
}


FORMATTING::~FORMATTING()
{
}


BaseObjectPtr FORMATTING::clone()
{
	return BaseObjectPtr(new FORMATTING(*this));
}


//FORMATTING = 1*510Font 8*218Format XFS *DXF STYLES [TABLESTYLES] [Palette] [ClrtClient]
const bool FORMATTING::loadContent(BinProcessor& proc)
{
	global_info = proc.getGlobalWorkbookInfo();

	if (proc.optional<Palette>())
	{
		m_Palette = elements_.back();
		elements_.pop_back();
	}

	int count = 0;
	count = proc.repeated<Font>(0, 510); // Wrong records sequence workaround (originally  at least one Font is mandatory)
	while(count > 0)
	{
		Font *font = dynamic_cast<Font *>(elements_.front().get());
		if ((font) && (font->correct))
		{
			global_info->m_arFonts.push_back(elements_.front());
		}
		elements_.pop_front();
		count--;
	}	
//---------------------------------------------------------------------------------------------------	
	count = proc.repeated<Format>(0, 218); // Originally: proc.repeated<Format>(8, 218);
	while(count > 0)
	{
		global_info->m_arNumFormats.insert(global_info->m_arNumFormats.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
//----------------------------------------------------------------------------------------------------	
	count = proc.repeated<Font>(0, 510); // Wrong records sequence workaround (originally Font follows by Format)
	int countFonts = global_info->m_arFonts.size();
	while(count > 0)
	{
		global_info->m_arFonts.insert(global_info->m_arFonts.begin() + countFonts, elements_.back());
		elements_.pop_back();
		count--;
	}	
//----------------------------------------------------------------------------------------------------	
	if (proc.mandatory<XFS>())
	{
		m_XFS = elements_.back();
		elements_.pop_back();
	}
	count = proc.repeated<DXF>(0, 0);
	while(count > 0)
	{
		m_arDXF.insert(m_arDXF.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	
	//----------------------------------------------------------------------------------------------------	
	if (proc.optional<STYLES>())
	{
		m_Styles = elements_.back();
		elements_.pop_back();
	}
//----------------------------------------------------------------------------------------------------	
	if (proc.optional<TABLESTYLES>())
	{
		m_TABLESTYLES = elements_.back();
		elements_.pop_back();
	}
		
	if (proc.optional<THEME>()) // не по стандарту - china_price.xls 
	{
		m_THEME = elements_.back();
		elements_.pop_back();
	}
	proc.optional<Compat12>();	//china_price.xls

	if (proc.optional<Palette>())
	{
		m_Palette = elements_.back();
		elements_.pop_back();
	}
	if (proc.optional<ClrtClient>())
	{
		m_ClrtClient= elements_.back();
		elements_.pop_back();
	}

	return true;
}
void FORMATTING::update_xfs()
{
	STYLES * st = dynamic_cast<STYLES*>(m_Styles.get());
	XFS * xfs	= dynamic_cast<XFS*>(m_XFS.get());

	if (!xfs) return;

	for (size_t i = 0; (st) && (i < st->m_arStyles.size()); i++)
	{
		XLS::Style		*style		= dynamic_cast<Style*>		(st->m_arStyles[i].first.get());
		XLS::StyleExt	*styleExt	= dynamic_cast<StyleExt*>	(st->m_arStyles[i].second.get());
			
		if (styleExt && style)
		{
			if (styleExt->fBuiltIn && styleExt->xfProps.cprops > 0)
			{
				bool bFound = false;
				for (size_t i = 0; i < xfs->m_arCellStyles.size(); i++)
				{
					XF* xf = dynamic_cast<XF*>(xfs->m_arCellStyles[i].get());

					if (xf->ind_xf == style->ixfe)
					{
						xf->xf_props = styleExt->xfProps.rgExt;
						bFound = true;
						break;
					}
				}
				//for (size_t i = 0; !bFound && i < xfs->m_arCellXFs.size(); i++) небывает совпадений
				//{
				//	XF* xf = dynamic_cast<XF*>(xfs->m_arCellXFs[i].get());
				//	if (xf->ind_xf == style->ixfe)
				//	{
				//		xf->cell.xf_props = styleExt->xfProps.rgExt;
				//		bFound = true;
				//		break;
				//	}
				//}
			}
		}
	}
	xfs->RegisterFillBorder();
}
void FORMATTING::concatinate(FORMATTING* ext)
{
	if (ext->m_XFS)
	{
		if (!m_XFS)
			m_XFS = ext->m_XFS;
		else
		{
			XFS * xf		= dynamic_cast<XFS*>(m_XFS.get());
			XFS * xf_ext	= dynamic_cast<XFS*>(ext->m_XFS.get());
		}
	}
	if (ext->m_Styles)
	{
		if (!m_Styles)
			m_Styles = ext->m_Styles;
		else
		{
			STYLES * st		= dynamic_cast<STYLES*>(m_Styles.get());
			STYLES * st_ext	= dynamic_cast<STYLES*>(ext->m_Styles.get());
			if (st && st_ext)
				st->elements_.insert(st->elements_.end(), st_ext->elements_.begin(), st_ext->elements_.end());

		}
	}
}

int FORMATTING::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		if (m_XFS)
		{
			m_XFS->serialize(stream);
		}
		if (m_Styles)
		{
			m_Styles->serialize(stream);
		}
		if (global_info->cellStyleDxfs_count > 0)
		{
			CP_XML_NODE(L"dxfs")
			{
				CP_XML_ATTR(L"count", global_info->cellStyleDxfs_count);

				for (size_t i = 0 ; i < m_arDXF.size(); i++)
				{
					m_arDXF[i]->serialize(CP_XML_STREAM());
				}

				for (size_t i = 0; i < global_info->arrUserDxfs.size(); i++)
				{
					CP_XML_STREAM() << global_info->arrUserDxfs[i];
				}
			}
		}
		if (m_TABLESTYLES)
			m_TABLESTYLES->serialize(stream);

		if (m_Palette)
		{
			CP_XML_NODE(L"colors")
			{
				m_Palette->serialize(CP_XML_STREAM());
			}
		}
	}

	return 0;
}


} // namespace XLS

