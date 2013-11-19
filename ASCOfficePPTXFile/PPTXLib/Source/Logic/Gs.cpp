#include "Gs.h"

namespace PPTX
{
	namespace Logic
	{

		Gs::Gs()
		{
		}


		Gs::~Gs()
		{
		}
	

		Gs::Gs(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Gs& Gs::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Gs::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			pos = element.attribute("pos").value();
			color.GetColorFrom(element);

			FillParentPointersForChilds();
		}


		const XML::XNode Gs::toXML() const
		{
			return XML::XElement(ns.a + "gs",
					XML::XAttribute("pos", pos) +
					XML::Write(color)
				);
		}

		void Gs::FillParentPointersForChilds()
		{
			color.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX