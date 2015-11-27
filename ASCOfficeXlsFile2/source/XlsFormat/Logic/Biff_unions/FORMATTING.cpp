
#include "FORMATTING.h"
#include <Logic/Biff_records/Font.h>
#include <Logic/Biff_records/Format.h>
#include <Logic/Biff_unions/XFS.h>
#include <Logic/Biff_records/DXF.h>
#include <Logic/Biff_unions/STYLES.h>
#include <Logic/Biff_records/Palette.h>
#include <Logic/Biff_records/ClrtClient.h>
#include <Logic/Biff_records/TableStyles.h>
#include <Logic/Biff_unions/TABLESTYLES.h>

#include <simple_xml_writer.h>

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
	proc.repeated<DXF>(0, 0);

	//----------------------------------------------------------------------------------------------------	
	if (proc.optional<STYLES>())
	{
		m_Styles = elements_.back();
		elements_.pop_back();
	}
//----------------------------------------------------------------------------------------------------	
	proc.optional<TABLESTYLES>();
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

