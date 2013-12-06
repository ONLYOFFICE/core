
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FillToRect.h"


namespace OOX
{
	namespace Theme
	{

		FillToRect::FillToRect()
		{
		}
	

		FillToRect::~FillToRect()
		{
		}


		FillToRect::FillToRect(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FillToRect& FillToRect::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FillToRect::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_l	=	element.attribute("l").value();
			m_t	=	element.attribute("t").value();
			m_r	=	element.attribute("r").value();
			m_b	=	element.attribute("b").value();
		}


		const XML::XNode FillToRect::toXML() const
		{
		return XML::XElement();

		}

	} // namespace Theme
} // namespace OOX