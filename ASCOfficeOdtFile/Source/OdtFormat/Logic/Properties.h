#pragma once
#ifndef ODT_LOGIC_PROPERTIES_INCLUDE_H_
#define ODT_LOGIC_PROPERTIES_INCLUDE_H_

#include "./../WritingElement.h"
#include "Property/Graphic.h"
#include "Property/ListLevel.h"
#include "Property/Paragraph.h"
#include "Property/Section.h" 
#include "Property/TableCell.h"
#include "Property/TableColumn.h"
#include "Property/Table.h"
#include "Property/TableRow.h"
#include "Property/Text.h"
#include "Property/DrawingPage.h"
#include "nullable_property.h"


namespace Odt
{
	namespace Logic
	{
		class Properties : public WritingElement
		{
		public:
			Properties();
			virtual ~Properties();
			explicit Properties(const XML::XNode& node);
			const Properties& operator =(const XML::XNode& node);
		
		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			static const Properties merge(const Properties& prev, const Properties& current);

		public:
			const bool isSimple() const;

		public:
			nullable_property<Property::Graphic>		Graphic;
			nullable_property<Property::ListLevel>		ListLevel;
			nullable_property<Property::Paragraph>		Paragraph;
			nullable_property<Property::Section>		Section;
			nullable_property<Property::TableCell>		TableCell;
			nullable_property<Property::TableColumn>	TableColumn;
			nullable_property<Property::Table>			Table;
			nullable_property<Property::TableRow>		TableRow;
			nullable_property<Property::Text>			Text;
			nullable_property<Property::DrawingPage>	DrawingPage;

		private:
			template<class T, class S, class G>
			static const nullable_property<T, S, G> merge(const nullable_property<T, S, G>& prev, const nullable_property<T, S, G>& current)
			{
				if (!current.is_init())
					return prev;
				if (!prev.is_init() && current.is_init())
					return current;
				return T::merge(prev, current);
			}
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_PROPERTIES_INCLUDE_H_