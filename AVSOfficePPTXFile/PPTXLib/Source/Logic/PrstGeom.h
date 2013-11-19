#pragma once
#ifndef PPTX_LOGIC_PRSTGEOM_INCLUDE_H_
#define PPTX_LOGIC_PRSTGEOM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "./../Limit/ShapeType.h"
#include "Gd.h"
#include <list>

namespace PPTX
{
	namespace Logic
	{
		class PrstGeom : public WrapperWritingElement
		{
		public:
			PrstGeom();
			virtual ~PrstGeom();			
			explicit PrstGeom(const XML::XNode& node);
			const PrstGeom& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string, Limit::ShapeType> prst;
			property<std::list<Gd> > avLst;
		protected:
			virtual void FillParentPointersForChilds();
		public:
			std::string GetODString()const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PRSTGEOM_INCLUDE_H