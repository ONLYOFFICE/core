#include "SolidFill.h"

namespace PPTX
{
	namespace Logic
	{

		SolidFill::SolidFill()
		{
		}


		SolidFill::~SolidFill()
		{
		}


		SolidFill::SolidFill(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SolidFill& SolidFill::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SolidFill::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_namespace = element.XName.get().Ns;
			Color.GetColorFrom(element);
			FillParentPointersForChilds();
		}


		const XML::XNode SolidFill::toXML() const
		{
			 return XML::XElement(m_namespace + "solidFill",
					XML::Write(Color)
				 );
		}

		void SolidFill::Merge(SolidFill& fill)const
		{
			if(Color.is_init())
				fill.Color = Color;
		}

		void SolidFill::FillParentPointersForChilds()
		{
			Color.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX