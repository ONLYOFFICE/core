
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XNamespace.h"
#include "Private/NullNamespace.h"


namespace XML
{

	XNamespace::XNamespace()
		:	base(new Private::NullNamespace())//,
			//Prefix(m_ptr->Prefix),
			//Name(m_ptr->Name)
			//Prefix(m_ptr->GetPrefix()),
			//Name(m_ptr->GetName())
	{
	}


	XNamespace::XNamespace(const std::string& prefix, const std::string& name)
		:	base(new Private::Namespace(prefix, name))//,
			//Prefix(new std::string(prefix)),
			//Name(new std::string(name))
			//Prefix(m_ptr->Prefix),
			//Name(m_ptr->Name)
			//Prefix(m_ptr->GetPrefix()),
			//Name(m_ptr->GetName())
	{
	}


	XNamespace::XNamespace(const XNamespace& ns)
		:	base(ns.m_ptr)//,
			//Prefix(ns.Prefix),
			//Name(ns.Name)
	{
	}


	const XNamespace& XNamespace::operator =(const XNamespace& rhs)
	{
		if (this != &rhs)
		{
			m_ptr = rhs.m_ptr;
			//Prefix = rhs.Prefix;
			//Name = rhs.Name;
			//Prefix = m_ptr->GetPrefix();
			//Name = m_ptr->GetName();
		}
		return *this;
	}


	const bool XNamespace::operator==(const XNamespace& rhs) const
	{
		return ((*m_ptr) == (*rhs.m_ptr));
	}


	const bool XNamespace::operator<(const XNamespace& rhs) const
	{
		return ((*m_ptr) < (*rhs.m_ptr));
	}


	const bool XNamespace::exist() const
	{
		return m_ptr->exist();
	}


	const std::string XNamespace::GetPrefix()const
	{
		return m_ptr->GetPrefix();
	}


	const std::string XNamespace::GetName()const
	{
		return m_ptr->GetName();
	}

} // namespace XML