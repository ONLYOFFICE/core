
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "LineStyle.h"

namespace OOX
{
	namespace Logic
	{			
		LineStyle::LineStyle()
		{

		}

		LineStyle::~LineStyle()
		{

		}

		LineStyle::LineStyle(const XML::XNode& node)
		{
			fromXML(node);
		}

		const LineStyle& LineStyle::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void LineStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element.attribute("stroked").exist())
				stroked					=	element.attribute("stroked").value();

			if (element.attribute("strokecolor").exist())
				strokeColor				=	element.attribute("strokecolor").value();

			if (element.attribute("strokeweight").exist())
				strokeWeight			=	element.attribute("strokeweight").value();

			const XML::XElement strokeXML = element.element("stroke");
			if (strokeXML.is_init())
			{
				if (strokeXML.attribute("linestyle").exist())
					lineStyle			=	strokeXML.attribute ("linestyle").value();
				if (strokeXML.attribute("dashstyle").exist())
					dashStyle			=	strokeXML.attribute ("dashstyle").value();
				if (strokeXML.attribute("opacity").exist())
					opacity				=	strokeXML.attribute ("opacity").value();
				if (strokeXML.attribute("endcap").exist())
					endcap				=	strokeXML.attribute ("endcap").value();
				if (strokeXML.attribute("startarrow").exist())
					startarrow			=	strokeXML.attribute ("startarrow").value();
				if (strokeXML.attribute("endarrow").exist())
					endarrow			=	strokeXML.attribute ("endarrow").value();			
				if (strokeXML.attribute("startarrowwidth").exist())
					startarrowwidth		=	strokeXML.attribute ("startarrowwidth").value();
				if (strokeXML.attribute("startarrowlength").exist())
					startarrowlength	=	strokeXML.attribute ("startarrowlength").value();
				if (strokeXML.attribute("endarrowwidth").exist())
					endarrowwidth		=	strokeXML.attribute ("endarrowwidth").value();
				if (strokeXML.attribute("endarrowlength").exist())
					endarrowlength		=	strokeXML.attribute ("endarrowlength").value();
			}	

			if (element.attribute("color").exist())
				strokeColor				=	element.attribute("color").value();			
			if (element.attribute("opacity").exist())
				strokeColor			=	element.attribute("opacity").value();	
			if (element.attribute("weight").exist())
				strokeWeight			=	element.attribute("weight").value();	
			if (element.attribute("endcap").exist())
				endcap					=	element.attribute("endcap").value();	
			if (element.attribute("startarrow").exist())
				startarrow				=	element.attribute ("startarrow").value();
			if (element.attribute("endarrow").exist())
				endarrow				=	element.attribute ("endarrow").value();
			if (element.attribute("startarrowwidth").exist())
				startarrowwidth			=	element.attribute ("startarrowwidth").value();
			if (element.attribute("startarrowlength").exist())
				startarrowlength		=	element.attribute ("startarrowlength").value();
			if (element.attribute("endarrowwidth").exist())
				endarrowwidth			=	element.attribute ("endarrowwidth").value();
			if (element.attribute("endarrowlength").exist())
				endarrowlength			=	element.attribute ("endarrowlength").value();
		}

		const XML::XNode LineStyle::toXML() const
		{			
			return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX
