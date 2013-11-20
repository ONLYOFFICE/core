#pragma once
#ifndef XML_XDECLARATION_INCLUDE_H_
#define XML_XDECLARATION_INCLUDE_H_

#include <string>
#include "property.h"
#include "nullable_property.h"
#include "./../Limit/Version.h"
#include "./../Limit/Encoding.h"
#include "./../Limit/Standalone.h"
//#include "XSource.h"
#include "XSingleSource.h"
#include "XWideSource.h"


namespace XML
{
	namespace Private
	{
		class XDeclaration
		{
		public:
			XDeclaration();

		public:
			void fromSource(boost::shared_ptr<XSingleSource> source);
			void fromSource(boost::shared_ptr<XWideSource> source);
			const std::string ToString() const;
			const std::wstring ToWString() const;

		public:
			property<std::string, Limit::Version>		Version;
			property<std::string, Limit::Encoding>	Encoding;
			nullable_property<std::string, Limit::Standalone>		Standalone;

		private:
			void setValue(const std::string& name, const std::string& value);
		};
	} // namespace Private
} // namespace XML

#endif // XML_XDECLARATION_INCLUDE_H_