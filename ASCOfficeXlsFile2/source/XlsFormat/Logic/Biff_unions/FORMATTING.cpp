
#include "FORMATTING.h"
#include <Logic/Biff_records/Font.h>
#include <Logic/Biff_records/Format.h>
#include <Logic/Biff_records/DXF.h>
#include <Logic/Biff_unions/STYLES.h>
#include <Logic/Biff_records/Palette.h>
#include <Logic/Biff_records/ClrtClient.h>
#include <Logic/Biff_records/TableStyles.h>
#include <Logic/Biff_records/Compat12.h>
#include <Logic/Biff_unions/TABLESTYLES.h>
#include <Logic/Biff_unions/XFS.h>
#include <Logic/Biff_unions/THEME.h>

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

	int count = 0;
	count = proc.repeated<Font>(0, 510); // Wrong records sequence workaround (originally  at least one Font is mandatory)
	while(count > 0)
	{
		m_arFonts.push_back(elements_.front());
		elements_.pop_front();
		count--;
	}	
//---------------------------------------------------------------------------------------------------	
	count = proc.repeated<Format>(0, 218); // Originally: proc.repeated<Format>(8, 218);
	while(count > 0)
	{
		m_arFormats.insert(m_arFormats.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
//----------------------------------------------------------------------------------------------------	
	count = proc.repeated<Font>(0, 510); // Wrong records sequence workaround (originally Font follows by Format)
	int countFonts = m_arFonts.size();
	while(count > 0)
	{
		m_arFonts.insert(m_arFonts.begin()+countFonts, elements_.back());
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

	global_info->cellStyleDxfs_count = m_arDXF.size(); // + будут юзерские
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

int FORMATTING::serialize1(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		if (m_arFormats.size() > 0)
		{
			CP_XML_NODE(L"numFmts")
			{
				CP_XML_ATTR(L"count", m_arFormats.size());
                for (int i = 0 ; i < m_arFormats.size(); i++)
				{
					m_arFormats[i]->serialize(CP_XML_STREAM());
				}
			}
		}
		if (m_arFonts.size() > 0)
		{
			CP_XML_NODE(L"fonts")
			{
				CP_XML_ATTR(L"count", m_arFonts.size());
                for (int i = 0 ; i < m_arFonts.size(); i++)
				{
					Font * font = dynamic_cast<Font*>(m_arFonts[i].get());
					std::map<int, FillInfoExt>::iterator it = global_info->fonts_color_ext.find(i);
					if (font && (it!=global_info->fonts_color_ext.end()))
					{					
						font->set_color_ext(it->second);
					}
					m_arFonts[i]->serialize(CP_XML_STREAM());
				}
			}
		}
   }
	return 0;
}

int FORMATTING::serialize2(std::wostream & stream)
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

				for (int i = 0 ; i < m_arDXF.size(); i++)
				{
					m_arDXF[i]->serialize(CP_XML_STREAM());
				}

				CP_XML_STREAM() << global_info->users_Dxfs_stream.str();
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

