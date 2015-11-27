
#include "LD.h"
#include <Logic/Biff_records/Legend.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/Pos.h>
#include <Logic/Biff_unions/ATTACHEDLABEL.h>
#include <Logic/Biff_unions/FRAME.h>
#include <Logic/Biff_records/CrtLayout12.h>
#include <Logic/Biff_records/FontX.h>
#include <Logic/Biff_unions/TEXTPROPS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>
#include <Logic/Biff_records/End.h>

#include <simple_xml_writer.h>

namespace XLS
{


LD::LD()
{
}


LD::~LD()
{
}


BaseObjectPtr LD::clone()
{
	return BaseObjectPtr(new LD(*this));
}


// LD = Legend Begin Pos ATTACHEDLABEL [FRAME] [CrtLayout12] [TEXTPROPS] [CRTMLFRT] CRTMLFRT
const bool LD::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Legend>())
	{
		return false;
	}
	proc.mandatory<Begin>();			elements_.pop_back();
	
	//proc.mandatory<Pos>();

	// fix
	if (proc.optional<Pos>())
	{
		m_Pos = elements_.back();
		elements_.pop_back();
	}

	proc.mandatory<ATTACHEDLABEL>();

	m_ATTACHEDLABEL = elements_.back();
	elements_.pop_back();

	if (proc.optional<FRAME>())
	{
		m_FRAME = elements_.back();
		elements_.pop_back();
	}
	proc.optional<CrtLayout12>();
	proc.optional<TEXTPROPS>();
	proc.optional<CRTMLFRT>();
	proc.mandatory<End>();				elements_.pop_back();
	return true;
}

int LD::serialize (std::wostream & _stream)
{
	ATTACHEDLABEL * label = dynamic_cast<ATTACHEDLABEL*>(m_ATTACHEDLABEL.get());

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:legend")
		{
			//<c:legendPos val="r"/>
			CP_XML_NODE(L"c:legendEntry")
			{
				CP_XML_NODE(L"c:txPr")
				{
					CP_XML_NODE(L"a:bodyPr");
					CP_XML_NODE(L"a:lstStyle");
					CP_XML_NODE(L"a:p")
					{
						CP_XML_NODE(L"a:pPr")
						{
							if (label)
							{
								FontX * font = dynamic_cast<FontX*>(label->m_FontX.get());
								if (font)
								{
									label->serialize_rPr(CP_XML_STREAM(), font->iFont, true);
								}
								else
								{
									//default ???
								}
							}
						}
					}
				}
			}

			if (m_Pos)
				m_Pos->serialize(CP_XML_STREAM());

			//<c:overlay val="0"/>
		}
	}
	return 0;
}

} // namespace XLS

