#pragma once
#ifndef OOX_LOGIC_TABLE_IND_INCLUDE_H_
#define OOX_LOGIC_TABLE_IND_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"


namespace OOX
{
	namespace Logic
	{
		class TableInd : public WritingElement
		{
		public:
			TableInd();
			virtual ~TableInd();
			explicit TableInd(const XML::XNode& node);
			const TableInd& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int>			Width;
			property<std::string>	Type;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_STYLES_TABLE_IND_INCLUDE_H_