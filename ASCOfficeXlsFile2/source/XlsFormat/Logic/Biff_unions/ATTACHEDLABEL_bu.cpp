
#include "ATTACHEDLABEL.h"
#include <Logic/Biff_records/Text.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/Pos.h>
#include <Logic/Biff_records/FontX.h>
#include <Logic/Biff_records/AlRuns.h>
#include <Logic/Biff_records/ObjectLink.h>
#include <Logic/Biff_records/DataLabExtContents.h>
#include <Logic/Biff_records/CrtLayout12.h>
#include <Logic/Biff_records/End.h>
#include <Logic/Biff_records/SeriesText.h>
#include <Logic/Biff_records/Font.h>

#include <Logic/Biff_unions/AI.h>
#include <Logic/Biff_unions/FRAME.h>
#include <Logic/Biff_unions/TEXTPROPS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>

#include <simple_xml_writer.h>
#include <utils.h>

namespace XLS
{


ATTACHEDLABEL::ATTACHEDLABEL()
{
	m_iLinkObject = -1;
}


ATTACHEDLABEL::~ATTACHEDLABEL()
{
}


BaseObjectPtr ATTACHEDLABEL::clone()
{
	return BaseObjectPtr(new ATTACHEDLABEL(*this));
}


/*
ATTACHEDLABEL = Text Begin Pos [FontX] [AlRuns] AI [FRAME] [ObjectLink] [DataLabExtContents] 
				[CrtLayout12] [TEXTPROPS] [CRTMLFRT] End
*/
const bool ATTACHEDLABEL::loadContent(BinProcessor& proc)
{
	pGlobalWorkbookInfoPtr = proc.getGlobalWorkbookInfo();

	if(!proc.mandatory<Text>())
	{
		return false;
	}
	proc.mandatory<Begin>();	elements_.pop_back();

	if (proc.optional<Pos>())
	{
		m_Pos = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<FontX>())
	{
		m_FontX = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<AlRuns>())
	{
		m_AlRuns = elements_.back();
		elements_.pop_back();
	}

	if (proc.mandatory<AI>())
	{
		m_AI = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<FRAME>())
	{
		m_FRAME = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<ObjectLink>())
	{
		m_ObjectLink = elements_.back();
		elements_.pop_back();

		ObjectLink *o_l = dynamic_cast<ObjectLink*>(m_ObjectLink.get());
		
		m_iLinkObject = o_l->wLinkObj;

	}
	
	if (proc.optional<DataLabExtContents>())
	{
		m_DataLabExtContents = elements_.back();
		elements_.pop_back();
	}
	
	proc.optional<CrtLayout12>();
	if (proc.optional<TEXTPROPS>())
	{
		m_TEXTPROPS = elements_.back();
		elements_.pop_back();
	}
	
	proc.optional<CRTMLFRT>();
	proc.mandatory<End>();			elements_.pop_back();

	return true;
}

int ATTACHEDLABEL::serialize(std::wostream & _stream)
{
	AI *ai	= dynamic_cast<AI *>( m_AI.get());
	
	if (ai == NULL) return 0;
	SeriesText * seriesText = dynamic_cast<SeriesText *>(ai->m_SeriesText.get());

	AlRuns *allRuns = dynamic_cast<AlRuns *>( m_AlRuns.get());
	int count_runs = allRuns->rgRuns.size();

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:tx")
		{
			CP_XML_NODE(L"c:rich")
			{
				//<a:bodyPr>
				//  <xsl:if test="$ATTACHEDLABEL/Text/@trot != 0">
				//    <xsl:attribute name="rot">
				//      <xsl:call-template name="angle_b2x">
				//        <xsl:with-param name="degrees" select="$ATTACHEDLABEL/Text/@trot"/>
				//      </xsl:call-template>
				//    </xsl:attribute>
				//  </xsl:if>
				//</a:bodyPr>
				CP_XML_NODE(L"a:p")
				{
					CP_XML_NODE(L"a:pPr")
					{
						FontX * font = dynamic_cast<FontX*>(m_FontX.get());
						if (font)
						{
							serialize_rPr (CP_XML_STREAM(),font->iFont, true);
						}
					}
					if (seriesText)//todoooo сделать вариант с DFTTEXT
					{	
						std::wstring & str_ = seriesText->stText.value();
						int str_size = str_.size();

						for (int i = 0 ; i < allRuns->rgRuns.size(); i++)
						{
							FormatRun *run = dynamic_cast<FormatRun*>(allRuns->rgRuns[i].get());
							if (run == NULL) continue;

							int end_string = str_size;

							if ( i < allRuns->rgRuns.size() - 1)
							{
								FormatRun *run_next = dynamic_cast<FormatRun*>(allRuns->rgRuns[i+1].get());
								if (run_next)
									end_string = run_next->ich;
							}

							CP_XML_NODE(L"a:r")
							{
								serialize_rPr(CP_XML_STREAM(), run->ifnt );

								CP_XML_NODE(L"a:t")
								{		
									if (run->ich > str_.length())
									{
										//ошибка
										run->ich = 0;
									}

									std::wstring str_part = str_.substr( run->ich, end_string - run->ich);

									CP_XML_STREAM() << xml::utils::replace_text_to_xml(str_part);
								}
							}
						}
					}
				}
			}	
		}
		Pos *pos = dynamic_cast<Pos *>(m_Pos.get());
		if (pos)pos->serialize(_stream);
	
		FRAME *frame = dynamic_cast<FRAME *>(m_FRAME.get());
		if (frame)frame->serialize(_stream);
	}
	return 0;
}

int ATTACHEDLABEL::serialize_rPr (std::wostream & _stream, int iFmt, bool defRPr)
{
	if (!pGlobalWorkbookInfoPtr)			return 0;
	if (!pGlobalWorkbookInfoPtr->m_arFonts) return 0;

	int sz = pGlobalWorkbookInfoPtr->m_arFonts->size();
	if (iFmt -1 > sz || iFmt < 1) return 0;

	Font * font = dynamic_cast<Font*>(pGlobalWorkbookInfoPtr->m_arFonts->at(iFmt-1).get());

	if (font)
	{
		if (defRPr)
		{
			CP_XML_WRITER(_stream)    
			{		
				CP_XML_NODE(L"a:defRPr")
				{
					font->serialize_properties(CP_XML_STREAM(), true);
				}
			}
		}		
		else
		{
			font->serialize_rPr(_stream);
		}
	}
	
	return 0;
}
} // namespace XLS

