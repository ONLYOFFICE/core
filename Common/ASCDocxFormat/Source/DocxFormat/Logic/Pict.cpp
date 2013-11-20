
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Pict.h"

namespace OOX
{
	namespace Logic
	{
		Pict::Pict()
		{

		}
		
		Pict::~Pict()
		{

		}	

		Pict::Pict(const XML::XNode& node)
		{
			fromXML(node);
		}
		
		void Pict::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			name				=	element.XName->Name;
			shape				=	element.element("shape");
			group				=	element.element("group");
			rect				=	element.element("rect");
			oval				=	element.element("oval");
			line				=	element.element("line");
			roundrect			=	element.element("roundrect");

			if (element.element("shapetype").exist())
			{
				shapetype		=	element.element("shapetype");
			}
			else
			{
				if (element.element("shape").exist())
					shapetype	=	element.element("shape");
			}
			
			DxaOrig				=	element.attribute("dxaOrig").value();
			DyaOrig				=	element.attribute("dyaOrig").value();
			OleObject			=	element.element("OLEObject");	
		}

		const XML::XNode Pict::toXML() const
		{
			return
				XML::XElement(ns.w + name.ToString(), 
					XML::XAttribute(ns.w + "dxaOrig", DxaOrig)	+
					XML::XAttribute(ns.w + "dyaOrig", DyaOrig)	+					
					XML::Write(shapetype)						+
					XML::Write(shape)							+
					XML::Write(rect)							+
					XML::Write(oval)							+
					XML::Write(line)							+
					XML::Write(roundrect)						+
					XML::Write(OleObject)
				);
		}

		const std::string Pict::toTxt() const
		{
			return "[pict]";
		}

		const bool Pict::hasPictures() const
		{
			return shape.is_init() && shape->imageData.is_init();
		}

		const bool Pict::isOle() const
		{
			return OleObject.is_init();
		}

	} // namespace Logic
} // namespace OOX
