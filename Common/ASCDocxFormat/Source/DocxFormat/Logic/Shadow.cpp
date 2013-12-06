
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Shadow.h"


namespace OOX
{
	namespace Logic
	{
			
		Shadow::Shadow()
		{
		}


		Shadow::~Shadow()
		{
		}


		Shadow::Shadow(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Shadow& Shadow::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Shadow::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			On		= element.attribute("on").value();
			Type	= element.attribute("type").value();
			Color	= element.attribute("color").value();
			Color2	= element.attribute("color2").value();
			Offset	= element.attribute("offset").value();
			Offset2	= element.attribute("offset2").value();
			Opacity	= element.attribute("opacity").value();
			Origin	= element.attribute("origin").value();
			Matrix	= element.attribute("matrix").value();
		}

		
		const XML::XNode Shadow::toXML() const
		{			
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX
