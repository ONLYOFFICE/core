#include "LineStyle.h"

namespace PPTX
{
	namespace Logic
	{

		LineStyle::LineStyle()
		{
		}


		LineStyle::~LineStyle()
		{
		}
	

		LineStyle::LineStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const LineStyle& LineStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void LineStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;

			ln = element.element("ln");
			lnRef = element.element("lnRef");
			
			FillParentPointersForChilds();
		}


		const XML::XNode LineStyle::toXML() const
		{
			if(ln.is_init())
				return XML::XElement(m_namespace + m_name.get(), XML::Write(ln));
			return XML::XElement(m_namespace + m_name.get(), XML::Write(lnRef));
		}

		void LineStyle::FillParentPointersForChilds()
		{
			if(ln.is_init())
				ln->SetParentPointer(*this);
			if(lnRef.is_init())
				lnRef->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX