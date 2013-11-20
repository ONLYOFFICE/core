
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "HdrShapeDefaults.h"


namespace OOX
{
	namespace Settings
	{

		HdrShapeDefaults::HdrShapeDefaults()
		{
		}


		HdrShapeDefaults::~HdrShapeDefaults()
		{
		}


		HdrShapeDefaults::HdrShapeDefaults(const XML::XNode& node)
		{
			fromXML(node);
		}


		const HdrShapeDefaults& HdrShapeDefaults::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void HdrShapeDefaults::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_shapeDefault = element.element("shapedefaults");
		}


		const XML::XNode HdrShapeDefaults::toXML() const
		{
			return 
				XML::XElement(ns.m + "hdrShapeDefaults",
					XML::Write(m_shapeDefault)
				);
		}

	} // namespace Settings
} // namespace OOX