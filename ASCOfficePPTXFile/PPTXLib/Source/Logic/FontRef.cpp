#include "FontRef.h"

namespace PPTX
{
	namespace Logic
	{

		FontRef::FontRef()
		{
		}


		FontRef::~FontRef()
		{
		}


		FontRef::FontRef(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FontRef& FontRef::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FontRef::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;

			idx = element.attribute("idx").value();
			Color.GetColorFrom(element);

			FillParentPointersForChilds();
		}


		const XML::XNode FontRef::toXML() const
		{
			return XML::XElement(m_namespace + m_name.get(),
						XML::XAttribute("idx", idx) +
						XML::Write(Color)
					);
		}

		void FontRef::FillParentPointersForChilds()
		{
			Color.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX