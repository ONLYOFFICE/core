#pragma once
#ifndef XML_XNAMESPACE_INCLUDE_H_
#define XML_XNAMESPACE_INCLUDE_H_

#include "Private/XPointer.h"
#include "Private/Namespace.h"


namespace XML
{
	class XNamespace : public Private::XPointer<Private::Namespace>
	{
	public:
		XNamespace(const std::string& prefix, const std::string& name);
		XNamespace(const XNamespace& ns);

	public:
		const XNamespace& operator= (const XNamespace& rhs);
		const bool operator==(const XNamespace& rhs) const;
		const bool operator<(const XNamespace& rhs) const;


	public:
		virtual const bool exist() const;

	public:
		const std::string GetPrefix()const;
		const std::string GetName()const;

	protected:
		XNamespace();
	};
} // namespace XML

#endif // XML_XNAMESPACE_INCLUDE_H_