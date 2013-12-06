
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "SectorProperty.h"
#include "NumFormat.h"

namespace OOX
{
	namespace Logic
	{
		SectorProperty::SectorProperty()
		{

		}

		SectorProperty::~SectorProperty()
		{

		}	

		SectorProperty::SectorProperty(const XML::XNode& node)
		{
			fromXML(node);
		}

		const SectorProperty& SectorProperty::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void SectorProperty::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			PageSize		=	element.element("pgSz");
			PageMargin		=	element.element("pgMar");
			LnNumType		=	element.element("lnNumType");
			Columns			=	element.element("cols");
			DocumentGrid	=	element.element("docGrid");
			TitlePage		=	element.element("titlePg").exist();
			Type			=	element.element("type").attribute("val").value();
			PageBorders		=	element.element("pgBorders");

			if (false == element.element("pgSz").exist())
			{
				if (element.element("pgBorders").element("pgSz").exist())
					PageSize	=	element.element("pgBorders").element("pgSz");
			}

			XML::Fill(Footers, element, "footerReference");
			XML::Fill(Headers, element, "headerReference");

			// TODO исправить, могут быть не всегда
			if (element.element("footnotePr").exist())
			{
				FootNoteProperty = element.element("footnotePr");
			}			

			if (element.element("endnotePr").exist())
			{
				EndNoteProperty = element.element("endnotePr");
			}			
		}

		const XML::XNode SectorProperty::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX