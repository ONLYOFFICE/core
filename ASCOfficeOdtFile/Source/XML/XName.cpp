
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XName.h"
#include <algorithm>
#include "Encoding.h"
#include "ToString.h"


namespace XML
{

	XName::XName()
	{
	}


	XName::XName(const char* name)
		:	Name(name)
	{
	}


	XName::XName(const std::string& name)
		: Name(name)
	{
	}


	XName::XName(const XNamespace& ns, const std::string& name)
		:	Ns(ns),
			Name(name)
	{
	}


	const bool XName::Equal(const XName& rhs) const
	{
		if (rhs.Ns.is_init() && Ns.is_init())
			return	((/*ToUpper(*/rhs.Ns->GetPrefix()/*.ToString()* /)*/ == /*ToUpper(*/Ns->GetPrefix()/*.ToString()* /)*/) && 
							(/*ToUpper(*/rhs.Name.ToString()/*)*/ == /*ToUpper(*/Name.ToString()/*)*/));
		if (!rhs.Ns.is_init() && !Ns.is_init())
			return /*ToUpper(*/rhs.Name.ToString()/*)*/ == /*ToUpper(*/Name.ToString()/*)*/;
		return false;
	}


	const bool XName::operator ==(const XName& rhs) const
	{
		if (!rhs.Ns.is_init() || !Ns.is_init())
			return /*ToUpper(*/Name.ToString()/*)*/ == /*ToUpper(*/rhs.Name.ToString()/*)*/;
		return	((/*ToUpper(*/Ns->GetPrefix()/*.ToString()* /)*/ == /*ToUpper(*/rhs.Ns->GetPrefix()/*.ToString()* /)*/) && 
						(/*ToUpper(*/Name.ToString()/*)*/ == /*ToUpper(*/rhs.Name.ToString()/*)*/));
	}


	const std::string XName::ToString() const
	{
		if (!Ns.is_init())
			return Name;
		return Ns->GetPrefix()/*.ToString()*/ + ":" + Name.ToString();
	}


	const std::wstring XName::ToWString() const
	{
		if (!Ns.is_init())
			return Encoding::utf82unicode(Name.ToString());
		return Encoding::utf82unicode(Ns->GetPrefix()/*.ToString()*/) + L":" + Encoding::utf82unicode(Name.ToString());
	}


	void XName::SaveToStringList(std::list<std::string>& strList)const
	{
		if (!Ns.is_init())
		{
			strList.push_back(*Name);
			return;
		}
		strList.push_back(Ns->GetPrefix()/*.ToString()*/);// + ":" + *Name);
		strList.push_back(":");
		strList.push_back(*Name);
	}


	void XName::SaveToWStringList(std::list<std::wstring>& strList)const
	{
		if (!Ns.is_init())
		{
			strList.push_back(Encoding::utf82unicode(*Name));
			return;
		}
		strList.push_back(Encoding::utf82unicode(Ns->GetPrefix())/*.ToString()*/);// + ":" + *Name);
		strList.push_back(L":");
		strList.push_back(Encoding::utf82unicode(*Name));
	}


	const XName operator +(const XNamespace& ns, const std::string& name)
	{
		return XName(ns, name);
	}


	const XName operator +(const XNamespace& ns, const char* name)
	{
		return XName(ns, name);
	}

} // namespace XML