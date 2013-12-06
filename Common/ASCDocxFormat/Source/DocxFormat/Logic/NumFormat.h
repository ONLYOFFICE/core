#pragma once
#ifndef OOX_LOGIC_NUM_FORMAT_INCLUDE_H_
#define OOX_LOGIC_NUM_FORMAT_INCLUDE_H_

#include <string>

#include "Common.h"

#include "./../Namespaces.h"
#include "./../WritingElement.h"

#include "../.././../../Common/DocxFormat/Source/Base/Nullable.h"
#include "../.././../../Common/DocxFormat/Source/Xml/XmlUtils.h"

namespace OOX
{
	namespace Logic
	{
		class NumFormat : public Common::NumFormat, public WritingElement
		{
		public:
			NumFormat();
			virtual ~NumFormat();
			explicit NumFormat(const Common::NumFormat& numFormat);
			explicit NumFormat(XmlUtils::CXmlNode& oNode);										//	DELETE
			explicit NumFormat(const std::string& format);
			explicit NumFormat(const std::wstring& format);
			explicit NumFormat(const XML::XNode& node);

		public:
			const NumFormat& operator =(const Common::NumFormat& numFormat);
			const NumFormat& operator =(const XML::XNode& node);							//	DELETE
			const NumFormat& operator =(const std::string& format);
			
			const NumFormat& operator =(XmlUtils::CXmlNode& oNode);

		public:
			virtual void fromXML(const XML::XNode& node);									//	DELETE
			virtual const XML::XNode toXML() const;											//	DELETE
		
			virtual void fromXML(XmlUtils::CXmlNode& oNode);

		public:
			const std::string ToString() const;
			const std::wstring ToStringW() const;
			void fromString(const std::string& value);
			void fromStringW(const std::wstring& value);
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_NUM_FORMAT_INCLUDE_H_