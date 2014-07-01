#include "precompiled_cpodf.h"


#include "oox_chart_axis.h"
#include <cpdoccore/xml/simple_xml_writer.h>
#include <boost/foreach.hpp>

#include "oox_title.h"
#include "oox_chart_shape.h"

namespace cpdoccore {
namespace oox {

_CP_PTR(oox_axis_content) oox_axis_content::create(int type)
{
    return boost::make_shared<oox_axis_content>(type);
}

void oox_axis_content::oox_serialize(std::wostream & _Wostream)
{
	if (id_ <0 )return;//not activate
 
	CP_XML_WRITER(_Wostream)
    {
		switch(type_)
		{
		case 1:
			CP_XML_NODE(L"c:catAx")
			{	
				oox_serialize_content(CP_XML_STREAM());
				CP_XML_NODE(L"c:auto")
				{
					CP_XML_ATTR(L"val", 1);
				}
				//CP_XML_NODE(L"c:lblAlgn")
				//{
				//	CP_XML_ATTR(L"val", "ctr");
				//}
				//CP_XML_NODE(L"c:lblOffset")
				//{
				//	CP_XML_ATTR(L"val",100);
				//}
				//CP_XML_NODE(L"c:noMultiLvlLbl")
				//{
				//	CP_XML_ATTR(L"val", 1);
				//}
			}
			break;
		case 2:
			CP_XML_NODE(L"c:valAx")
			{	
				oox_serialize_content(CP_XML_STREAM());
				//CP_XML_NODE(L"c:crossBetween")
				//{
				//	CP_XML_ATTR(L"val", L"between");
				//}
				//CP_XML_NODE(L"c:numFmt")
				//{
				//	CP_XML_ATTR(L"sourceLinked", 1);
				//	CP_XML_ATTR(L"formatCode", "General");
				//}
			}
			break;
		case 3:
			CP_XML_NODE(L"c:serAx")
			{	
				oox_serialize_content(CP_XML_STREAM());
			}
			break;
		case 4:
			CP_XML_NODE(L"c:dateAx")
			{	
				oox_serialize_content(CP_XML_STREAM());
				CP_XML_NODE(L"c:auto")
				{
					CP_XML_ATTR(L"val", 1);
				}
			}
			break;
		}
	}
}
void oox_axis_content::oox_serialize_content(std::wostream & _Wostream)
{
	//odf::_variant Val;
	_CP_OPT(bool) boolVal;
	
	oox_title title;
	title.set_content(content_.title_);//todo нужно задать понармальному layout
  
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:axId")
        {
			CP_XML_ATTR(L"val", id_);
		}
		CP_XML_NODE(L"c:delete")//visible item
		{
			CP_XML_ATTR(L"val", 0);
		}
		CP_XML_NODE(L"c:scaling")
		{
			CP_XML_NODE(L"c:orientation")
			{
				odf::GetProperty(content_.properties_,L"reverse-direction",boolVal);
				if ((boolVal == true) && (boolVal.get()==true))
				{
					CP_XML_ATTR(L"val", "maxMin");
				}
				else
				{
					CP_XML_ATTR(L"val", "minMax");//default
				}
			}
			odf::GetProperty(content_.properties_,L"logarithmic",boolVal);
			if ((boolVal == true) && (boolVal.get()==true))
			{
				CP_XML_NODE(L"c:logBase")
				{
					CP_XML_ATTR(L"val", "10");
				}
			}
		}
		//oox_serialize_ln(_Wostream,content_.graphic_properties_);

		oox_chart_shape shape;
		shape.set(content_.graphic_properties_,content_.fill_) ;
		shape.oox_serialize(_Wostream);
		BOOST_FOREACH(odf::chart::axis::grid  & g, content_.grids_)
		{
			_oox_fill fill_null;
			shape.set(g.graphic_properties_, fill_null);
			
			if (g.type_ == odf::chart::axis::grid::major)
			{
				CP_XML_NODE(L"c:majorGridlines")
				{
					shape.oox_serialize(CP_XML_STREAM());
				}
				odf::GetProperty(content_.properties_,L"display_label",boolVal);
				if ((boolVal == true) && (boolVal.get()==true))
				{
					CP_XML_NODE(L"c:majorTickMark")
					{
						CP_XML_ATTR(L"val", L"cross");//cross (Cross); in (Inside); none (None); out (Outside)
					}
				}
			}
			if (g.type_ == odf::chart::axis::grid::minor)
			{
				CP_XML_NODE(L"c:minorGridlines")
				{
					shape.oox_serialize(CP_XML_STREAM());
				}
				odf::GetProperty(content_.properties_,L"display_label",boolVal);
				if ((boolVal == true) && (boolVal.get()==true))
				{
					CP_XML_NODE(L"c:minorTickMark")
					{
						CP_XML_ATTR(L"val", L"cross");
					}
				}			
			}
		}
		BOOST_FOREACH(int const & ii, cross_id_)
		{
			CP_XML_NODE(L"c:crossAx")
			{
				CP_XML_ATTR(L"val", ii);
			}
		}
		CP_XML_NODE(L"c:axPos")
        {
			if (content_.dimension_ == L"x")
				CP_XML_ATTR(L"val",L"b");//  "b" | "l" |  "r" |  "t"// == bottom left right top
				
			if (content_.dimension_ == L"y")
				CP_XML_ATTR(L"val",L"l");//  "b" | "l" |  "r" |  "t"// == bottom left right top
		}
//////////////////////////////////////////////////////
		CP_XML_NODE(L"c:crosses")
        {
			CP_XML_ATTR(L"val", L"autoZero"); //""autoZero" | "max" | "min"
		}		
		odf::GetProperty(content_.properties_,L"display_label",boolVal);
		if ((boolVal == true) && (boolVal.get()==true))
		{
			CP_XML_NODE(L"c:tickLblPos")
			{
				CP_XML_ATTR(L"val",L"nextTo");//"high" | "low" |  "nextTo" |  "none"
			}
		}
		title.oox_serialize(_Wostream);
	}
}



}
}