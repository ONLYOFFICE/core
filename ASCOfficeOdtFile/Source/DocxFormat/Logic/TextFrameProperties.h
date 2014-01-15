#ifndef OOX_LOGIC_TEXT_FRAME_PROPERTIES_INCLUDE_H_
#define OOX_LOGIC_TEXT_FRAME_PROPERTIES_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
#include "./../Unit.h"
#include "./../Limit/HorizontalAnchorLocation.h"
#include "./../Limit/VerticalAnchorLocation.h"
#include "./../Limit/TextFrameType.h"
#include "./../Limit/DropCap.h"



namespace OOX
{
	namespace Logic
	{
		class TextFrameProperties : public WritingElement
		{
		public:
			TextFrameProperties();
			virtual ~TextFrameProperties();
			explicit TextFrameProperties(const XML::XNode& node);
			const TextFrameProperties& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
		    nullable_property<std::string, Limit::HorizontalAnchorLocation>	HAnchor;
			nullable_property<std::string, Limit::VerticalAnchorLocation>	VAnchor;
            nullable_property<std::string, Limit::TextFrameType>	        Wrap;
		    nullable_property<UniversalUnit>								X;
            nullable_property<UniversalUnit>								Y;
			nullable_property<UniversalUnit>								H;
			nullable_property<UniversalUnit>								W;
			nullable_property<UniversalUnit>								HSpace;
		    nullable_property<UniversalUnit>								VSpace;
			nullable_property<int>											Lines;
			nullable_property<std::string, Limit::DropCap>					DropCap;
		};
	} // namespace Logic
}// namespace OOX

#endif // OOX_LOGIC_TEXT_FRAME_PROPERTIES_INCLUDE_H_