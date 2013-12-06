
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Lock.h"


namespace OOX
{
	namespace Logic
	{
			
		Lock::Lock()
		{
		}


		Lock::~Lock()
		{
		}


		Lock::Lock(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Lock& Lock::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Lock::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Aspectratio	= element.attribute("aspectratio").value();
			Ext					= element.attribute("ext").value();
			Text				= element.attribute("text").value();
			ShapeType		= element.attribute("shapetype").value();
		}

		
		const XML::XNode Lock::toXML() const
		{			
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX
