#include "precompiled_xls.h"
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
#include <Logic/Biff_unions/TABLESTYLES.h>

#include <simple_xml_writer.h>

namespace XLS
{;


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
	//todooo разобраться - ЗАЧЕМ 2 раза читаются фонты
	int count = 0;
	count = proc.repeated<Font>(0, 510); // Wrong records sequence workaround (originally  at least one Font is mandatory)
	while(count > 0)
	{
		m_Fonts.insert(m_Fonts.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	
//---------------------------------------------------------------------------------------------------	
	count = proc.repeated<Format>(0, 218); // Originally: proc.repeated<Format>(8, 218);
	while(count > 0)
	{
		m_Formats.insert(m_Formats.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
//----------------------------------------------------------------------------------------------------	
	count = proc.repeated<Font>(0, 510); // Wrong records sequence workaround (originally Font follows by Format)
	while(count > 0)
	{
		m_Fonts.insert(m_Fonts.begin(), elements_.back());
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

int FORMATTING::serialize(std::wostream & stream)
{
    CP_XML_WRITER(stream)    
    {
		if (m_Fonts.size() > 0)
		{
			CP_XML_NODE(L"fonts")
			{
				CP_XML_ATTR(L"count", m_Fonts.size());
				for (long i = 0 ; i < m_Fonts.size(); i++)
				{
					m_Fonts[i]->serialize(CP_XML_STREAM());
				}
			}
		}
		if (m_XFS)
		{
			m_XFS->serialize(stream);
		}
		if (m_Styles)
		{
			m_Styles->serialize(stream);
		}
	}

    //cpdoccore::oox::xlsx_serialize(_Wostream, numFmts_);
    //cpdoccore::oox::xlsx_serialize(_Wostream, fonts_);
    //cpdoccore::oox::xlsx_serialize(_Wostream, fills_);
    //cpdoccore::oox::xlsx_serialize(_Wostream, borders_);

    //
    //xlsx_serialize_xf(_Wostream, cellStyleXfs_, L"cellStyleXfs");
    //xlsx_serialize_xf(_Wostream, cellXfs_, L"cellXfs");
    //cellStyles_.xlsx_serialize(_Wostream);

	return 0;
}


} // namespace XLS

