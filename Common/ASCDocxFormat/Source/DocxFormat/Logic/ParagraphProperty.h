#pragma once
#ifndef OOX_LOGIC_PARAGRAPH_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_PARAGRAPH_PROPERTY_INCLUDE_H_

#include "./../WritingElement.h"
#include "RunProperty.h"
#include "Spacing.h"
#include <string>
#include "nullable_property.h"
#include "Color.h"
#include "Align.h"
#include "NumPr.h"
#include "ParagraphBorder.h"
#include "Ind.h"
#include "TabsProperty.h"
#include "Shading.h"
#include "TextFrameProperties.h"
#include "ParagraphPropertyChange.h"
#include "./../Document.h"
#include "./../Limit/TextAlignment.h"

namespace OOX
{
	namespace Logic
	{
		class ParagraphProperty : public WritingElement
		{
		public:
			ParagraphProperty();
			virtual ~ParagraphProperty();
			explicit ParagraphProperty(const XML::XNode& node);
			const ParagraphProperty& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:			
			const bool isSimple() const;

		public:
			nullable_property<std::string>				PStyle;
			nullable_property<RunProperty>				RunProperty;

			nullable_property<Align>					Align;
			//nullable_property<Color>					Background;
			nullable_property<Spacing>					Spacing;
			nullable_property<NumPr>					NumPr;
			nullable_property<ParagraphBorder>			ParagraphBorder;
			nullable_property<Ind>						Ind;
		
			nullable_property<Shading>                  Shading; //TODO: Read only now
			nullable_property<TextFrameProperties>      TextFrameProperties;

			nullable_property<SectorProperty>			SectorProperty;
			nullable_property<int>						OutlineLvl;
			nullable_property<TabsProperty>				Tabs;
			property<bool>								KeepNext;
			property<bool>								KeepLines;
			property<bool>								pageBreakBefore;
			property<bool>                              ContextualSpacing;
			property<bool>								SuppressLineNumbers;
			nullable_property<bool>						WidowControl;

			nullable_property<std::string, Limit::TextAlignment> TextAlignment;
			nullable_property<ParagraphPropertyChange> PropertyChange;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PARAGRAPH_PROPERTY_INCLUDE_H_