#pragma once
#ifndef OOX_CONTENT_TYPES_OVERRIDE_TABLE_INCLUDE_H_
#define OOX_CONTENT_TYPES_OVERRIDE_TABLE_INCLUDE_H_

#include "./../WritingVector.h"
#include "Override.h"
#include <boost/filesystem.hpp>


namespace OOX
{
	namespace ContentTypes
	{
		class OverrideTable : public WritingVector<Override>
		{
		public:
			OverrideTable();
			virtual ~OverrideTable();
			explicit OverrideTable(const XML::XNode& node);
			const OverrideTable& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			void add(const std::string& type, const boost::filesystem::wpath& path);
		};
	} // namespace ContentTypes
} // namespace OOX

#endif // OOX_CONTENT_TYPES_OVERRIDE_TABLE_INCLUDE_H_