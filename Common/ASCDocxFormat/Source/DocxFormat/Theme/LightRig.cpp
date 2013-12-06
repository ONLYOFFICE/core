
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "LightRig.h"


namespace OOX
{
	namespace Theme
	{

		LightRig::LightRig()
		{
		}


		LightRig::~LightRig()
		{
		}


		LightRig::LightRig(const XML::XNode& node)
		{
			fromXML(node);
		}


		const LightRig& LightRig::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void LightRig::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_rig = element.attribute("rig").value();
			m_dir = element.attribute("dir").value();
			m_rot = element.element("rot");
		}


		const XML::XNode LightRig::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX