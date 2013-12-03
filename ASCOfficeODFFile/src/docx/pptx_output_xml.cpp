#include "precompiled_cpodf.h"

#include "pptx_output_xml.h"
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {


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
	const std::wstring rId = std::wstring(L"sId") + boost::lexical_cast<std::wstring>(id);
    return boost::make_shared<pptx_xml_slide>(name,rId);
}

pptx_xml_slide::pptx_xml_slide(std::wstring const & name,std::wstring const & id)
{
	name_ = name;
	rId_ = id;
}

pptx_xml_slide::~pptx_xml_slide()
{
}
std::wostream & pptx_xml_slide::slideData()
{
    return slideData_;
}
rels & pptx_xml_slide::slideRels()
{
    return rels_;
}

void pptx_xml_slide::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:sld")
        {
			CP_XML_ATTR(L"xmlns:p", L"http://schemas.openxmlformats.org/presentationml/2006/main");        
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");        

            CP_XML_NODE(L"p:cSld")
            {
   				CP_XML_ATTR(L"name", name());   
				CP_XML_NODE(L"p:spTree")
				{
					CP_XML_STREAM() << slideData_.str();
				}
            }
			CP_XML_NODE(L"p:clrMapOvr")
			{
				CP_XML_NODE(L"a:masterClrMapping");
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::wstring pptx_xml_slideLayout::rId() const
{
    return rId_;
}

pptx_xml_slideLayout_ptr pptx_xml_slideLayout::create(int id)
{
	const std::wstring rId = std::wstring(L"lrId") + boost::lexical_cast<std::wstring>(id);
    return boost::make_shared<pptx_xml_slideLayout>(rId);
}

pptx_xml_slideLayout::pptx_xml_slideLayout(std::wstring const & id)
{
	rId_ = id;
}

pptx_xml_slideLayout::~pptx_xml_slideLayout()
{
}
std::wostream & pptx_xml_slideLayout::slideLayoutData()
{
    return slideLayoutData_;
}
rels & pptx_xml_slideLayout::slideLayoutRels()
{
    return rels_;
}

void pptx_xml_slideLayout::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:sldLayout")
        {
			if (slideLayoutData_.str().length()<0)
				CP_XML_ATTR(L"type",L"cust");//---------------------------!!!!!!!!!!!!
			else
				CP_XML_ATTR(L"type",L"cust");
			CP_XML_ATTR(L"xmlns:p", L"http://schemas.openxmlformats.org/presentationml/2006/main");        
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");        

            CP_XML_NODE(L"p:cSld")
            {
				CP_XML_NODE(L"p:spTree")
				{
					CP_XML_STREAM() << slideLayoutData_.str();

//â slideLayoutData_
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::wstring pptx_xml_slideMaster::rId() const
{
    return rId_;
}

pptx_xml_slideMaster_ptr pptx_xml_slideMaster::create(int id)
{
	const std::wstring rId = std::wstring(L"smId") + boost::lexical_cast<std::wstring>(id);
    return boost::make_shared<pptx_xml_slideMaster>(rId);
}

pptx_xml_slideMaster::pptx_xml_slideMaster(std::wstring const & id)
{
	rId_ = id;
}

pptx_xml_slideMaster::~pptx_xml_slideMaster()
{
}
std::wostream & pptx_xml_slideMaster::slideMasterData()
{
    return slideMasterData_;
}
rels & pptx_xml_slideMaster::slideMasterRels()
{
    return rels_;
}
void pptx_xml_slideMaster::add_theme(int id, const std::wstring & tId)
{
	rels_.add(relationship( tId, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme",
			std::wstring(L"../theme/theme")  + boost::lexical_cast<std::wstring>(id) + L".xml"));
}

void pptx_xml_slideMaster::add_layout(int id, const std::wstring & rId)
{
	layoutsId_.push_back(rId);

	rels_.add(relationship( rId,L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout",
			std::wstring(L"../slideLayouts/slideLayout")  + boost::lexical_cast<std::wstring>(id) + L".xml"));
}

static std::wstring strSlideMasterEmpty = L"<p:spTree><p:nvGrpSpPr><p:cNvPr name=\"\" id=\"1\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr><p:grpSpPr>\
<a:xfrm><a:off y=\"0\" x=\"0\"/><a:ext cy=\"0\" cx=\"0\"/><a:chOff y=\"0\" x=\"0\"/><a:chExt cy=\"0\" cx=\"0\"/></a:xfrm></p:grpSpPr></p:spTree>";

void pptx_xml_slideMaster::write_to(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:sldMaster")
        {
			CP_XML_ATTR(L"xmlns:p", L"http://schemas.openxmlformats.org/presentationml/2006/main");        
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");        

            CP_XML_NODE(L"p:cSld")
            {
				CP_XML_NODE(L"p:bg")
				{
					CP_XML_NODE(L"p:bgPr")
					{
						CP_XML_NODE(L"a:noFill");
						CP_XML_NODE(L"a:effectLst");
					}
				}
				if (slideMasterData_.str().length()>0)
				{
					CP_XML_STREAM() << slideMasterData_.str();
				}
				else
				{
					CP_XML_STREAM() << strSlideMasterEmpty;
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
			long count=0;
			CP_XML_NODE(L"p:sldLayoutIdLst")
			{
				BOOST_FOREACH(const std::wstring & q, layoutsId_)
				{
					CP_XML_NODE(L"p:sldLayoutId")
					{
						CP_XML_ATTR(L"r:id",q);
						CP_XML_ATTR(L"id",0x80000000 + (++count));
					}
				}
				count++;
			}
			CP_XML_NODE(L"p:txStyles")
			{
				CP_XML_NODE(L"p:titleStyle");
				CP_XML_NODE(L"p:bodyStyle");
				CP_XML_NODE(L"p:otherStyle");
			}
		}
	}
}

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
			CP_XML_NODE(L"p:sldIdLst")
            {
                CP_XML_STREAM() << slidesData_.str();
            }
			CP_XML_STREAM() << slidesProperties_.str();

			CP_XML_STREAM() << notesSlidesSize_.str();

			CP_XML_NODE(L"p:defaultTextStyle")
			{
				CP_XML_NODE(L"a:defPPr");
				//CP_XML_NODE(L"a:lvl1pPr");
				//CP_XML_NODE(L"a:lvl2pPr");
				//CP_XML_NODE(L"a:lvl3pPr");
				//CP_XML_NODE(L"a:lvl4pPr");
				//CP_XML_NODE(L"a:lvl5pPr");
				//CP_XML_NODE(L"a:lvl6pPr");
				//CP_XML_NODE(L"a:lvl7pPr");
				//CP_XML_NODE(L"a:lvl8pPr");
				//CP_XML_NODE(L"a:lvl9pPr");
			}
        }
	}
}
}
}
