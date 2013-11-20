#pragma once
#ifndef XML_PRIVATE_XATTRIBUTE_INCLUDE_H_
#define XML_PRIVATE_XATTRIBUTE_INNLUDE_H_

#include <string>
#include "XString.h"
#include "./../XName.h"
#include "property.h"
#include <list>


namespace XML
{
	namespace Private
	{
		class Attribute
		{
		public:
			Attribute(const XName& xname, const std::string& value);

		public:
			const bool operator ==(const Attribute& rhs) const;
			const bool operator <(const Attribute& rhs) const;
			virtual const bool exist() const;

		public:
			const std::string ToString() const;
			const std::wstring ToWString() const;
			virtual void SaveToStringList(std::list<std::string>& strList)const;
			virtual void SaveToWStringList(std::list<std::wstring>& strList)const;

		public:
			const XString value() const;

		public:
			property<XName>							XName;
			property<Private::XString>	Value;

		protected:
			Attribute();
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_ATTRIBUTE_INCLUDE_H_