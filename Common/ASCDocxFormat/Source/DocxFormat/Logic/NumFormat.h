#pragma once
#ifndef OOX_LOGIC_NUM_FORMAT_INCLUDE_H_
#define OOX_LOGIC_NUM_FORMAT_INCLUDE_H_

#include <string>
#include "Common.h"
#include "./../Namespaces.h"
#include "./../WritingElement.h"


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
			explicit NumFormat(const XML::XNode& node);
			explicit NumFormat(const std::string& format);

		public:
			const NumFormat& operator =(const Common::NumFormat& numFormat);
			const NumFormat& operator =(const XML::XNode& node);
			const NumFormat& operator =(const std::string& format);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const std::string ToString() const;
			void fromString(const std::string& value);
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_NUM_FORMAT_INCLUDE_H_