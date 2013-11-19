#include "PartTitle.h"


namespace PPTX
{
	namespace Logic
	{

		PartTitle::PartTitle()
		{
		}


		PartTitle::~PartTitle()
		{
		}


		PartTitle::PartTitle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const PartTitle& PartTitle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void PartTitle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_title = element.text();
		}


		const XML::XNode PartTitle::toXML() const
		{
			return 	XML::XElement(ns.vt + "lpstr",
						XML::XText(m_title)
					);

		}

	} // namespace Logic
} // namespace PPTX