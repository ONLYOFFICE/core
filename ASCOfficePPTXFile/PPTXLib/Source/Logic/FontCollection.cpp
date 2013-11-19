#include "FontCollection.h"

namespace PPTX
{
	namespace Logic
	{

		FontCollection::FontCollection()
		{
		}


		FontCollection::~FontCollection()
		{
		}


		FontCollection::FontCollection(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FontCollection& FontCollection::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FontCollection::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;

			latin = element.element("latin");
			ea = element.element("ea");
			cs = element.element("cs");
			XML::Fill(Fonts, element, "font");

			FillParentPointersForChilds();
		}


		const XML::XNode FontCollection::toXML() const
		{
			return	XML::XElement(m_namespace + m_name.get(),
				XML::Write(latin) +
				XML::Write(ea) +
				XML::Write(cs) +
				XML::Write(Fonts)
			);
		}

		void FontCollection::FillParentPointersForChilds()
		{
			latin->SetParentPointer(*this);
			ea->SetParentPointer(*this);
			cs->SetParentPointer(*this);
			for(std::list<SupplementalFont>::iterator i = Fonts->begin(); i != Fonts->end(); i++)
				i->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX