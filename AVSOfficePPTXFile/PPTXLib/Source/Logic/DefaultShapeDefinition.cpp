#include "DefaultShapeDefinition.h"

namespace PPTX
{
	namespace Logic
	{

		DefaultShapeDefinition::DefaultShapeDefinition()
		{
		}


		DefaultShapeDefinition::~DefaultShapeDefinition()
		{
		}
	

		DefaultShapeDefinition::DefaultShapeDefinition(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DefaultShapeDefinition& DefaultShapeDefinition::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DefaultShapeDefinition::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;

			spPr = element.element("spPr");
			bodyPr = element.element("bodyPr");
			lstStyle = element.element("lstStyle");
			style = element.element("style");

			FillParentPointersForChilds();
		}


		const XML::XNode DefaultShapeDefinition::toXML() const
		{
			return XML::XElement(ns.a + m_name.get(),
					XML::Write(spPr) +
					XML::Write(bodyPr) +
					XML::Write(lstStyle) +
					XML::Write(style)
				);
		}

		void DefaultShapeDefinition::FillParentPointersForChilds()
		{
			spPr->SetParentPointer(*this);
			bodyPr->SetParentPointer(*this);
			lstStyle->SetParentPointer(*this);
			if(style.is_init())
				style->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX