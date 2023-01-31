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
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>

#include <xml/simple_xml_writer.h>

#include "pptx_output_xml.h"


namespace cpdoccore {
namespace oox {

//---------------------------------------------------------------------------------------------------------
std::wstring pptx_xml_slide::name() const
{
    return name_;
}
std::wstring pptx_xml_slide::rId() const
{
    return rId_;
}

pptx_xml_slide_ptr pptx_xml_slide::create(std::wstring const & name,int id)
{
	const std::wstring rId = std::wstring(L"sId") + std::to_wstring(id);
    return boost::make_shared<pptx_xml_slide>(name, rId);
}

pptx_xml_slide::pptx_xml_slide(std::wstring const & name,std::wstring const & id)
{
	name_ = name;
	rId_ = id;
}

pptx_xml_slide::~pptx_xml_slide()
{
}
std::wostream & pptx_xml_slide::Data()
{
    return strmData_;
}
std::wostream & pptx_xml_slide::Background()
{
    return strmBackground_;
}
std::wostream & pptx_xml_slide::Timing()
{
    return strmTiming_;
}
rels & pptx_xml_slide::Rels()
{
    return rels_;
}

void pptx_xml_slide::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:sld")
        {
			CP_XML_ATTR(L"xmlns:a",		L"http://schemas.openxmlformats.org/drawingml/2006/main"); 
			CP_XML_ATTR(L"xmlns:p",		L"http://schemas.openxmlformats.org/presentationml/2006/main");
			CP_XML_ATTR(L"xmlns:r",		L"http://schemas.openxmlformats.org/officeDocument/2006/relationships"); 
			CP_XML_ATTR(L"xmlns:p14",	L"http://schemas.microsoft.com/office/powerpoint/2010/main"); 
			CP_XML_ATTR(L"xmlns:p15",	L"http://schemas.microsoft.com/office/powerpoint/2012/main"); 
			CP_XML_ATTR(L"xmlns:mc",	L"http://schemas.openxmlformats.org/markup-compatibility/2006");
           
			CP_XML_NODE(L"p:cSld")
            {
   				CP_XML_ATTR(L"name", name());   
				
				CP_XML_STREAM() << strmBackground_.str();

				CP_XML_NODE(L"p:spTree")
				{
					CP_XML_STREAM() << strmData_.str();
				}
            }
			CP_XML_STREAM() << strmTiming_.str();
			CP_XML_NODE(L"p:clrMapOvr")
			{
				CP_XML_NODE(L"a:masterClrMapping");
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
std::wstring pptx_xml_slideLayout::rId() const
{
    return rId_;
}

pptx_xml_slideLayout_ptr pptx_xml_slideLayout::create(int id)
{
	const std::wstring rId = std::wstring(L"lrId") + std::to_wstring(id);
    return boost::make_shared<pptx_xml_slideLayout>(rId);
}

pptx_xml_slideLayout::pptx_xml_slideLayout(std::wstring const & id)
{
	rId_ = id;
}

pptx_xml_slideLayout::~pptx_xml_slideLayout()
{
}
std::wostream & pptx_xml_slideLayout::Data()
{
    return strmData_;
}
rels & pptx_xml_slideLayout::Rels()
{
    return rels_;
}

void pptx_xml_slideLayout::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:sldLayout")
        {
			CP_XML_ATTR(L"xmlns:a",		L"http://schemas.openxmlformats.org/drawingml/2006/main"); 
			CP_XML_ATTR(L"xmlns:p",		L"http://schemas.openxmlformats.org/presentationml/2006/main");
			CP_XML_ATTR(L"xmlns:r",		L"http://schemas.openxmlformats.org/officeDocument/2006/relationships"); 
			CP_XML_ATTR(L"xmlns:p14",	L"http://schemas.microsoft.com/office/powerpoint/2010/main"); 
			CP_XML_ATTR(L"xmlns:p15",	L"http://schemas.microsoft.com/office/powerpoint/2012/main"); 
			CP_XML_ATTR(L"xmlns:mc",	L"http://schemas.openxmlformats.org/markup-compatibility/2006");
      
			//if (slideLayoutData_.str().length()<0)
			//	CP_XML_ATTR(L"type",L"cust");//---------------------------!!!!!!!!!!!!
			//else
			//	CP_XML_ATTR(L"type",L"cust");     
			
			CP_XML_NODE(L"p:cSld")
            {
				CP_XML_NODE(L"p:spTree")
				{
					CP_XML_STREAM() << strmData_.str();

//в slideLayoutData_
//contentPart (Content Part) §19.3.1.14
//cxnSp (Connection Shape) §19.3.1.19
//extLst (Extension List with Modification Flag) §19.3.1.20
//graphicFrame (Graphic Frame) §19.3.1.2
//grpSp (Group Shape) §19.3.1.22
//grpSpPr (Group Shape Properties) §19.3.1.23
//nvGrpSpPr (Non-Visual Properties for a Group Shape) §19.3.1.31
//pic (Picture) §19.3.1.37
//sp (Shape) §19.3.1.43

				}
            }
			CP_XML_NODE(L"p:clrMapOvr")
			{
				CP_XML_NODE(L"a:masterClrMapping");
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------
std::wstring pptx_xml_slideMaster::rId() const
{
    return rId_;
}

pptx_xml_slideMaster_ptr pptx_xml_slideMaster::create(int id)
{
	const std::wstring rId = std::wstring(L"smId") + std::to_wstring(id);
    return boost::make_shared<pptx_xml_slideMaster>(rId,id);
}

pptx_xml_slideMaster::pptx_xml_slideMaster(std::wstring const & rId, int id)
{
	rId_ = rId;
	id_ = id;
}

pptx_xml_slideMaster::~pptx_xml_slideMaster()
{
}
std::wostream & pptx_xml_slideMaster::Data()
{
    return strmData_;
}
std::wostream & pptx_xml_slideMaster::DataExtra()
{
    return strmDataExtra_;
}
std::wostream & pptx_xml_slideMaster::Background()
{
    return strmBackground_;
}
std::wstringstream & pptx_xml_slideMaster::Sizes()
{
    return strmSizes_;
}
rels & pptx_xml_slideMaster::Rels()
{
    return rels_;
}
void pptx_xml_slideMaster::add_theme(int id, const std::wstring & tId)
{
	rels_.add(relationship( tId, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme",
			std::wstring(L"../theme/theme")  + std::to_wstring(id) + L".xml"));
}

void pptx_xml_slideMaster::add_layout(int id, const std::wstring & rId, const unsigned int & uniqId)
{
	layoutsId_.push_back(std::pair<std::wstring, unsigned int>(rId, uniqId));

	rels_.add(relationship( rId,L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout",
			std::wstring(L"../slideLayouts/slideLayout")  + std::to_wstring(id) + L".xml"));
}

void pptx_xml_slideMaster::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:sldMaster")
        {
			CP_XML_ATTR(L"xmlns:a",		L"http://schemas.openxmlformats.org/drawingml/2006/main"); 
			CP_XML_ATTR(L"xmlns:p",		L"http://schemas.openxmlformats.org/presentationml/2006/main");
			CP_XML_ATTR(L"xmlns:r",		L"http://schemas.openxmlformats.org/officeDocument/2006/relationships"); 
			CP_XML_ATTR(L"xmlns:p14",	L"http://schemas.microsoft.com/office/powerpoint/2010/main"); 
			CP_XML_ATTR(L"xmlns:p15",	L"http://schemas.microsoft.com/office/powerpoint/2012/main"); 
			CP_XML_ATTR(L"xmlns:mc",	L"http://schemas.openxmlformats.org/markup-compatibility/2006");
           
			CP_XML_NODE(L"p:cSld")
            {
				CP_XML_STREAM() << strmBackground_.str();

				CP_XML_NODE(L"p:spTree")
				{
					CP_XML_STREAM() << strmData_.str();
				}
            }
			CP_XML_NODE(L"p:clrMap")
			{
				CP_XML_ATTR(L"folHlink",L"folHlink");
				CP_XML_ATTR(L"hlink",L"hlink");
				CP_XML_ATTR(L"accent6",L"accent6");
				CP_XML_ATTR(L"accent5",L"accent5");
				CP_XML_ATTR(L"accent4",L"accent4");
				CP_XML_ATTR(L"accent3",L"accent3");
				CP_XML_ATTR(L"accent2",L"accent2");
				CP_XML_ATTR(L"accent1",L"accent1");
				CP_XML_ATTR(L"tx2",L"dk2");
				CP_XML_ATTR(L"tx1",L"dk1");
				CP_XML_ATTR(L"bg2",L"lt2");
				CP_XML_ATTR(L"bg1",L"lt1");
			}
			CP_XML_NODE(L"p:sldLayoutIdLst")
			{
				for (size_t i = 0; i < layoutsId_.size(); i++)
				{
					CP_XML_NODE(L"p:sldLayoutId")
					{
						CP_XML_ATTR(L"r:id", layoutsId_[i].first);
						CP_XML_ATTR(L"id", layoutsId_[i].second);
					}
				}
			}
			CP_XML_STREAM() << strmDataExtra_.str();
			CP_XML_NODE(L"p:txStyles")
			{
				CP_XML_NODE(L"p:titleStyle");
				CP_XML_NODE(L"p:bodyStyle");
				CP_XML_NODE(L"p:otherStyle");
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------
pptx_xml_theme_ptr pptx_xml_theme::create(std::wstring const & name,int id)
{
    return boost::make_shared<pptx_xml_theme>(name,id);
}

void pptx_xml_theme::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"a:theme")
        {
			CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");   
			CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"xmlns:p", L"http://schemas.openxmlformats.org/presentationml/2006/main");
		
			CP_XML_ATTR(L"name", name_);
            CP_XML_NODE(L"a:themeElements")
            {
				CP_XML_NODE(L"a:clrScheme")
				{
					CP_XML_ATTR(L"name", name_);
					CP_XML_STREAM() << clrSchemeData_.str();
				}
 				CP_XML_NODE(L"a:fontScheme")
				{
					CP_XML_ATTR(L"name", name_);
					CP_XML_STREAM() << fontSchemeData_.str();
				}
				CP_XML_NODE(L"a:fmtScheme")
				{
					CP_XML_ATTR(L"name", name_);
					CP_XML_STREAM() << fmtSchemeData_.str();
				}
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
pptx_xml_authors_comments_ptr pptx_xml_authors_comments::create()
{
    return boost::make_shared<pptx_xml_authors_comments>();
}

std::pair<int,int> pptx_xml_authors_comments::add_or_find(std::wstring author)
{
	int id_author=0;
	int last_id=0;

	bool find = false;
	for (size_t i=0;i<list_.size();i++)
	{		
		if (author == list_[i].name)
		{
			id_author = i;
			last_id = list_[i].last_idx+1;
			list_[i].last_idx++;
			find=true;
			break;
		}
	}
	if (!find)
	{
		_author_elm elm = {author,0};
		list_.push_back(elm);
		id_author = list_.size()-1;
	}

   return std::pair<int,int>(id_author,last_id);
}

void pptx_xml_authors_comments::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:cmAuthorLst")
        {
			CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");   
			CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"xmlns:p", L"http://schemas.openxmlformats.org/presentationml/2006/main");
		
			int i=0;
			int size = list_.size();
			while(true)
			{
				if (i>=size)break;
				
				CP_XML_NODE(L"p:cmAuthor")
				{
					CP_XML_ATTR(L"id", i);
					CP_XML_ATTR(L"name", list_[i].name);
					CP_XML_ATTR(L"initials", L"");
					CP_XML_ATTR(L"lastIdx", list_[i].last_idx+1);
					CP_XML_ATTR(L"clrIdx", i);
				}
				
				i++;
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------
void pptx_xml_presentation::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:presentation")
        {
			CP_XML_ATTR(L"xmlns:p", L"http://schemas.openxmlformats.org/presentationml/2006/main");
			CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");

            CP_XML_NODE(L"p:sldMasterIdLst")
            {
                CP_XML_STREAM() << slideMastersData_.str();
            }       
			std::wstring notesMaster = slideNotesMastersData_.str();
			if (notesMaster.empty() == false)
			{
				CP_XML_NODE(L"p:notesMasterIdLst")
				{
					CP_XML_STREAM() << notesMaster;
				} 
			}
			CP_XML_NODE(L"p:sldIdLst")
            {
                CP_XML_STREAM() << slidesData_.str();
            }
			CP_XML_STREAM() << slidesProperties_.str();

			CP_XML_STREAM() << slidesNotesProperties_.str();

			CP_XML_NODE(L"p:defaultTextStyle")//??
			{
				CP_XML_NODE(L"a:defPPr");
			}
        }
	}
}
//---------------------------------------------------------------------------------------------------------
std::wstring pptx_xml_slideNotes::rId() const
{
    return rId_;
}

pptx_xml_slideNotes_ptr pptx_xml_slideNotes::create(int id)
{
	const std::wstring rId = std::wstring(L"nId") + std::to_wstring(id);
    return boost::make_shared<pptx_xml_slideNotes>(rId);
}

pptx_xml_slideNotes::pptx_xml_slideNotes(std::wstring const & id)
{
	rId_ = id;
}

pptx_xml_slideNotes::~pptx_xml_slideNotes()
{
}
std::wostream & pptx_xml_slideNotes::Data()
{
    return strmData_;
}
std::wostream & pptx_xml_slideNotes::Background()
{
    return strmBackground_;
}

rels & pptx_xml_slideNotes::Rels()
{
    return rels_;
}

void pptx_xml_slideNotes::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:notes")
        {
			CP_XML_ATTR(L"xmlns:a",		L"http://schemas.openxmlformats.org/drawingml/2006/main"); 
			CP_XML_ATTR(L"xmlns:p",		L"http://schemas.openxmlformats.org/presentationml/2006/main");
			CP_XML_ATTR(L"xmlns:r",		L"http://schemas.openxmlformats.org/officeDocument/2006/relationships"); 
			CP_XML_ATTR(L"xmlns:p14",	L"http://schemas.microsoft.com/office/powerpoint/2010/main"); 
			CP_XML_ATTR(L"xmlns:p15",	L"http://schemas.microsoft.com/office/powerpoint/2012/main"); 
			CP_XML_ATTR(L"xmlns:mc",	L"http://schemas.openxmlformats.org/markup-compatibility/2006");
           
			CP_XML_NODE(L"p:cSld")
            {
				CP_XML_STREAM() << strmBackground_.str();

				CP_XML_NODE(L"p:spTree")
				{
					CP_XML_STREAM() << strmData_.str();
				}
            }
			CP_XML_NODE(L"p:clrMapOvr")
			{
				CP_XML_NODE(L"a:masterClrMapping");
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
std::wstring pptx_xml_slideNotesMaster::rId() const
{
    return rId_;
}

pptx_xml_slideNotesMaster_ptr pptx_xml_slideNotesMaster::create()
{
	const std::wstring rId = std::wstring(L"nmId1");
    return boost::make_shared<pptx_xml_slideNotesMaster>(rId, 1);
}

pptx_xml_slideNotesMaster::pptx_xml_slideNotesMaster(std::wstring const & rId, int id)
{
	rId_ = rId;
	id_ = id;
}

pptx_xml_slideNotesMaster::~pptx_xml_slideNotesMaster()
{
}
std::wostream & pptx_xml_slideNotesMaster::Data()
{
    return strmData_;
}
std::wostream & pptx_xml_slideNotesMaster::Background()
{
    return strmBackground_;
}
std::wostream & pptx_xml_slideNotesMaster::Sizes()
{
    return strmSizes_;
}
rels & pptx_xml_slideNotesMaster::Rels()
{
    return rels_;
}
void pptx_xml_slideNotesMaster::add_theme(int id, const std::wstring & tId)
{
	rels_.add(relationship( tId, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme",
		std::wstring(L"../theme/theme")  + std::to_wstring(id) + L".xml"));
}

void pptx_xml_slideNotesMaster::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:notesMaster")
        {
			CP_XML_ATTR(L"xmlns:a",		L"http://schemas.openxmlformats.org/drawingml/2006/main"); 
			CP_XML_ATTR(L"xmlns:p",		L"http://schemas.openxmlformats.org/presentationml/2006/main");
			CP_XML_ATTR(L"xmlns:r",		L"http://schemas.openxmlformats.org/officeDocument/2006/relationships"); 
			CP_XML_ATTR(L"xmlns:p14",	L"http://schemas.microsoft.com/office/powerpoint/2010/main"); 
			CP_XML_ATTR(L"xmlns:p15",	L"http://schemas.microsoft.com/office/powerpoint/2012/main"); 
			CP_XML_ATTR(L"xmlns:mc",	L"http://schemas.openxmlformats.org/markup-compatibility/2006");
           
			CP_XML_NODE(L"p:cSld")
            {
				CP_XML_STREAM() << strmBackground_.str();

				CP_XML_NODE(L"p:spTree")
				{
					CP_XML_STREAM() << strmData_.str();
				}
            }
			CP_XML_NODE(L"p:clrMap")
			{
				CP_XML_ATTR(L"folHlink",L"folHlink");
				CP_XML_ATTR(L"hlink",L"hlink");
				CP_XML_ATTR(L"accent6",L"accent6");
				CP_XML_ATTR(L"accent5",L"accent5");
				CP_XML_ATTR(L"accent4",L"accent4");
				CP_XML_ATTR(L"accent3",L"accent5");
				CP_XML_ATTR(L"accent2",L"accent2");
				CP_XML_ATTR(L"accent1",L"accent1");
				CP_XML_ATTR(L"tx2",L"dk2");
				CP_XML_ATTR(L"tx1",L"dk1");
				CP_XML_ATTR(L"bg2",L"lt2");
				CP_XML_ATTR(L"bg1",L"lt1");
			}
			CP_XML_NODE(L"p:notesStyle")
			{
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
}
}
