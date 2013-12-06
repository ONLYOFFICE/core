
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Point2D.h"


namespace OOX
{
	namespace Logic
	{

		Point2D::Point2D()
		{
		}


		Point2D::~Point2D()
		{
		}


		Point2D::Point2D(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Point2D& Point2D::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Point2D::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			name = element.XName->Name;
			x = element.attribute("x").value();
			y = element.attribute("y").value();
		}


		const XML::XNode Point2D::toXML() const
		{
		return XML::XElement();
		}
	} // namespace Logic
} // namespace OOX