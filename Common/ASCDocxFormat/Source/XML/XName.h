#pragma once
#ifndef XML_XNAME_INCLUDE_H_
#define XML_XNAME_INCLUDE_H_

#include <boost/operators.hpp>
#include <string>
#include <boost/shared_ptr.hpp>
#include "property.h"
#include "nullable_property.h"
#include "XNamespace.h"

namespace XML
{
	class XName
	{
	public:
		XName();
		XName(const char* name);
		XName(const std::string& name);
		XName(const XNamespace& ns, const std::string& name);

	public:
		const bool Equal(const XName& rhs) const;
		const bool operator ==(const XName& rhs) const;

		XName const* const	operator->() const	{return this;}
		XName*							operator->()				{return this;}

	public:
		const std::string ToString() const;
		const std::wstring ToWString() const;
		virtual void SaveToStringList(std::list<std::string>& strList)const;
		virtual void SaveToWStringList(std::list<std::wstring>& strList)const;

	public:
		nullable_property<XNamespace>		Ns;
		property<std::string>						Name;
	};

	const XName operator +(const XNamespace& ns, const std::string& name);
	const XName operator +(const XNamespace& ns, const char* name);
} // namespace XML

#endif // XML_XNAME_INCLUDE_H_