#pragma once
#ifndef OOX_LOGIC_TABLE_STYLE_INCLUDE_H_
#define OOX_LOGIC_TABLE_STYLE_INCLUDE_H_

#include "./../WritingVector.h"
#include "TableStyleProperties.h"

namespace OOX
{
	namespace Logic
	{
		class TableStyle : public OOX::WritingVector<TableStyleProperties>
		{
		public:
			TableStyle();
			virtual ~TableStyle();
			explicit TableStyle(const XML::XNode& node);
			const TableStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			const bool exist(const std::string& type) const;
			const TableStyleProperties& operator[](const std::string& type) const;
			TableStyleProperties& operator[](const std::string& type);

			const TableStyleProperties& find(const std::string& type) const;
			TableStyleProperties& find(const std::string& type);
		};
	}//namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_STYLE_INCLUDE_H_