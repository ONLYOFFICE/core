
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Sp3d.h"


namespace OOX
{
	namespace Theme
	{

		Sp3d::Sp3d()
		{
		}


		Sp3d::~Sp3d()
		{
		}
	

		Sp3d::Sp3d(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Sp3d& Sp3d::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Sp3d::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_bevelT = element.element("bevelT");
		}


		const XML::XNode Sp3d::toXML() const
		{
			return 
				XML::XElement(ns.a + "sp3d", 
					XML::Write(m_bevelT)
				);
		}

	} // namespace Theme
} // namespace OOX