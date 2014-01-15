#pragma once
#ifndef OOX_LOGIC_RUN_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_RUN_PROPERTY_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "nullable_property.h"
#include "Color.h"
#include "BackgroundColor.h"
#include "Index.h"
#include "Lang.h"
#include "RFonts.h"
#include "Shading.h"
#include "Border.h"
#include "RunPropertyChange.h"
#include "./../Unit.h"
#include "./../Limit/UnderType.h"

namespace OOX
{
	namespace Logic
	{
		class RunProperty : public WritingElement
		{
		public:
			RunProperty();
			virtual ~RunProperty();
			explicit RunProperty(const XML::XNode& node);
			const RunProperty& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			static const RunProperty merge(const RunProperty& prev, const RunProperty& current);
				
		public:
			const bool isSimple() const;

		public:
			nullable_property<bool>										Bold;
			nullable_property<bool>										Italic;
			nullable_property<bool>										Under;
			nullable_property<bool>										Strike;
			nullable_property<bool>										DStrike;
			nullable_property<std::string, Limit::UnderType>			UnderType;
            nullable_property<bool>										SmallCaps;
			nullable_property<bool>										Caps;
			nullable_property<bool>										Emboss;
			nullable_property<bool>										Imprint;
			nullable_property<bool>										Outline;
			nullable_property<bool>										Shadow;
			nullable_property<bool>										Vanish;
			nullable_property<bool>										WebHidden;
			nullable_property<double>									FontSize;
			nullable_property<std::string>								RStyle;
			nullable_property<Lang>										Lang;
			nullable_property<Index>									Index;
			nullable_property<Color>									FontColor;
			//nullable_property<Color>									Background;
			nullable_property<BackgroundColor>							Highlight;
			nullable_property<Shading>									Shading;
			nullable_property<RFonts>									rFonts;
			nullable_property<UnitDx>									Spacing;
			nullable_property<int, setter::between<int, 0, 3276> >		Kern;
            nullable_property<int, setter::between<int, -3168, 3168> >	Position;
            nullable_property<int, setter::between<int, 1, 600> >		Scale;
			nullable_property<Border>									Border;
			nullable_property<RunPropertyChange>                        PropertyChange;
			
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_RUN_PROPERTY_INCLUDE_H_