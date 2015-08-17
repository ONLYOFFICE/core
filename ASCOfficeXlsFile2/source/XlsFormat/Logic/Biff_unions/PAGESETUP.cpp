
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
#include <Logic/Biff_records/HeaderFooter.h> // Moved from WorksheetSubstream

#include <simple_xml_writer.h>

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
	if(!proc.mandatory<Header>())
	{
		return false;
	}
	proc.mandatory<Footer>();
	proc.mandatory<HCenter>();
	proc.mandatory<VCenter>();
	
	proc.optional<BottomMargin>();// OpenOffice Calc stored files workaround
	proc.optional<TopMargin>();// OpenOffice Calc stored files workaround
	proc.optional<LeftMargin>();
	proc.optional<RightMargin>();
	proc.optional<TopMargin>();
	proc.optional<BottomMargin>();
	
	proc.optional<Parenthesis_PAGESETUP_1>();

    Pls pls(proc.getParent());
    proc.optional(pls); // OpenOffice Calc stored files workaround ?????????
	
	if (proc.mandatory<Setup>())
	{
		m_Setup = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<HeaderFooter>())
	{
		m_HeaderFooter = elements_.back();
		elements_.pop_back();
	}


	return true;
}

int PAGESETUP::serialize(std::wostream & stream)
{
	Setup * setup = dynamic_cast<Setup*>(m_Setup.get());

	bool headerFooter = false;
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"pageMargins")
		{
			if (setup)
			{
				if (setup->numHdr.value())
				{
					CP_XML_ATTR(L"header", setup->numHdr);
					headerFooter = true;
				}

				if (setup->numFtr.value())
				{
					CP_XML_ATTR(L"footer", setup->numFtr);
					headerFooter = true;
				}
			}
			bool b = false, t = false, l = false, r = false ;
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
			if (headerFooter)
			{
				if (!t)CP_XML_ATTR(L"top", 1);
				if (!b)CP_XML_ATTR(L"bottom", 1);
				if (!l)CP_XML_ATTR(L"left", 0.75);
				if (!r)CP_XML_ATTR(L"right", 0.75);
			}
		}

		if (headerFooter)
		{		
			//CP_XML_NODE(L"headerFooter"){}
		}

		CP_XML_NODE(L"pageSetup")
		{
		}
	}
	return 0;
}

} // namespace XLS

