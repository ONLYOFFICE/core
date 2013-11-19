#include "Stretch.h"

namespace PPTX
{
	namespace Logic
	{

		Stretch::Stretch()
		{
		}


		Stretch::~Stretch()
		{
		}


		Stretch::Stretch(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Stretch& Stretch::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Stretch::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			fillRect = element.element("fillRect");
			FillParentPointersForChilds();
		}


		const XML::XNode Stretch::toXML() const
		{
			 return XML::XElement(ns.a + "stretch",
					XML::Write(fillRect)
				);
		}

		void Stretch::FillParentPointersForChilds()
		{
			if(fillRect.is_init())
				fillRect->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX