
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "EffectLst.h"


namespace OOX
{
	namespace Theme
	{

		EffectLst::EffectLst()
		{
		}


		EffectLst::~EffectLst()
		{
		}
	

		EffectLst::EffectLst(const XML::XNode& node)
		{
			fromXML(node);
		}


		const EffectLst& EffectLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void EffectLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_outerShdw	=	element.element("outerShdw");
			m_scene3d		=	element.element("scene3d");
			m_sp3d			=	element.element("sp3d");
		}


		const XML::XNode EffectLst::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX