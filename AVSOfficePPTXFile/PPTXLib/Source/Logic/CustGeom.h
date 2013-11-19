#pragma once
#ifndef PPTX_LOGIC_CUSTGEOM_INCLUDE_H_
#define PPTX_LOGIC_CUSTGEOM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include <list>
#include "Gd.h"
#include "Rect.h"
#include "Path2D.h"
#include "AhBase.h"
#include "Cxn.h"

namespace PPTX
{
	namespace Logic
	{
		class CustGeom : public WrapperWritingElement
		{
		public:
			CustGeom();
			virtual ~CustGeom();			
			explicit CustGeom(const XML::XNode& node);
			const CustGeom& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::list<Gd> > avLst;
			property<std::list<Gd> > gdLst;
			property<std::list<AhBase> > ahLst;
			property<std::list<Cxn> > cxnLst;
			nullable_property<Rect> rect;
			property<std::list<Path2D> > pathLst;
		protected:
			virtual void FillParentPointersForChilds();
		public:
			std::string GetODString() const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CUSTGEOM_INCLUDE_H