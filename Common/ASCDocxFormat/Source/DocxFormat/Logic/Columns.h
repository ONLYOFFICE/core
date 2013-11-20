#ifndef OOX_LOGIC_COLUMNS_INCLUDE_H_
#define OOX_LOGIC_COLUMNS_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "Utility.h"
#include "property.h"
#include "nullable_property.h"
#include "Column.h"

namespace OOX
{
	namespace Logic
	{
		class Columns : public WritingElement
		{
		public:
			Columns();
			virtual ~Columns();
			explicit Columns(const XML::XNode& node);
			const Columns& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<int>					Num;
			nullable_property<UniversalUnit>		Space;		
			nullable_property<bool>					EqualWidth;
			nullable_property<bool>					Sep;
			property<std::vector<Column> >			Items;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_COLUMNS_INCLUDE_H_