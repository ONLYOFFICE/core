#pragma once
#ifndef ODT_CONTENT_ENHANCED_GEOMETRY_INCLUDE_H_
#define ODT_CONTENT_ENHANCED_GEOMETRY_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <string>
#include <vector>
#include "Handle.h"
#include "Equation.h"
#include "./../Limit/ShapeType.h"
#include "./../Limit/TextPathMode.h"
#include "./../Limit/TextPathScale.h"


namespace Odt
{
	namespace Content
	{
		class EnhancedGeometry : public WritingElement
		{
		public:
			EnhancedGeometry();
			virtual ~EnhancedGeometry();
			explicit EnhancedGeometry(const XML::XNode& node);
			const EnhancedGeometry& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>									  ViewBox;
			property<std::string, Limit::ShapeType> Type;
			property<std::string>										EnhancedPath;
			nullable_property<std::string>					GluePoints;			
			nullable_property<std::string>					TextAreas;
			nullable_property<bool>									TextPath;
			nullable_property<std::string, Limit::TextPathMode>		TextPathMode;
			nullable_property<std::string, Limit::TextPathScale>	TextPathScale;
			nullable_property<bool>									TextPathSameLetterHeights;
			nullable_property<std::string>					Modifiers;
			property<std::vector<Equation> >				Equations;
			property<std::vector<Handle> >					Handles;
			nullable_property<bool>									MirrorHorizontal;
			nullable_property<bool>									MirrorVertical;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_ENHANCED_GEOMETRY_INCLUDE_H_