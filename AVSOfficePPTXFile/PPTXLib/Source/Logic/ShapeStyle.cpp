#include "ShapeStyle.h"

namespace PPTX
{
	namespace Logic
	{

		ShapeStyle::ShapeStyle()
		{
		}


		ShapeStyle::~ShapeStyle()
		{
		}
	

		ShapeStyle::ShapeStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ShapeStyle& ShapeStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ShapeStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_namespace = element.XName.get().Ns;

			lnRef = element.element("lnRef");
			fillRef = element.element("fillRef");
			effectRef = element.element("effectRef");
			fontRef = element.element("fontRef");

			FillParentPointersForChilds();
		}


		const XML::XNode ShapeStyle::toXML() const
		{
			return XML::XElement(m_namespace + "style",
					XML::Write(lnRef) +
					XML::Write(fillRef) +
					XML::Write(effectRef) +
					XML::Write(fontRef)
				);
		}

		void ShapeStyle::FillParentPointersForChilds()
		{
			lnRef->SetParentPointer(*this);
			fillRef->SetParentPointer(*this);
			effectRef->SetParentPointer(*this);
			fontRef->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX