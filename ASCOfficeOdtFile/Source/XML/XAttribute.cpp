
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XAttribute.h"
#include "Private/NullAttribute.h"
#include "Private/NullXAttribute.h"


namespace XML
{

	XAttribute::XAttribute()
		: base(0)
	{
	}


	XAttribute::XAttribute(Private::NullAttribute)
		: base(new Private::NullAttribute)
	{
	}


	const bool XAttribute::exist() const
	{
		return m_ptr->exist();
	}


	const Private::XString XAttribute::value() const
	{
		return m_ptr->value();
	}


	const Private::XString XAttribute::get_value_or(const std::string& def)const
	{
		if(exist())
			return value();
		return Private::XString(def);
	}


	const bool XAttribute::operator ==(const XAttribute& rhs) const
	{
		return ((*m_ptr) == (*rhs.m_ptr));
	}


	const bool XAttribute::operator <(const XAttribute& rhs) const
	{
		return ((*m_ptr) < (*rhs.m_ptr));
	}

} // namespace XML