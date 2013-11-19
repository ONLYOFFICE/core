#include "BlipFill.h"

namespace PPTX
{
	namespace Logic
	{

		BlipFill::BlipFill()
		{
		}


		BlipFill::~BlipFill()
		{
		}


		BlipFill::BlipFill(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BlipFill& BlipFill::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BlipFill::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_namespace = element.XName.get().Ns;

			dpi = element.attribute("dpi").value();
			rotWithShape = element.attribute("rotWithShape").value();

			blip = element.element("blip");
			srcRect = element.element("srcRect");
			tile = element.element("tile");
			stretch = element.element("stretch");

			FillParentPointersForChilds();
		}


		const XML::XNode BlipFill::toXML() const
		{
			 return XML::XElement(m_namespace + "blipFill",
					XML::XAttribute("dpi", dpi) +
					XML::XAttribute("rotWithShape", rotWithShape) +
					XML::Write(blip) +
					XML::Write(srcRect) +
					XML::Write(tile) +
					XML::Write(stretch)
				);
		}

		void BlipFill::FillParentPointersForChilds()
		{
			if(blip.is_init())
				blip->SetParentPointer(*this);
			if(srcRect.is_init())
				srcRect->SetParentPointer(*this);
			if(tile.is_init())
				tile->SetParentPointer(*this);
			if(stretch.is_init())
				stretch->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX