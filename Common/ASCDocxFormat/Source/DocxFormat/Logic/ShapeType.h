#pragma once
#ifndef OOX_LOGIC_SPAPE_TYPE_INCLUDE_H_
#define OOX_LOGIC_SPAPE_TYPE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "nullable_property.h"
#include "./../Limit/TrueFalse.h"
#include "Stroke.h"
#include "Formulas.h"
#include "Lock.h"
#include "Path.h"
#include "Handles.h"
#include "Complex.h"
#include "textPath.h"

namespace OOX
{
	namespace Logic
	{
		class ShapeType : public WritingElement
		{
		public:
			ShapeType();
			virtual ~ShapeType();			
			explicit ShapeType(const XML::XNode& node);
			const ShapeType& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>								Id;
			property<std::string>								coordsize;
			property<int>										Spt;
			property<std::string>								path;			
			nullable_property<Stroke>							Stroke;
			nullable_property<Formulas>							formulas;
			nullable_property<Lock>								Lock;
			nullable_property<Path>								PathElement;
			nullable_property<Handles>							Handles;
			nullable_property<TextPath>							TextPath;
			nullable_property<std::string>						adj;
			nullable_property<std::string, Limit::TrueFalse>	Filled;
			nullable_property<std::string, Limit::TrueFalse>	Stroked;
			nullable_property<std::string, Limit::TrueFalse>	Preferrelative;
			nullable_property<Complex>							Complex;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SPAPE_TYPE_INCLUDE_H_