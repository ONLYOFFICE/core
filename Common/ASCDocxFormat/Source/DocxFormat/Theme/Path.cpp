
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Path.h"

namespace OOX
{
	namespace Theme
	{
		Path::Path()
		{
		}

		Path::~Path()
		{
		}

		Path::Path(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Path& Path::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Path::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_path				=	element.attribute("path").value();

			if (element.element("fillToRect").exist())
				m_fillToRect	=	element.element("fillToRect");
		}

		const XML::XNode Path::toXML() const
		{
			return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX