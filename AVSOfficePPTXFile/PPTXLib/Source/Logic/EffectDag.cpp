#include "EffectDag.h"

namespace PPTX
{
	namespace Logic
	{
		EffectDag::EffectDag()
		{
		}

		EffectDag::~EffectDag()
		{
		}

		EffectDag::EffectDag(const XML::XNode& node)
		{
			fromXML(node);
		}

		const EffectDag& EffectDag::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void EffectDag::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName->Name;
			name = element.attribute("name").value();
			type = element.attribute("type").value();
			Effects->clear();
			XML::Fill(Effects, element);

			FillParentPointersForChilds();
		}

		const XML::XNode EffectDag::toXML() const
		{
			return XML::XElement(ns.a + m_name.get(),
					XML::XAttribute("name", name) +
					XML::XAttribute("type", type) +
					XML::Write(Effects)
				);
		}

		void EffectDag::FillParentPointersForChilds()
		{
			for(std::list<UniEffect>::iterator i = Effects->begin(); i != Effects->end(); i++)
				i->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX