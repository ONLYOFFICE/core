
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "DocumentGrid.h"


namespace OOX
{
	namespace Logic
	{

		DocumentGrid::DocumentGrid()
		{
		}


		DocumentGrid::~DocumentGrid()
		{
		}
	

		DocumentGrid::DocumentGrid(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DocumentGrid& DocumentGrid::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DocumentGrid::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			LinePitch = element.attribute("linePitch").value();
			CharSpace	=	element.attribute("charSpace").value();
			Type			= element.attribute("type").value();
		}


		const XML::XNode DocumentGrid::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX