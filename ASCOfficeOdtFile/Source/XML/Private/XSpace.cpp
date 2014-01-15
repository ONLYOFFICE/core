
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XSpace.h"


namespace XML
{
	namespace Private
	{

		XSpace::XSpace()
			: m_space(false)
		{
		}


		XSpace::XSpace(const bool space)
			: m_space(space)
		{
		}


		const XSpace& XSpace::operator =(const bool rhs)
		{
			m_space = rhs;
			return *this;
		}


		void XSpace::setDefault()
		{
			m_space = false;
		}


		void XSpace::setPreserve()
		{
			m_space = true;
		}


		const bool XSpace::isDefault() const
		{
			return m_space == false;
		}


		const bool XSpace::isPreserve() const
		{
			return m_space == true;
		}

	} // namespace Private
} // namespace XML