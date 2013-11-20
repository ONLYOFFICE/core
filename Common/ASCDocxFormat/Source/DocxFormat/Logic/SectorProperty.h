#ifndef OOX_LOGIC_SECTOR_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_SECTOR_PROPERTY_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
#include "PageSize.h"
#include "LnNumType.h"
#include "PageMargin.h"
#include "Columns.h"
#include "DocumentGrid.h"
#include "HeaderReference.h"
#include "FooterReference.h"
#include "FootNoteProperty.h"
#include "EndNoteProperty.h"
#include "./../Limit/SectPrType.h"
#include "PageBorders.h"


namespace OOX
{
	namespace Logic
	{
		class SectorProperty : public WritingElement
		{
		public:
			SectorProperty();
			virtual ~SectorProperty();
			explicit SectorProperty(const XML::XNode& node);
			const SectorProperty& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<PageSize>									PageSize;
			property<PageMargin>								PageMargin;
			property<LnNumType>									LnNumType;
			nullable_property<Columns>							Columns;
			nullable_property<DocumentGrid>						DocumentGrid;
			nullable_property<std::string, Limit::SectPrType>	Type;
			property<std::vector<HeaderReference> >				Headers;
			property<std::vector<FooterReference> >				Footers;
			property<bool>										TitlePage;
			nullable_property<PageBorders>						PageBorders;

			// TODO исправить, могут быть не всегда
			nullable_property<FootNoteProperty>					FootNoteProperty;
			nullable_property<EndNoteProperty>					EndNoteProperty;
		};
	} // namespace Logic
}// namespace OOX

#endif // OOX_LOGIC_SECTOR_PROPERTY_INCLUDE_H_