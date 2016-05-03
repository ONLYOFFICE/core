
#include "PAGESETUP.h"

#include <Logic/Biff_records/Header.h>
#include <Logic/Biff_records/Footer.h>
#include <Logic/Biff_records/HCenter.h>
#include <Logic/Biff_records/VCenter.h>
#include <Logic/Biff_records/LeftMargin.h>
#include <Logic/Biff_records/RightMargin.h>
#include <Logic/Biff_records/TopMargin.h>
#include <Logic/Biff_records/BottomMargin.h>
#include <Logic/Biff_records/Pls.h>
#include <Logic/Biff_records/Continue.h>
#include <Logic/Biff_records/Setup.h>
#include <Logic/Biff_records/HeaderFooter.h> 

namespace XLS
{


PAGESETUP::PAGESETUP()
{
}


PAGESETUP::~PAGESETUP()
{
}

class Parenthesis_PAGESETUP_1: public ABNFParenthesis
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

		switch(type)
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
			
			case rt_HCenter:		proc.optional<HCenter>();			break;
			case rt_VCenter:		proc.optional<VCenter>();			break;
			
			case rt_BottomMargin:	proc.optional<BottomMargin>();		break;
			case rt_TopMargin:		proc.optional<TopMargin>();			break;
			case rt_LeftMargin:		proc.optional<LeftMargin>();		break;
			case rt_RightMargin:	proc.optional<RightMargin>();		break;
			case rt_Pls:			proc.optional<Pls>();				break;
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
				if (setup->numHdr.value())
				{
					CP_XML_ATTR(L"header", setup->numHdr);
					header = true;
				}

				if (setup->numFtr.value())
				{
					CP_XML_ATTR(L"footer", setup->numFtr);
					footer = true;
				}
			}
			for (std::list<XLS::BaseObjectPtr>::iterator it = elements_.begin(); it != elements_.end(); it++)
			{
				switch((*it)->get_type())
				{
				case typeTopMargin:
					if (!t)
					{
						TopMargin* top = dynamic_cast<TopMargin*>(it->get());
						CP_XML_ATTR(L"top", top->num);
						t = true;
					}break;
				case typeBottomMargin:
					if (!b)
					{
						BottomMargin* bottom = dynamic_cast<BottomMargin*>(it->get());
						CP_XML_ATTR(L"bottom", bottom->num);
						b= true;
					}break;
				case typeLeftMargin:
					{
						LeftMargin* left = dynamic_cast<LeftMargin*>(it->get());
						CP_XML_ATTR(L"left", left->num);
						l= true;
					}break;				
				case typeRightMargin:
					{
						RightMargin* right = dynamic_cast<RightMargin*>(it->get());
						CP_XML_ATTR(L"right", right->num);
						r= true;
					}break;		
				}
			}
			if (!header)	CP_XML_ATTR(L"header"	, 0.5);
			if (!footer)	CP_XML_ATTR(L"footer"	, 0.5);
			if (!t)			CP_XML_ATTR(L"top"		, 1);
			if (!b)			CP_XML_ATTR(L"bottom"	, 1);
			if (!l)			CP_XML_ATTR(L"left"		, 0.75);
			if (!r)			CP_XML_ATTR(L"right"	, 0.75);
		}

		CP_XML_NODE(L"pageSetup")
		{
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

