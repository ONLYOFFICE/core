#pragma once
#ifndef OOX_CONTENT_TYPES_OVERRIDE_INCLUDE_H_
#define OOX_CONTENT_TYPES_OVERRIDE_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <string>
#include <boost/filesystem.hpp>


namespace OOX
{
	namespace ContentTypes
	{
		class Override : public WritingElement
		{
		public:
			Override(const std::string& type, const boost::filesystem::wpath& path);
			virtual ~Override();
			explicit Override(const XML::XNode& node);
			const Override& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const std::string type() const;
			const boost::filesystem::wpath filename() const;

		private:
			std::string								m_type;
			boost::filesystem::wpath	m_part;
		};
	} // namespace ContentTypes
} // namespace OOX

#endif // OOX_CONTENT_TYPES_OVERRIDE_INCLUDE_H_