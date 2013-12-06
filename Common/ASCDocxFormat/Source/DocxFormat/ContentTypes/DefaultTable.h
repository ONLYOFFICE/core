#pragma once
#ifndef OOX_CONTENT_TYPES_DEFAULT_TABLE_INCLUDE_H_
#define OOX_CONTENT_TYPES_DEFAULT_TABLE_INCLUDE_H_

#include "./../WritingVector.h"
#include "Default.h"

namespace OOX
{
	namespace ContentTypes
	{
		class DefaultTable : public WritingVector<Default>
		{
		public:
			DefaultTable();
			virtual ~DefaultTable();
			explicit DefaultTable(const XML::XNode& node);
			const DefaultTable& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			void add(const OOX::CPath& path);
		};
	} // namespace ContentTypes
} // namespace OOX

#endif // OOX_CONTENT)TYPES_DEFAULT_TABLE_INCLUDE_H_