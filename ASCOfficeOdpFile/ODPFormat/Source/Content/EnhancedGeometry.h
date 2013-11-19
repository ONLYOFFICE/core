#pragma once
#ifndef ODP_CONTENT_ENHANCED_GEOMETRY_INCLUDE_H_
#define ODP_CONTENT_ENHANCED_GEOMETRY_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "property.h"
#include <string>
#include <vector>
#include "OdtFormat/Content/Handle.h"
#include "OdtFormat/Content/Equation.h"
#include "OdtFormat/Limit/ShapeType.h"

namespace Odp
{
	namespace Content
	{
		class EnhancedGeometry : public Odt::WritingElement
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
			nullable_property<std::string> ViewBox;
			property<std::string, Odt::Limit::ShapeType> Type;
			property<std::string> EnhancedPath;
			nullable_property<std::string> TextAreas;
			nullable_property<std::string> Modifiers;
			property<std::vector<Odt::Content::Equation> > Equations;
			property<std::vector<Odt::Content::Handle> > Handles;
			nullable_property<bool> MirrorHorizontal;
			nullable_property<bool> MirrorVertical;
			nullable_property<std::string> GluePoints;
		public:
			const std::string GetODString() const;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_ENHANCED_GEOMETRY_INCLUDE_H_