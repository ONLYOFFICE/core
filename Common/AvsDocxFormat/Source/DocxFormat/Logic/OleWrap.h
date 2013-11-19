#pragma once
#ifndef OOX_LOGIC_OLE_WRAP_INCLUDE_H_
#define OOX_LOGIC_OLE_WRAP_INCLUDE_H_

#include <string>
#include "./../Namespaces.h"
#include "Common.h"
#include "nullable_property.h"

namespace OOX
{
	namespace Logic
	{
		class OleWrap : public Common::Wrap
		{
		public:
			OleWrap();
			explicit OleWrap(const Common::Wrap& wrap);
			explicit OleWrap(const std::string& value);
			explicit OleWrap(const XML::XElement& element);
			
			const OleWrap& operator = (const Common::Wrap& wrap);
			const OleWrap& operator = (const std::string& value);

		public:
			const std::string ToString() const;
			void fromString(const std::string& value);

		public:
			nullable_property<std::string> WrapCoords;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_OLE_WRAP_INCLUDE_H_