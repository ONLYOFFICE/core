
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ParagraphPropertyChange.h"

namespace OOX
{
	namespace Logic
	{
		ParagraphPropertyChange::ParagraphPropertyChange()
		{
		}

		ParagraphPropertyChange::~ParagraphPropertyChange()
		{
		}	

		ParagraphPropertyChange::ParagraphPropertyChange(const XML::XNode& node)
		{
			fromXML(node);
		}

		const ParagraphPropertyChange& ParagraphPropertyChange::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void ParagraphPropertyChange::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Id          = element.attribute("id").value();
			Author      = element.attribute("author").value();
			Date        = element.attribute("date").value();
		}

		const XML::XNode ParagraphPropertyChange::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX