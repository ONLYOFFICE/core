#pragma once
#ifndef OOX_CONTENT_TYPES_OVERRIDE_INCLUDE_H_
#define OOX_CONTENT_TYPES_OVERRIDE_INCLUDE_H_

#include <string>

#include "./../WritingElement.h"
#include "property.h"

namespace OOX
{
	namespace ContentTypes
	{
		class Override : public WritingElement
		{
		public:
			Override(const std::string& type, const OOX::CPath& path);
			virtual ~Override();
			explicit Override(const XML::XNode& node);
			const Override& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const std::string type() const;
			const OOX::CPath filename() const;

		private:
			std::string	m_type;
			OOX::CPath	m_part;
		};
	} // namespace ContentTypes
} // namespace OOX

#endif // OOX_CONTENT_TYPES_OVERRIDE_INCLUDE_H_