#pragma once
#ifndef PPTX_LOGIC_GRADFILL_INCLUDE_H_
#define PPTX_LOGIC_GRADFILL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../../Limit/Flip.h"
#include "./../Rect.h"
#include "./../Gs.h"
#include "./../Lin.h"
#include "./../Path.h"

#include <list>

namespace PPTX
{
	namespace Logic
	{

		class GradFill : public WrapperWritingElement
		{
		public:
			GradFill();
			virtual ~GradFill();
			explicit GradFill(const XML::XNode& node);
			const GradFill& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			void Merge(GradFill& fill)const;
		public:
			nullable_property<std::string, Limit::Flip> flip;
			nullable_property<bool> rotWithShape;

			property<std::list<Gs> > GsLst;
			nullable_property<Lin> lin;
			nullable_property<Path> path;
			nullable_property<Rect> tileRect;
		private:
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GRADFILL_INCLUDE_H_