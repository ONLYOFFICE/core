#ifndef OOX_LOGIC_PAGE_MARGIN_INCLUDE_H_
#define OOX_LOGIC_PAGE_MARGIN_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
#include "./../Unit.h"


namespace OOX
{
	namespace Logic
	{
		class PageMargin : public WritingElement
		{
		public:
			PageMargin();
			virtual ~PageMargin();
			explicit PageMargin(const XML::XNode& node);
			const PageMargin& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int, setter::only_positive<int> > Top;
			property<int, setter::only_positive<int> > Right;
			property<int, setter::only_positive<int> > Bottom;
			property<int, setter::only_positive<int> > Left;
			nullable_property<UnitDx> Header;
			nullable_property<UnitDx> Footer;
			nullable_property<UnitDx> Gutter;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PAGE_MARGIN_INCLUDE_H_