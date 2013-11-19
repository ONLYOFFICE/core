#include "TcTxStyle.h"

namespace PPTX
{
	namespace Logic
	{
		TcTxStyle::TcTxStyle()
		{
		}

		TcTxStyle::~TcTxStyle()
		{
		}

		TcTxStyle::TcTxStyle(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TcTxStyle& TcTxStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TcTxStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			i = element.attribute("i").value();
			b = element.attribute("b").value();

			FontRef = element.element("fontRef");
			Color.GetColorFrom(element);

			FillParentPointersForChilds();
		}

		const XML::XNode TcTxStyle::toXML() const
		{
			return XML::XElement(ns.a + "tcTxStyle",
					XML::XAttribute("i", i) +
					XML::XAttribute("b", b) +
					XML::Write(FontRef) +
					XML::Write(Color)
				);
		}

		void TcTxStyle::FillParentPointersForChilds()
		{
			if(FontRef.is_init())
				FontRef->SetParentPointer(*this);
			Color.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX