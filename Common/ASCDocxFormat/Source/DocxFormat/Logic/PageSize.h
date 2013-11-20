#ifndef OOX_LOGIC_PAGE_SIZE_INCLUDE_H_
#define OOX_LOGIC_PAGE_SIZE_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
//#include "./../Unit.h"
#include "./../Limit/Orient.h"


namespace OOX
{
	namespace Logic
	{
		class PageSize : public WritingElement
		{
		public:
			PageSize();
			virtual ~PageSize();
			explicit PageSize(const XML::XNode& node);
			const PageSize& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int, setter::only_positive<int> > Width;
			property<int, setter::only_positive<int> > Height;
			nullable_property<std::string, Limit::Orient> Orient;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PAGE_SIZE_INCLUDE_H_