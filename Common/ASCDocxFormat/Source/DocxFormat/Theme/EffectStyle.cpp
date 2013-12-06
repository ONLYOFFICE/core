
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "EffectStyle.h"


namespace OOX
{
	namespace Theme
	{

		EffectStyle::EffectStyle()
		{
		}


		EffectStyle::~EffectStyle()
		{
		}
	

		EffectStyle::EffectStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const EffectStyle& EffectStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void EffectStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_effestLst	=	element.element("effectLst");
			m_scene3d		=	element.element("scene3d");
			m_sp3d			=	element.element("sp3d");
		}


		const XML::XNode EffectStyle::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX