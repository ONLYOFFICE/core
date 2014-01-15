#ifndef OOX_LOGIC_COLUMN_INCLUDE_H_
#define OOX_LOGIC_COLUMN_INCLUDE_H_

#include "./../WritingElement.h"
#include "Utility.h"
#include "property.h"
#include "nullable_property.h"


namespace OOX
{
	namespace Logic
	{
		class Column : public WritingElement
		{
		public:
			Column();
			virtual ~Column();
			explicit Column(const XML::XNode& node);
			const Column& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<UniversalUnit>				Width;
			nullable_property<UniversalUnit>	Space;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_COLUMN_INCLUDE_H_